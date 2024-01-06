#ifndef CONNECT_ACCOUNT_H
#define CONNECT_ACCOUNT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "main_menu.h"

void login();
SDL_Texture *createAccountButton(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect *buttonRect);
void showConnectAccountPage(SDL_Renderer *renderer, TTF_Font *font, enum ProgramState *currentState);

#endif
