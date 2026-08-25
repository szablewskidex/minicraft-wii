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

#ifdef __PSP__
#include <pspctrl.h>
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
    s8 gc_stick_x = PAD_StickX(0);
    s8 gc_stick_y = PAD_StickY(0);

    // Read expansion data (Nunchuk / Classic Controller)
    expansion_t exp;
    memset(&exp, 0, sizeof(exp));
    WPAD_Expansion(0, &exp);

    if (g_held != 0 || gc_stick_x > 30 || gc_stick_x < -30 || gc_stick_y > 30 || gc_stick_y < -30) {
        g_activeControllerType = 0; // GameCube
    } else if (w_held != 0 || exp.type != WPAD_EXP_NONE) {
        g_activeControllerType = 1; // Wiimote / Nunchuk
    }

    char move_up    = 0;
    char move_down  = 0;
    char move_left  = 0;
    char move_right = 0;

    // 1. GameCube Controller (D-Pad & Analog Stick)
    if (g_held & PAD_BUTTON_UP)    move_up = 1;
    if (g_held & PAD_BUTTON_DOWN)  move_down = 1;
    if (g_held & PAD_BUTTON_LEFT)  move_left = 1;
    if (g_held & PAD_BUTTON_RIGHT) move_right = 1;
    if (gc_stick_y > 35)  move_up = 1;
    if (gc_stick_y < -35) move_down = 1;
    if (gc_stick_x < -35) move_left = 1;
    if (gc_stick_x > 35)  move_right = 1;

    // 2. Nunchuk Expansion: Walk with Nunchuk Analog Stick (Full 360-degree support)
    if (exp.type == WPAD_EXP_NUNCHUK) {
        float mag = exp.nunchuk.js.mag;
        float ang = exp.nunchuk.js.ang;
        if (mag > 0.28f) {
            // Nunchuk angle: 0 deg = UP, 90 deg = RIGHT, 180 deg = DOWN, 270 deg = LEFT
            if (ang >= 337.5f || ang < 22.5f) {
                move_up = 1;
            } else if (ang >= 22.5f && ang < 67.5f) {
                move_up = 1; move_right = 1;
            } else if (ang >= 67.5f && ang < 112.5f) {
                move_right = 1;
            } else if (ang >= 112.5f && ang < 157.5f) {
                move_down = 1; move_right = 1;
            } else if (ang >= 157.5f && ang < 202.5f) {
                move_down = 1;
            } else if (ang >= 202.5f && ang < 247.5f) {
                move_down = 1; move_left = 1;
            } else if (ang >= 247.5f && ang < 292.5f) {
                move_left = 1;
            } else if (ang >= 292.5f && ang < 337.5f) {
                move_up = 1; move_left = 1;
            }
        }
        // Wiimote D-Pad when Nunchuk is attached (Vertical orientation)
        if (w_held & WPAD_BUTTON_UP)    move_up = 1;
        if (w_held & WPAD_BUTTON_DOWN)  move_down = 1;
        if (w_held & WPAD_BUTTON_LEFT)  move_left = 1;
        if (w_held & WPAD_BUTTON_RIGHT) move_right = 1;
    } else if (exp.type == WPAD_EXP_CLASSIC) {
        // Classic Controller (Left Stick & D-Pad)
        float mag = exp.classic.ljs.mag;
        float ang = exp.classic.ljs.ang;
        if (mag > 0.28f) {
            if (ang >= 337.5f || ang < 22.5f) {
                move_up = 1;
            } else if (ang >= 22.5f && ang < 67.5f) {
                move_up = 1; move_right = 1;
            } else if (ang >= 67.5f && ang < 112.5f) {
                move_right = 1;
            } else if (ang >= 112.5f && ang < 157.5f) {
                move_down = 1; move_right = 1;
            } else if (ang >= 157.5f && ang < 202.5f) {
                move_down = 1;
            } else if (ang >= 202.5f && ang < 247.5f) {
                move_down = 1; move_left = 1;
            } else if (ang >= 247.5f && ang < 292.5f) {
                move_left = 1;
            } else if (ang >= 292.5f && ang < 337.5f) {
                move_up = 1; move_left = 1;
            }
        }
        if (w_held & WPAD_CLASSIC_BUTTON_UP)    move_up = 1;
        if (w_held & WPAD_CLASSIC_BUTTON_DOWN)  move_down = 1;
        if (w_held & WPAD_CLASSIC_BUTTON_LEFT)  move_left = 1;
        if (w_held & WPAD_CLASSIC_BUTTON_RIGHT) move_right = 1;
    } else {
        // 3. Standalone Wiimote (Held Sideways / Horizontal NES-Style by default)
        // Wiimote held sideways: D-Pad on the left hand, Buttons (1/2/A/B) on the right hand.
        // Physical RIGHT -> UP (Move Up)
        // Physical LEFT  -> DOWN (Move Down)
        // Physical UP    -> LEFT (Move Left)
        // Physical DOWN  -> RIGHT (Move Right)
        if (w_held & WPAD_BUTTON_RIGHT) move_up = 1;
        if (w_held & WPAD_BUTTON_LEFT)  move_down = 1;
        if (w_held & WPAD_BUTTON_UP)    move_left = 1;
        if (w_held & WPAD_BUTTON_DOWN)  move_right = 1;
    }

    // Action / Attack / Mine / Confirm:
    // - Wiimote Sideways: Button 2 or Button A
    // - Nunchuk: Button Z (trigger) or Button C
    // - Wiimote Vertical: Button A or Button 2
    // - Classic: Button A / Button B
    // - GameCube: Button A / Button B
    char act_attack = (w_held & (WPAD_BUTTON_2 | WPAD_BUTTON_A | WPAD_NUNCHUK_BUTTON_Z | WPAD_CLASSIC_BUTTON_A | WPAD_CLASSIC_BUTTON_B)) || (g_held & (PAD_BUTTON_A | PAD_BUTTON_B));

    // Menu / Inventory / Crafting:
    // - Wiimote Sideways: Button 1 or Button Minus (-)
    // - Nunchuk: Button C
    // - Wiimote Vertical: Button 1 or Button Minus (-)
    // - Classic: Button X / Button Y
    // - GameCube: Button X / Button Y
    char act_menu   = (w_held & (WPAD_BUTTON_1 | WPAD_BUTTON_MINUS | WPAD_NUNCHUK_BUTTON_C | WPAD_CLASSIC_BUTTON_X | WPAD_CLASSIC_BUTTON_Y)) || (g_held & (PAD_BUTTON_X | PAD_BUTTON_Y));

    // Pause / Settings:
    // - Wiimote: Button Plus (+) or Button Home
    // - GameCube: Start button
    char act_pause  = (w_held & (WPAD_BUTTON_PLUS | WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_PLUS | WPAD_CLASSIC_BUTTON_HOME)) || (g_held & PAD_BUTTON_START);

    // Quick Tool / Item Cycle:
    // Next Item: Wiimote B (trigger on back under index finger), Classic R / ZR, GC R / Z
    // Prev Item: Classic L / ZL, GC L
    char act_next   = (w_held & (WPAD_BUTTON_B | WPAD_CLASSIC_BUTTON_FULL_R | WPAD_CLASSIC_BUTTON_ZR)) || (g_held & (PAD_TRIGGER_R | PAD_TRIGGER_Z));
    char act_prev   = (w_held & (WPAD_CLASSIC_BUTTON_FULL_L | WPAD_CLASSIC_BUTTON_ZL)) || (g_held & PAD_TRIGGER_L);

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

#ifdef __PSP__
    SceCtrlData pad;
    sceCtrlPeekBufferPositive(&pad, 1);

    char move_up    = (pad.Buttons & PSP_CTRL_UP)    || (pad.Ly < 60);
    char move_down  = (pad.Buttons & PSP_CTRL_DOWN)  || (pad.Ly > 195);
    char move_left  = (pad.Buttons & PSP_CTRL_LEFT)  || (pad.Lx < 60);
    char move_right = (pad.Buttons & PSP_CTRL_RIGHT) || (pad.Lx > 195);

    // Cross (X) or Circle (O) for Attack / Action / Select
    char act_attack = (pad.Buttons & (PSP_CTRL_CROSS | PSP_CTRL_CIRCLE)) != 0;

    // Square ([]) or Triangle (/_\) or Select for Inventory / Menu
    char act_menu   = (pad.Buttons & (PSP_CTRL_SQUARE | PSP_CTRL_TRIANGLE | PSP_CTRL_SELECT)) != 0;

    // Start for Pause
    char act_pause  = (pad.Buttons & PSP_CTRL_START) != 0;

    // R / L Triggers for Next / Prev Item
    char act_next   = (pad.Buttons & PSP_CTRL_RTRIGGER) != 0;
    char act_prev   = (pad.Buttons & PSP_CTRL_LTRIGGER) != 0;

    key_toggle(&up, move_up);
    key_toggle(&down, move_down);
    key_toggle(&left, move_left);
    key_toggle(&right, move_right);
    key_toggle(&attack, act_attack);
    key_toggle(&menu, act_menu);
    key_toggle(&pause_key, act_pause);
    key_toggle(&cycle_next, act_next);
    key_toggle(&cycle_prev, act_prev);
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
