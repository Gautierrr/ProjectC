#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "../header/create_account.h"
#include "../header/main_menu.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 750;

SDL_Texture *createAccountButton(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect *buttonRect)
{
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    SDL_QueryTexture(textTexture, NULL, NULL, &(buttonRect->w), &(buttonRect->h));
    buttonRect->x = (SCREEN_WIDTH - buttonRect->w) / 2;
    buttonRect->y = (SCREEN_HEIGHT - buttonRect->h) / 2;

    return textTexture;
}

void showCreateAccountPage(SDL_Renderer *renderer, TTF_Font *font, enum ProgramState *currentState)
{
    SDL_Texture *backButtonTexture;
    SDL_Rect backButtonRect;

    backButtonTexture = createAccountButton(renderer, font, "Retour a la page d'accueil", &backButtonRect);

    SDL_Event e;
    bool quit = false;

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    quit = true;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (mouseX >= backButtonRect.x && mouseX <= backButtonRect.x + backButtonRect.w &&
                    mouseY >= backButtonRect.y && mouseY <= backButtonRect.y + backButtonRect.h)
                {
                    *currentState = STATE_MAIN_MENU;
                    quit = true;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Dessiner les éléments de la page de création de compte ici

        SDL_RenderCopy(renderer, backButtonTexture, NULL, &backButtonRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(backButtonTexture);
}
