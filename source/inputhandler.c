#include "inputhandler.h"
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


void input_tick(){
#ifdef __wii__
    WPAD_ScanPads();
    PAD_ScanPads();

    u32 w_held = WPAD_ButtonsHeld(0);
    u16 g_held = PAD_ButtonsHeld(0);
    s8 stick_x = PAD_StickX(0);
    s8 stick_y = PAD_StickY(0);

    // Support Vertical Wiimote, Horizontal (Sideways) Wiimote, Classic Controller & GameCube Pad
    char move_up    = (w_held & (WPAD_BUTTON_UP | WPAD_BUTTON_RIGHT | WPAD_CLASSIC_BUTTON_UP)) || (g_held & PAD_BUTTON_UP) || (stick_y > 40);
    char move_down  = (w_held & (WPAD_BUTTON_DOWN | WPAD_BUTTON_LEFT | WPAD_CLASSIC_BUTTON_DOWN)) || (g_held & PAD_BUTTON_DOWN) || (stick_y < -40);
    char move_left  = (w_held & (WPAD_BUTTON_LEFT | WPAD_BUTTON_UP | WPAD_CLASSIC_BUTTON_LEFT)) || (g_held & PAD_BUTTON_LEFT) || (stick_x < -40);
    char move_right = (w_held & (WPAD_BUTTON_RIGHT | WPAD_BUTTON_DOWN | WPAD_CLASSIC_BUTTON_RIGHT)) || (g_held & PAD_BUTTON_RIGHT) || (stick_x > 40);

    char act_attack = (w_held & (WPAD_BUTTON_2 | WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A | WPAD_CLASSIC_BUTTON_B)) || (g_held & (PAD_BUTTON_A | PAD_BUTTON_B));
    char act_menu   = (w_held & (WPAD_BUTTON_1 | WPAD_BUTTON_B | WPAD_BUTTON_PLUS | WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_PLUS | WPAD_CLASSIC_BUTTON_X | WPAD_CLASSIC_BUTTON_Y)) || (g_held & (PAD_BUTTON_START | PAD_BUTTON_X | PAD_BUTTON_Y));

    key_toggle(&up, move_up);
    key_toggle(&down, move_down);
    key_toggle(&left, move_left);
    key_toggle(&right, move_right);
    key_toggle(&attack, act_attack);
    key_toggle(&menu, act_menu);
#endif

    key_tick(&up);
    key_tick(&down);
    key_tick(&left);
    key_tick(&right);
    key_tick(&attack);
    key_tick(&menu);
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
        case SDLK_6:
        case SDLK_RIGHT:
            key_toggle(&right, pressed);
            break;
        case SDLK_TAB:
        case SDLK_LALT:
        case SDLK_RALT:
        case SDLK_RETURN:
        case SDLK_x:
            key_toggle(&menu, pressed);
            break;
        case SDLK_SPACE:
        case SDLK_LCTRL:
        case SDLK_INSERT:
        case SDLK_c:
            key_toggle(&attack, pressed);
            break;
        default:
            break;
    }
}
