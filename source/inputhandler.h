#ifndef INPUTHANDLER_H_
#define INPUTHANDLER_H_

#ifdef USE_SDL1
	#include <SDL/SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

typedef struct{
	int presses, absorbs;
	char down, clicked;
} Key;

extern Key up, down, left, right, attack, menu;

void key_toggle(Key* key, char pressed);
void key_tick(Key* key);

#ifdef USE_SDL1
void input_toggle(SDLKey key, char pressed);
#else
void input_toggle(SDL_Keycode key, char pressed);
#endif

void input_tick();

#endif /* INPUTHANDLER_H_ */
