#ifndef CREATE_ACCOUNT_H
#define CREATE_ACCOUNT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "main_menu.h"

SDL_Texture *createAccountButton(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect *buttonRect);
// SDL_Texture *renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y);
void showCreateAccountPage(SDL_Renderer *renderer, TTF_Font *font, enum ProgramState *currentState);

#endif
