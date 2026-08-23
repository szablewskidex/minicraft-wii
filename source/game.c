#include "crafting/crafting.h"
#include "gfx/spritesheet.h"
#include "gfx/font.h"

#ifdef USE_SDL1
	#include <SDL/SDL.h>
	/* SDL1 compatibility layer */
	typedef SDL_Surface* SDL_Window;
	#define SDL_WINDOWPOS_UNDEFINED 0
	#define SDL_GetError() SDL_GetError()
#else
	#include <SDL2/SDL.h>
#endif

#include "game.h"
#include "inputhandler.h"
#include "utils/arraylist.h"
#include "utils/utils.h"
#include "screen/menu.h"
#include "level/tile/tile.h"
#include "item/resource/resource.h"
#include "level/level.h"
#include "level/levelgen/levelgen.h"
#include "gfx/color.h"
#include "entity/player.h"
#include "entity/_entity_caller.h"
#include "item/item.h"
#include "item/tool_item.h"
#include "item/resourceitem.h"
#include "icons.h"
#include "save.h"
#include "sound.h"
#include "lang.h"

// Helper: print available SDL video drivers (useful for embedded / RISC-V / no-X11 systems)
static void print_sdl_video_drivers(void) {
#ifdef USE_SDL1
    char driver[32];
    if (SDL_VideoDriverName(driver, sizeof(driver))) {
        printf("SDL1 video driver: %s\n", driver);
    } else {
        printf("SDL1 video driver: (unknown)\n");
    }
#else
    int num = SDL_GetNumVideoDrivers();
    printf("Available SDL2 video drivers (%d):\n", num);
    if (num <= 0) {
        printf("  (none)\n");
        return;
    }
    for (int i = 0; i < num; ++i) {
        const char* drv = SDL_GetVideoDriver(i);
        printf("  %d: %s\n", i, drv ? drv : "(null)");
    }
    const char* current = SDL_GetCurrentVideoDriver();
    if (current) {
        printf("Current driver: %s\n", current);
    }
#endif
}

// Set video driver hint (called before SDL_Init)
static void set_video_driver_hint(void) {
    const char* driver = NULL;

#ifdef USE_FB
    #ifdef USE_SDL1
        driver = "fbcon";   // SDL1 framebuffer driver (needs /dev/fb0)
    #else
        driver = "KMSDRM";  // SDL2 modern framebuffer (needs DRM/KMS)
    #endif
#endif

    if (driver) {
#ifdef USE_SDL1
        // SDL1: use environment variable
        char env[128];
        snprintf(env, sizeof(env), "SDL_VIDEODRIVER=%s", driver);
        putenv(env);
#else
        SDL_SetHint(SDL_HINT_VIDEODRIVER, driver);
#endif
        printf("Forcing video driver: %s\n", driver);
    }

    // Allow user override via env var
    const char* env_driver = getenv("SDL_VIDEODRIVER");
    if (env_driver && !driver) {
#ifdef USE_SDL1
        char env[128];
        snprintf(env, sizeof(env), "SDL_VIDEODRIVER=%s", env_driver);
        putenv(env);
#else
        SDL_SetHint(SDL_HINT_VIDEODRIVER, env_driver);
#endif
        printf("Using SDL_VIDEODRIVER from environment: %s\n", env_driver);
    }
}

Screen game_screen;
Screen game_lightScreen;
int* prevBuf = NULL;

int g_ticks = 0; //perf measure
int g_frames = 0; //perf measure

unsigned long tickCount = 0;

SDL_Color sdl_colors[256];

enum menu_id current_menu;
char game_hasfocus = 0;
int game_pendingLevelChange = 0;
char updatePerfctr = 0;
char running = 1;
char isingame = 0;

int game_playerDeadTime = 0;
int game_wonTimer = 0;
int game_gameTime = 0;
int game_currentLevel;
char game_hasWon = 0;

Level game_levels[5] = {0};
Level* game_level = NULL;
Player* game_player = NULL;

const int MAX_FPS = -1;

// NOTE: this must be always an array, or C will treat this like read-only
char CLICK_TO_FOCUS[] = "Click to focus!";


void game_set_menu(enum menu_id menu) {
	current_menu = menu;
	if (menu) {
		init_menu(menu);
		if (menu == mid_TITLE || menu == mid_SLOT_SELECT || menu == mid_NEW_GAME || menu == mid_OPTIONS || menu == mid_ABOUT || menu == mid_INSTRUCTIONS || menu == mid_PAUSE) {
			music_play_menu();
		}
	} else {
		music_set_level(game_currentLevel);
		music_play_game();
	}
}


void game_changeLevel(int dir) {
	level_removeEntity1(game_level, &game_player->mob.entity);

	game_currentLevel += dir;
	game_level = game_levels + game_currentLevel;
	game_player->mob.entity.x = (game_player->mob.entity.x >> 4) * 16 + 8;
	game_player->mob.entity.y = (game_player->mob.entity.y >> 4) * 16 + 8;

	level_addEntity(game_level, &game_player->mob.entity);
	music_set_level(game_currentLevel);
	save_game(NULL);
}


void game_won(){
	game_wonTimer = 60 * 3;
	game_hasWon = 1;
}


#include "gamemode.h"

void game_reset() {
	game_playerDeadTime = 0;
	game_wonTimer = 0;
	game_gameTime = 0;
	game_hasWon = 0;

	for (int i = 0; i < 5; ++i) {
		level_free(game_levels + i);
	}

	if (!isingame) {
        return;
    }

	memset(game_levels, 0, sizeof(game_levels));

	int size = (int)g_worldSize;
	if (size != 64 && size != 128 && size != 256) size = 128;

	game_currentLevel = 3;
	level_init(game_levels + 4, size, size, 1, 0);
	level_init(game_levels + 3, size, size, 0, game_levels + 4);
	level_init(game_levels + 2, size, size, -1, game_levels + 3);
	level_init(game_levels + 1, size, size, -2, game_levels + 2);
	level_init(game_levels + 0, size, size, -3, game_levels + 1);

	if (game_player) {
		call_entity_free(&game_player->mob.entity);
		free(game_player);
	}

	game_level = game_levels + game_currentLevel;
	game_player = (Player*) malloc(sizeof(Player));
	player_create(game_player);
	player_findStartPos(game_player, game_level);

	if (g_gameMode == MODE_CREATIVE) {
		// Creative mode starting inventory
		Item cItem;
		toolitem_create(&cItem, SWORD, 4); // Gem Sword
		inventory_addItem(&game_player->inventory, &cItem);
		toolitem_create(&cItem, PICKAXE, 4); // Gem Pickaxe
		inventory_addItem(&game_player->inventory, &cItem);
		toolitem_create(&cItem, AXE, 4); // Gem Axe
		inventory_addItem(&game_player->inventory, &cItem);
		toolitem_create(&cItem, SHOVEL, 4); // Gem Shovel
		inventory_addItem(&game_player->inventory, &cItem);

		resourceitem_create_cnt(&cItem, &wood, 999);
		inventory_addItem(&game_player->inventory, &cItem);
		resourceitem_create_cnt(&cItem, &stone, 999);
		inventory_addItem(&game_player->inventory, &cItem);
		resourceitem_create_cnt(&cItem, &ironIngot, 999);
		inventory_addItem(&game_player->inventory, &cItem);
		resourceitem_create_cnt(&cItem, &goldIngot, 999);
		inventory_addItem(&game_player->inventory, &cItem);
		resourceitem_create_cnt(&cItem, &gem, 999);
		inventory_addItem(&game_player->inventory, &cItem);
	}

	level_addEntity(game_level, &game_player->mob.entity);

	for (int i = 0; i < 5; ++i) {
        level_trySpawn(game_levels + i, 5000);
    }
}


#include "sound.h"
#include "lang.h"

void game_init(){
	levelgen_preinit();
	font_pre_init();
	init_resources();
	init_tiles();
	init_menus();
	crafting_init();
	sound_init();
	lang_init();

	int pp = 0;
	for (int r = 0; r < 6; ++r) {
		for (int g = 0; g < 6; ++g) {
			for (int b = 0; b < 6; ++b) {
				int rr = r*255 / 5;
				int gg = g*255 / 5;
				int bb = b*255 / 5;
				int mid = (rr * 30 + gg * 59 + bb * 11) / 100;

				int r1 = ((rr + mid * 1) / 2) * 230 / 255 + 10;
				int g1 = ((gg + mid * 1) / 2) * 230 / 255 + 10;
				int b1 = ((bb + mid * 1) / 2) * 230 / 255 + 10;

				sdl_colors[pp].r = r1;
				sdl_colors[pp].g = g1;
				sdl_colors[pp].b = b1;
				++pp;
			}
		}
	}

	create_screen(&game_screen, 426, HEIGHT, (SpriteSheet*)&icons_spritesheet);
	create_screen(&game_lightScreen, 426, HEIGHT, (SpriteSheet*)&icons_spritesheet);
	game_screen.w = (g_aspectRatio == 1) ? 426 : 320;
	game_lightScreen.w = game_screen.w;

	game_reset();
	game_set_menu(mid_TITLE);
}


void set_pixel(SDL_Surface* surface, int x, int y, int color){
	*(int*)(surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel) = color;
}


void game_tick(){
	++tickCount;

	if (!game_hasfocus) {
		// TODO release all keys
	} else {
		if (isingame) {
            if (!game_player->mob.entity.removed && !game_hasWon) {
                ++game_gameTime;
                if (game_gameTime > 0 && (game_gameTime % 1800 == 0)) {
                    save_game(NULL);
                }
            }
        }

		input_tick();
		sound_tick();

		if (current_menu) {
			tick_menu(current_menu);
		} else {
			if (game_player->mob.entity.removed) {
				++game_playerDeadTime;
				if (game_playerDeadTime > 60) {
					game_set_menu(mid_DEAD);
				}
			} else {
				if (game_pendingLevelChange != 0) {
					game_set_menu(mid_LEVEL_TRANSITION);
					game_pendingLevelChange = 0;
				}
			}

			if (game_wonTimer > 0) {
				if (--game_wonTimer == 0) {
					game_set_menu(mid_WON);
				}
			}

			level_tick(game_level);
			++tile_tickCount;
		}
	}
}


void game_renderGui() {

    // -DTEST_SHOWPORTALPOS
    #ifdef TEST_SHOWPORTALPOS
        if (isingame) {
            char hax[64];

            int x = game_player->mob.entity.x >> 4;
            int y = game_player->mob.entity.y >> 4;

            sprintf(hax, "P %d %d", x, y);
            font_draw(hax, strlen(hax), &game_screen, 2, 2, getColor4(000, 200, 500, 533));

            int Scnt = 10;

            for (x = 0; x < game_player->mob.entity.level->w; ++x) {
                for (y = 0; y < game_player->mob.entity.level->h; ++y) {
                    if (level_get_tile(game_player->mob.entity.level, x, y) == STAIRS_UP) {
                        sprintf(hax, "U %d %d", x, y);
                        font_draw(hax, strlen(hax), &game_screen, 2, Scnt, getColor4(000, 200, 500, 533));
                        Scnt += 8;
                    }

                    if (level_get_tile(game_player->mob.entity.level, x, y) == STAIRS_DOWN) {
                        sprintf(hax, "D %d %d", x, y);
                        font_draw(hax, strlen(hax), &game_screen, 2, Scnt, getColor4(000, 200, 500, 533));
                        Scnt += 8;
                    }
                }
            }

            if (game_player->mob.entity.level->depth == 1) {
                for (int i = 0; i < game_player->mob.entity.level->entities.size; ++i) {
                    Entity* e = game_player->mob.entity.level->entities.elements[i];
                    if (e->type == AIRWIZARD) {
                        sprintf(hax, "W %d %d", e->x>> 4, e->y>> 4);
                        font_draw(hax, strlen(hax), &game_screen, 2, Scnt, getColor4(000, 200, 500, 533));
                        Scnt += 8;
                        break;
                    }
                }
            }
        }
    #endif


    #ifdef FPS_AND_TICKS
        char fpsticks[64];
        sprintf(fpsticks, "%dfps %dticks", g_frames, g_ticks);
        font_draw(fpsticks, strlen(fpsticks), &game_screen, 2, 2, getColor4(000, 200, 500, 533));
    #endif

	int hud_x = 12;
	int hud_y = (g_buttonPrompts) ? (game_screen.h - 38) : (game_screen.h - 22);

	if (isingame){
        /// RENDER THE HUD with safe margins
        // 1. Health Bar (Multi-row support for level ups!)
        int max_hearts = game_player->mob.maxHealth;
        int cur_health = game_player->mob.health;
        for (int h = 0; h < max_hearts; ++h) {
            int row = h / 10;
            int col = h % 10;
            int hx = hud_x + col * 8;
            int hy = hud_y - row * 8;

            if (h < cur_health) {
                render_screen(&game_screen, hx, hy, 0 + 12 * 32, getColor4(-1, 200, 500, 533), 0);
            } else {
                render_screen(&game_screen, hx, hy, 0 + 12 * 32, getColor4(-1, 100, 000, 000), 0);
            }
        }

        // 2. Stamina bar
        for (int s = 0; s < 10; ++s) {
            int sx = hud_x + s * 8;
            int sy = hud_y + 8;
            if (game_player->staminaRechargeDelay > 0) {
                if (game_player->staminaRechargeDelay / 4 % 2 == 0) {
                    render_screen(&game_screen, sx, sy, 1 + 12 * 32, getColor4(-1, 555, 000, 000), 0);
                } else {
                    render_screen(&game_screen, sx, sy, 1 + 12 * 32, getColor4(-1, 110, 000, 000), 0);
                }
            } else {
                if (s < game_player->stamina) {
                    render_screen(&game_screen, sx, sy, 1 + 12 * 32, getColor4(-1, 220, 550, 553), 0);
                } else {
                    render_screen(&game_screen, sx, sy, 1 + 12 * 32, getColor4(-1, 110, 000, 000), 0);
                }
            }
        }

        // Player Level & EXP Bar in center of HUD
        int exp_x = hud_x + 92;
        char lvl_str[16];
        sprintf(lvl_str, "LV%d", game_player->level);
        font_draw(lvl_str, strlen(lvl_str), &game_screen, exp_x, hud_y, getColor4(-1, 550, 550, 550));

        int exp_pct = (game_player->maxExp > 0) ? (game_player->exp * 8 / game_player->maxExp) : 0;
        for (int e = 0; e < 8; ++e) {
            if (e < exp_pct) {
                render_screen(&game_screen, exp_x + e * 8, hud_y + 8, 2 + 12 * 32, getColor4(-1, 050, 252, 050), 0);
            } else {
                render_screen(&game_screen, exp_x + e * 8, hud_y + 8, 2 + 12 * 32, getColor4(-1, 020, 020, 020), 0);
            }
        }

        // Equipped Armor display in HUD
        if (game_player->armor) {
            render_screen(&game_screen, exp_x + 40, hud_y, game_player->armor->sprite, game_player->armor->color, 0);
            char def_str[8];
            sprintf(def_str, "+%d", game_player->armorDefense);
            font_draw(def_str, strlen(def_str), &game_screen, exp_x + 48, hud_y, getColor4(-1, 333, 444, 555));
        }

        // Active item / tool box on the right side of HUD (lowered for clean alignment)
        int tool_x = game_screen.w - 96;
        int tool_y = hud_y + 8;
        if (game_player->activeItem) {
            item_renderInventory(game_player->activeItem, &game_screen, tool_x, tool_y);
        } else {
            const char* no_item = (g_currentLanguage == LANG_PL) ? "[Reka]" : "[Hand]";
            font_draw((char*)no_item, strlen(no_item), &game_screen, tool_x + 8, tool_y, getColor4(-1, 333, 333, 333));
        }

        // Top-right Minimap Radar (Enlarged to 54x42)
        int map_w = 54;
        int map_h = 42;
        int map_x = game_screen.w - map_w - 6;
        int map_y = 6;
        int p_tx = game_player->mob.entity.x >> 4;
        int p_ty = game_player->mob.entity.y >> 4;

        for (int my = 0; my < map_h; ++my) {
            for (int mx = 0; mx < map_w; ++mx) {
                int sx = map_x + mx;
                int sy = map_y + my;

                if (mx == 0 || mx == map_w - 1 || my == 0 || my == map_h - 1) {
                    game_screen.pixels[sx + sy * game_screen.w] = getColor(111); // Sleek dark border
                    continue;
                }

                int world_tx = p_tx - (map_w / 2) + mx;
                int world_ty = p_ty - (map_h / 2) + my;

                if (world_tx < 0 || world_ty < 0 || world_tx >= game_level->w || world_ty >= game_level->h) {
                    game_screen.pixels[sx + sy * game_screen.w] = getColor(000);
                    continue;
                }

                TileID t = level_get_tile(game_level, world_tx, world_ty);
                int pix_col = getColor(000);

                if (mx == map_w / 2 && my == map_h / 2) {
                    pix_col = getColor(555); // Player is bright white dot
                } else if (t == WATER || t == LAVA) {
                    pix_col = (t == WATER) ? getColor(004) : getColor(400);
                } else if (t == TREE) {
                    pix_col = getColor(030);
                } else if (t == ROCK || t == HARD_ROCK) {
                    pix_col = (t == HARD_ROCK) ? getColor(111) : getColor(222);
                } else if (t == SAND) {
                    pix_col = getColor(441);
                } else if (t == DIRT || t == FARMLAND) {
                    pix_col = getColor(210);
                } else if (t == WOOD_FLOOR || t == WOOD_WALL) {
                    pix_col = getColor(321);
                } else if (t == STAIRS_DOWN || t == STAIRS_UP) {
                    pix_col = getColor(550);
                } else if (t == GRASS || t == FLOWER) {
                    pix_col = (t == FLOWER) ? getColor(405) : getColor(040);
                } else if (t == CACTUS || t == CLOUD_CACTUS) {
                    pix_col = getColor(031);
                } else if (t == CLOUD) {
                    pix_col = getColor(444);
                } else {
                    pix_col = getColor(020);
                }

                game_screen.pixels[sx + sy * game_screen.w] = pix_col;
            }
        }

        // Render Minecraft-style HUD button prompt hints at bottom
        if (g_buttonPrompts) {
            int py = game_screen.h - 18;
            int text_y = py + 4;
            int textColor = getColor4(-1, 000, 444, 555);
            int cur_x = (game_screen.w >= 400) ? 14 : 6;

            if (g_activeControllerType == 0) {
                // GameCube Controller Prompts (Large 16x16 Badges)
                // 1. (A) Atak / Attack - Emerald Green
                render_screen(&game_screen, cur_x,     py,     0 + 44 * 32, getColor4(-1, 000, 041, 555), 0);
                render_screen(&game_screen, cur_x + 8, py,     1 + 44 * 32, getColor4(-1, 000, 041, 555), 0);
                render_screen(&game_screen, cur_x,     py + 8, 0 + 45 * 32, getColor4(-1, 000, 041, 555), 0);
                render_screen(&game_screen, cur_x + 8, py + 8, 1 + 45 * 32, getColor4(-1, 000, 041, 555), 0);
                cur_x += 18;
                const char* t_atk = (g_currentLanguage == LANG_PL) ? "Atak" : "Attack";
                font_draw((char*)t_atk, strlen(t_atk), &game_screen, cur_x, text_y, textColor);
                cur_x += strlen(t_atk) * 8 + 8;

                // 2. (Y) Plecak / Inv (Changed to Y as requested!)
                render_screen(&game_screen, cur_x,     py,     6 + 44 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x + 8, py,     7 + 44 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x,     py + 8, 6 + 45 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x + 8, py + 8, 7 + 45 * 32, getColor4(-1, 000, 333, 555), 0);
                cur_x += 18;
                const char* t_inv = (g_currentLanguage == LANG_PL) ? "Plecak" : "Inv";
                font_draw((char*)t_inv, strlen(t_inv), &game_screen, cur_x, text_y, textColor);
                cur_x += strlen(t_inv) * 8 + 8;

                // 3. (R) Przedmiot / Item
                render_screen(&game_screen, cur_x,     py,     12 + 44 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x + 8, py,     13 + 44 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x,     py + 8, 12 + 45 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x + 8, py + 8, 13 + 45 * 32, getColor4(-1, 000, 333, 555), 0);
                cur_x += 18;
                const char* t_itm = (g_currentLanguage == LANG_PL) ? "Przedmiot" : "Item";
                font_draw((char*)t_itm, strlen(t_itm), &game_screen, cur_x, text_y, textColor);
                cur_x += strlen(t_itm) * 8 + 8;

                // 4. (Start) Pauza / Pause
                render_screen(&game_screen, cur_x,     py,     14 + 44 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x + 8, py,     15 + 44 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x,     py + 8, 14 + 45 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x + 8, py + 8, 15 + 45 * 32, getColor4(-1, 000, 333, 555), 0);
                cur_x += 18;
                const char* t_pau = (g_currentLanguage == LANG_PL) ? "Pauza" : "Pause";
                font_draw((char*)t_pau, strlen(t_pau), &game_screen, cur_x, text_y, textColor);
            } else {
                // Wiimote Controller Prompts (Large 16x16 Badges)
                // 1. (A) Atak / Attack
                render_screen(&game_screen, cur_x,     py,     16 + 44 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x + 8, py,     17 + 44 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x,     py + 8, 16 + 45 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x + 8, py + 8, 17 + 45 * 32, getColor4(-1, 000, 333, 555), 0);
                cur_x += 18;
                const char* t_atk = (g_currentLanguage == LANG_PL) ? "Atak" : "Attack";
                font_draw((char*)t_atk, strlen(t_atk), &game_screen, cur_x, text_y, textColor);
                cur_x += strlen(t_atk) * 8 + 8;

                // 2. (1) Plecak / Inv
                render_screen(&game_screen, cur_x,     py,     20 + 44 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x + 8, py,     21 + 44 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x,     py + 8, 20 + 45 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x + 8, py + 8, 21 + 45 * 32, getColor4(-1, 000, 333, 555), 0);
                cur_x += 18;
                const char* t_inv = (g_currentLanguage == LANG_PL) ? "Plecak" : "Inv";
                font_draw((char*)t_inv, strlen(t_inv), &game_screen, cur_x, text_y, textColor);
                cur_x += strlen(t_inv) * 8 + 8;

                // 3. (B) Przedmiot / Item
                render_screen(&game_screen, cur_x,     py,     18 + 44 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x + 8, py,     19 + 44 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x,     py + 8, 18 + 45 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x + 8, py + 8, 19 + 45 * 32, getColor4(-1, 000, 333, 555), 0);
                cur_x += 18;
                const char* t_itm = (g_currentLanguage == LANG_PL) ? "Przedmiot" : "Item";
                font_draw((char*)t_itm, strlen(t_itm), &game_screen, cur_x, text_y, textColor);
                cur_x += strlen(t_itm) * 8 + 8;

                // 4. (+) Pauza / Pause
                render_screen(&game_screen, cur_x,     py,     24 + 44 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x + 8, py,     25 + 44 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x,     py + 8, 24 + 45 * 32, getColor4(-1, 000, 333, 555), 0);
                render_screen(&game_screen, cur_x + 8, py + 8, 25 + 45 * 32, getColor4(-1, 000, 333, 555), 0);
                cur_x += 18;
                const char* t_pau = (g_currentLanguage == LANG_PL) ? "Pauza" : "Pause";
                font_draw((char*)t_pau, strlen(t_pau), &game_screen, cur_x, text_y, textColor);
            }
        }
	}

	if (current_menu) {
		render_menu(current_menu, &game_screen);
	}
}


void game_renderFocusNagger() {
	//click_to_focus
	int c2fLen = strlen(CLICK_TO_FOCUS);
	int xx = (WIDTH - c2fLen * 8) / 2;
	int yy = (HEIGHT - 8) / 2;

	int w = c2fLen;
	int h = 1;

	render_screen(&game_screen, xx - 8, yy - 8, 0 + 13 * 32, getColor4(-1, 1, 5, 445), 0);
	render_screen(&game_screen, xx + w * 8, yy - 8, 0 + 13 * 32, getColor4(-1, 1, 5, 445), 1);
	render_screen(&game_screen, xx - 8, yy + 8, 0 + 13 * 32, getColor4(-1, 1, 5, 445), 2);
	render_screen(&game_screen, xx + w * 8, yy + 8, 0 + 13 * 32, getColor4(-1, 1, 5, 445), 3);

	for (int x = 0; x < w; ++x) {
		render_screen(&game_screen, xx + x * 8, yy - 8, 1 + 13 * 32, getColor4(-1, 1, 5, 445), 0);
		render_screen(&game_screen, xx + x * 8, yy + 8, 1 + 13 * 32, getColor4(-1, 1, 5, 445), 2);
	}

	for (int y = 0; y < h; ++y) {
		render_screen(&game_screen, xx - 8, yy + y * 8, 2 + 13 * 32, getColor4(-1, 1, 5, 445), 0);
		render_screen(&game_screen, xx + w * 8, yy + y * 8, 2 + 13 * 32, getColor4(-1, 1, 5, 445), 1);
	}

	if ((tickCount / 20) % 2 == 0) {
		font_draw(CLICK_TO_FOCUS, c2fLen, &game_screen, xx, yy, getColor4(5, 333, 333, 333));
	} else {
		font_draw(CLICK_TO_FOCUS, c2fLen, &game_screen, xx, yy, getColor4(5, 555, 555, 555));
	}
}


void game_render() {
	if (isingame) {
		int xScroll = game_player->mob.entity.x - game_screen.w / 2;
		int yScroll = game_player->mob.entity.y - (game_screen.h - 8) / 2;

		if (xScroll < 16) xScroll = 16;
		if (yScroll < 16) yScroll = 16;
		if (xScroll > game_level->w * 16 - game_screen.w - 16) xScroll = game_level->w * 16 - game_screen.w - 16;
		if (yScroll > game_level->h * 16 - game_screen.h - 16) yScroll = game_level->h * 16 - game_screen.h - 16;

		if (game_currentLevel > 3) {
			int col = getColor4(20, 20, 121, 121);

			for (int y = 0; y < 28; ++y) {
				for (int x = 0; x < 38; ++x) {
					render_screen(&game_screen, x * 8 - ((xScroll / 4) & 7), y * 8 - ((yScroll / 4) & 7), 0, col, 0);
				}
			}
		}

		level_renderBackground(game_level, &game_screen, xScroll, yScroll);
		level_renderSprites(game_level, &game_screen, xScroll, yScroll);

		if (game_currentLevel < 3) {
			clear_screen(&game_lightScreen, 0);
			renderLight(game_level, &game_lightScreen, xScroll, yScroll);
			screen_overlay(&game_screen, &game_lightScreen, xScroll, yScroll);
		} else if (game_currentLevel == 3) {
			int dayTime = game_gameTime % 24000;
			int ambient = 255;

			if (dayTime >= 10000 && dayTime < 13000) {
				// Smooth dusk: darkness slowly creeps in from outer edges towards center
				ambient = 255 * (13000 - dayTime) / 3000;
			} else if (dayTime >= 13000 && dayTime < 21000) {
				// Full night
				ambient = 0;
			} else if (dayTime >= 21000 && dayTime < 24000) {
				// Smooth dawn: morning light gradually expands in circle
				ambient = 255 * (dayTime - 21000) / 3000;
			}

			if (ambient < 255) {
				clear_screen(&game_lightScreen, ambient);
				renderLight(game_level, &game_lightScreen, xScroll, yScroll);
				screen_overlay(&game_screen, &game_lightScreen, xScroll, yScroll);
			}
		}
	}

	game_renderGui();

	if (!game_hasfocus){
		game_renderFocusNagger();
	}
}


#ifdef __wii__
#include <fat.h>
#include <wiiuse/wpad.h>
#include <ogc/pad.h>
#endif

int main(int argc, char** argv) {
#ifdef __wii__
	fatInitDefault();
	WPAD_Init();
	PAD_Init();
#endif
	unsigned long long int lastTime = getTimeUS();
	unsigned long long int lastPrinted = lastTime;
	double unprocessed = 0;

	const double usPerTick = 1000000.0 / 60;

	unsigned long long int nextExceptedFrameRenderTime = 0;
	unsigned long long int now  = 0;
	int ticks = 0, frames = 0;

	int ret = 0;
	int winHeight = HEIGHT * SCALE;
	int winWidth = WIDTH * SCALE;
	char needsFlip = 0;
	int flipXMin = 0, flipXMax = 0, flipYMin = 0, flipYMax = 0;

	// printf("Starting...\n");

#ifdef USE_SDL1
	SDL_Surface* window = NULL;   /* In SDL1 the "window" is the video surface */
	SDL_Surface* surface = NULL;
#else
	SDL_Window* window = NULL;
	SDL_Surface* surface = NULL;
#endif
	SDL_Event event;
#ifdef USE_SDL1
	SDL_KeyboardEvent* keyEvent = (SDL_KeyboardEvent*) &event;
#else
	SDL_KeyboardEvent* keyEvent = (SDL_KeyboardEvent*) &event;
#endif
	SDL_Rect pixel = {0, 0, SCALE, SCALE};

	game_init();

#ifndef __wii__
	// Set video driver hint (FB or SDL_VIDEODRIVER env) BEFORE SDL_Init
	set_video_driver_hint();
#endif

	// Initialize SDL and create the window
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("SDL_Init Error: %s\n", SDL_GetError());
		print_sdl_video_drivers();
		printf("\nHint: Try setting SDL_VIDEODRIVER=KMSDRM (SDL2) or fbcon (SDL1)\n");
		printf("      or export SDL_VIDEODRIVER=... before running.\n");
		printf("      On headless / embedded / RISC-V systems you may need DRM/KMS or /dev/fb0 in kernel.\n");
		ret = 1;
		goto QUIT;
	}

#ifdef USE_SDL1
	/* SDL 1.2: use SDL_SetVideoMode */
#ifdef __wii__
	window = SDL_SetVideoMode(winWidth, winHeight, 16, SDL_DOUBLEBUF | SDL_HWSURFACE);
#else
	window = SDL_SetVideoMode(winWidth, winHeight, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
#endif
	if (!window) {
		printf("Failed to set video mode (SDL1): %s\n", SDL_GetError());
		ret = 1;
		goto QUIT;
	}
	surface = window;   /* In SDL1 the returned surface IS the display surface */

	SDL_WM_SetCaption("Minicraft", NULL);
#else
	window = SDL_CreateWindow("Minicraft", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winWidth, winHeight, 0);
	if (!window) {
		printf("Failed to create window: %s\n", SDL_GetError());
		ret = 1;
		goto QUIT;
	}

	SDL_SetWindowTitle(window, "Minicraft");

	// Get the window's surface
	surface = SDL_GetWindowSurface(window);

	if (surface == NULL){
		printf("Failed to get window surface: %s\n", SDL_GetError());
		ret = 1;
		goto QUIT;
	}
#endif

	// Set palette (SDL1 vs SDL2)
	if (surface->format->palette != NULL) {
#ifdef USE_SDL1
		SDL_SetColors(surface, (SDL_Color*)sdl_colors, 0, 256);
#else
		SDL_SetPaletteColors(surface->format->palette, sdl_colors, 0, 256);
#endif
	}

    // -DLEVELGENTEST (only supported on SDL2 for now)
    #if defined(LEVELGENTEST) && !defined(USE_SDL1)
	{
		#define set_px(x, y, color) {             \
			pixel.x = (x)*SCALE;                  \
			pixel.y = (y)*SCALE;                  \
			SDL_FillRect(surface, &pixel, color); \
		}

		int w = 128;
		int h = 128;

		unsigned char* map;
		unsigned char* data;

		pixel.w = SCALE;
		pixel.h = SCALE;

		createAndValidateTopMap(&map, &data, w, h);
		// printf("gen stopped\n");

		// Create a new window for the levelgen test
		SDL_Window* genWindow = SDL_CreateWindow("LevelGen Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w * SCALE, h * SCALE, 0);
		SDL_Surface* genSurface = SDL_GetWindowSurface(genWindow);

		for (int y = 0; y < h; ++y) {
			for (int x = 0; x < w; ++x) {
				int i = x + y * w;

				if (map[i] == WATER) set_px(x, y, 0x000080);
				if (map[i] == GRASS) set_px(x, y, 0x208020);
				if (map[i] == ROCK) set_px(x, y, 0xa0a0a0);
				if (map[i] == DIRT) set_px(x, y, 0x604040);
				if (map[i] == SAND) set_px(x, y, 0xa0a040);
				if (map[i] == TREE) set_px(x, y, 0x003000);
				if (map[i] == LAVA) set_px(x, y, 0xff2020);
				if (map[i] == CLOUD) set_px(x, y, 0xa0a0a0);
				if (map[i] == STAIRS_DOWN) set_px(x, y, 0xffffff);
				if (map[i] == STAIRS_UP) set_px(x, y, 0xffffff);
				if (map[i] == CLOUD_CACTUS) set_px(x, y, 0xff00ff);
			}
		}

		SDL_UpdateWindowSurface(genWindow);

		while (running) {
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
					case SDL_QUIT: running = 0; break;
				}
			}
		}

		free(map);
		free(data);
		goto QUIT;
	}
    #endif


	prevBuf = malloc(sizeof(int) * 426 * 240);
    if (!prevBuf) {
        printf("Failed to allocate prevBuf memory!\n");
        ret = 1;
        goto QUIT;
    }

	memset(prevBuf, 0xFF, sizeof(int) * 426 * 240);

	game_hasfocus = 1;

	while (running) {

		now = getTimeUS();
		unprocessed += (now - lastTime) / usPerTick;


		while (unprocessed >= 1) {
			++ticks;
			game_tick();
			--unprocessed;
		}

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_KEYUP:
#ifdef USE_SDL1
					input_toggle(event.key.keysym.sym, 0);
#else
					input_toggle(keyEvent->keysym.sym, 0);
#endif
					break;
				case SDL_KEYDOWN:
#ifdef USE_SDL1
					input_toggle(event.key.keysym.sym, 1);
#else
					input_toggle(keyEvent->keysym.sym, 1);
#endif
					break;
				case SDL_QUIT:
					running = 0;
					break;
#ifdef USE_SDL1
				case SDL_ACTIVEEVENT:
					if (event.active.state & SDL_APPACTIVE) {
						game_hasfocus = event.active.gain;
					}
					break;
#else
				case SDL_WINDOWEVENT:
					// To manage focus, the WINDOWEVENT_FOCUS_GAINED/LOST event is used
					if(event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
						game_hasfocus = 0;
					else if(event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
						game_hasfocus = 1;
					break;
#endif
			}
		}

		needsFlip = 0;
		flipXMin = winWidth;
		flipXMax = 0;
		flipYMin = winHeight;
		flipYMax = 0;

        if (MAX_FPS >= 30) {
            if (now < nextExceptedFrameRenderTime){
                goto SKIP_RENDER;
            } else{
                nextExceptedFrameRenderTime = now + 1000000 / MAX_FPS;
            }
        }

		++frames;
		game_render();

		for (int y = 0; y < game_screen.h; ++y) {
			int dest_y0 = y * SCALE;
			int dest_y1 = dest_y0 + SCALE;

			for (int x = 0; x < game_screen.w; ++x) {
				int index = y * game_screen.w + x;
				int screen_px = game_screen.pixels[index];

				if (screen_px != prevBuf[index]) {
					prevBuf[index] = screen_px;
					needsFlip = 1;

					int dest_x0, dest_x1;
					if (g_aspectRatio == 1) {
						dest_x0 = (x * winWidth) / 426;
						dest_x1 = ((x + 1) * winWidth) / 426;
					} else {
						dest_x0 = x * SCALE;
						dest_x1 = dest_x0 + SCALE;
					}

					if (dest_x0 < flipXMin) flipXMin = dest_x0;
					if (dest_x1 > flipXMax) flipXMax = dest_x1;
					if (dest_y0 < flipYMin) flipYMin = dest_y0;
					if (dest_y1 > flipYMax) flipYMax = dest_y1;

					// Convert index palette to color.
					Uint32 mapped_color = SDL_MapRGB(surface->format, sdl_colors[screen_px].r, sdl_colors[screen_px].g, sdl_colors[screen_px].b);

					if (surface->format->BytesPerPixel == 2) {
						for (int sub_y = dest_y0; sub_y < dest_y1; sub_y++) {
							for (int sub_x = dest_x0; sub_x < dest_x1; sub_x++) {
								((Uint16*)surface->pixels)[sub_y * (surface->pitch / 2) + sub_x] = (Uint16)mapped_color;
							}
						}
					} else {
						for (int sub_y = dest_y0; sub_y < dest_y1; sub_y++) {
							for (int sub_x = dest_x0; sub_x < dest_x1; sub_x++) {
								((Uint32*)surface->pixels)[sub_y * (surface->pitch / 4) + sub_x] = mapped_color;
							}
						}
					}
				}
			}
		}

	SKIP_RENDER:
		// Update the window surface partially ...
		if (needsFlip) {
			// printf("RENDERING %d %d %d %d\n", flipXMin, flipXMax, flipYMin, flipYMax);
            SDL_Rect updateRect = { flipXMin, flipYMin, flipXMax - flipXMin, flipYMax - flipYMin };

#ifdef USE_SDL1
			/* SDL1: update whole surface or use dirty rects */
			SDL_Flip(surface);                 /* simple double buffer flip */
			/* Alternative for performance: SDL_UpdateRect(surface, flipXMin, flipYMin, ...); */
#else
			SDL_UpdateWindowSurfaceRects(window, &updateRect, 1);
#endif
		}

		if (now - lastPrinted > 1000000) {
			printf("%d ticks, %d fps\n", ticks, frames);
			g_ticks = ticks;
			g_frames = frames;
			ticks = 0;
			frames = 0;
			updatePerfctr = 1;
			lastPrinted = now;
		}

		lastTime = now;
	}

    QUIT:

	if (prevBuf) free(prevBuf);

	// Close SDL and free EVERYTHING
	SDL_Quit();
	crafting_free();
	delete_screen(&game_screen);
	delete_screen(&game_lightScreen);

	for (int i = 0; i < 5; ++i) {
		// printf("Freeing level %d\n", i);
		level_free(game_levels + i);
	}

	if (game_player) {
		call_entity_free(&game_player->mob.entity);
		free(game_player);
	}

	return ret;
}
