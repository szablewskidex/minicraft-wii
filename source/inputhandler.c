#include "inputhandler.h"
#include "gamemode.h"
#ifdef USE_SDL1
	#include <SDL/SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

#ifdef __wii__
#include <wiiuse/wpad.h>
#include <ogc/pad.h>
#endif

Key up;
Key down;
Key left;
Key right;
Key attack;
Key menu;
Key pause_key;
Key cycle_next;
Key cycle_prev;


void key_toggle(Key* key, char pressed){
    if (pressed != key->down) {
        key->down = pressed;
        if (pressed) {
            ++key->presses;
        }
    }
}


void key_tick(Key* key){
    if (key->absorbs < key->presses) {
        ++key->absorbs;
        key->clicked = 1;
    } else {
        key->clicked = 0;
    }
}


#ifdef __wii__
static int s_rumbleFrames = 0;
void input_rumble(int frames) {
    s_rumbleFrames = frames;
    WPAD_Rumble(0, 1);
    PAD_ControlMotor(0, PAD_MOTOR_RUMBLE);
}
#else
void input_rumble(int frames) { (void)frames; }
#endif

void input_tick(){
#ifdef __wii__
    WPAD_ScanPads();
    PAD_ScanPads();

    u32 w_held = WPAD_ButtonsHeld(0);
    u16 g_held = PAD_ButtonsHeld(0);
    s8 stick_x = PAD_StickX(0);
    s8 stick_y = PAD_StickY(0);

    if (g_held != 0 || stick_x > 30 || stick_x < -30 || stick_y > 30 || stick_y < -30) {
        g_activeControllerType = 0; // GameCube
    } else if (w_held != 0) {
        g_activeControllerType = 1; // Wiimote
    }

    // Movement: D-Pad, Nunchuk stick, Classic stick/D-pad, GameCube stick/D-pad
    char move_up    = (w_held & (WPAD_BUTTON_UP | WPAD_CLASSIC_BUTTON_UP)) || (g_held & PAD_BUTTON_UP) || (stick_y > 40);
    char move_down  = (w_held & (WPAD_BUTTON_DOWN | WPAD_CLASSIC_BUTTON_DOWN)) || (g_held & PAD_BUTTON_DOWN) || (stick_y < -40);
    char move_left  = (w_held & (WPAD_BUTTON_LEFT | WPAD_CLASSIC_BUTTON_LEFT)) || (g_held & PAD_BUTTON_LEFT) || (stick_x < -40);
    char move_right = (w_held & (WPAD_BUTTON_RIGHT | WPAD_CLASSIC_BUTTON_RIGHT)) || (g_held & PAD_BUTTON_RIGHT) || (stick_x > 40);

    // Also support Horizontal (Sideways) Wiimote orientation if player is using 2 as attack and 1 as menu
    if (w_held & WPAD_BUTTON_2) {
        if (w_held & WPAD_BUTTON_RIGHT) move_up = 1;
        if (w_held & WPAD_BUTTON_LEFT)  move_down = 1;
        if (w_held & WPAD_BUTTON_UP)    move_left = 1;
        if (w_held & WPAD_BUTTON_DOWN)  move_right = 1;
    }

    // Action / Attack: Wiimote 2 or A, Nunchuk C, Classic A/B, GC A/B
    char act_attack = (w_held & (WPAD_BUTTON_2 | WPAD_BUTTON_A | WPAD_NUNCHUK_BUTTON_C | WPAD_CLASSIC_BUTTON_A | WPAD_CLASSIC_BUTTON_B)) || (g_held & (PAD_BUTTON_A | PAD_BUTTON_B));

    // Menu / Inventory: Wiimote 1 or Minus, Classic X/Y, GC X/Y
    char act_menu   = (w_held & (WPAD_BUTTON_1 | WPAD_CLASSIC_BUTTON_X | WPAD_CLASSIC_BUTTON_Y)) || (g_held & (PAD_BUTTON_X | PAD_BUTTON_Y));

    // Pause: Wiimote Plus/Home, Classic Plus/Home, GC Start
    char act_pause  = (w_held & (WPAD_BUTTON_PLUS | WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_PLUS | WPAD_CLASSIC_BUTTON_HOME)) || (g_held & PAD_BUTTON_START);

    // Quick Tool / Item Cycle:
    // Next: Wiimote B (trigger under wiimote), Nunchuk Z, Classic R / ZR, GC R / Z
    // Prev: Wiimote Minus, Classic L / ZL, GC L
    char act_next   = (w_held & (WPAD_BUTTON_B | WPAD_NUNCHUK_BUTTON_Z | WPAD_CLASSIC_BUTTON_FULL_R | WPAD_CLASSIC_BUTTON_ZR)) || (g_held & (PAD_TRIGGER_R | PAD_TRIGGER_Z));
    char act_prev   = (w_held & (WPAD_BUTTON_MINUS | WPAD_CLASSIC_BUTTON_FULL_L | WPAD_CLASSIC_BUTTON_ZL)) || (g_held & PAD_TRIGGER_L);

    key_toggle(&up, move_up);
    key_toggle(&down, move_down);
    key_toggle(&left, move_left);
    key_toggle(&right, move_right);
    key_toggle(&attack, act_attack);
    key_toggle(&menu, act_menu);
    key_toggle(&pause_key, act_pause);
    key_toggle(&cycle_next, act_next);
    key_toggle(&cycle_prev, act_prev);

    if (s_rumbleFrames > 0) {
        --s_rumbleFrames;
        if (s_rumbleFrames <= 0) {
            WPAD_Rumble(0, 0);
            PAD_ControlMotor(0, PAD_MOTOR_STOP);
        }
    }
#endif

    key_tick(&up);
    key_tick(&down);
    key_tick(&left);
    key_tick(&right);
    key_tick(&attack);
    key_tick(&menu);
    key_tick(&pause_key);
    key_tick(&cycle_next);
    key_tick(&cycle_prev);
}


#ifdef USE_SDL1
void input_toggle(SDLKey key, char pressed) {
#else
void input_toggle(SDL_Keycode key, char pressed) {
#endif

    switch (key) {
        case SDLK_w:
        case SDLK_UP:
            key_toggle(&up, pressed);
            break;
        case SDLK_s:
        case SDLK_DOWN:
            key_toggle(&down, pressed);
            break;
        case SDLK_a:
        case SDLK_LEFT:
            key_toggle(&left, pressed);
            break;
        case SDLK_d:
        case SDLK_RIGHT:
            key_toggle(&right, pressed);
            break;
        case SDLK_TAB:
        case SDLK_x:
        case SDLK_e:
            key_toggle(&menu, pressed);
            break;
        case SDLK_ESCAPE:
        case SDLK_p:
        case SDLK_RETURN:
            key_toggle(&pause_key, pressed);
            break;
        case SDLK_SPACE:
        case SDLK_LCTRL:
        case SDLK_c:
            key_toggle(&attack, pressed);
            break;
        case SDLK_q:
            key_toggle(&cycle_prev, pressed);
            break;
        case SDLK_r:
            key_toggle(&cycle_next, pressed);
            break;
        default:
            break;
    }
}
