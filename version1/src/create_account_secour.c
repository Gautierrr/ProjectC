#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../header/create_account.h"
#include "../header/main_menu.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 750;
const int TEXT_INPUT_MAX = 50;

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

SDL_Texture *renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y)
{
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);

    return textTexture;
}

void showCreateAccountPage(SDL_Renderer *renderer, TTF_Font *font, enum ProgramState *currentState)
{
    SDL_Texture *backButtonTexture;
    SDL_Rect backButtonRect;

    backButtonTexture = createAccountButton(renderer, font, "Retour a la page d'accueil", &backButtonRect);

    const char *pageTitle = "Creation de votre compte";
    const char *usernameTitle = "Nom d'utilisateur";
    const char *passwordTitle = "Mot de passe";

    char usernameInput[TEXT_INPUT_MAX] = "";
    char passwordInput[TEXT_INPUT_MAX] = "";

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
                else if (e.key.keysym.sym == SDLK_BACKSPACE)
                {
                    if (SDL_strlen(usernameInput) > 0)
                    {
                        usernameInput[SDL_strlen(usernameInput) - 1] = '\0';
                    }
                }
            }
            else if (e.type == SDL_TEXTINPUT)
            {
                if (SDL_strlen(usernameInput) + SDL_strlen(e.text.text) < TEXT_INPUT_MAX - 1)
                {
                    SDL_strlcat(usernameInput, e.text.text, TEXT_INPUT_MAX);
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

        renderText(renderer, font, pageTitle, (SCREEN_WIDTH - strlen(pageTitle)) / 2.5, 20);
        renderText(renderer, font, usernameTitle, 20, 150);
        renderText(renderer, font, passwordTitle, 20, 250);

        renderText(renderer, font, usernameInput, 150, 150);
        renderText(renderer, font, passwordInput, 150, 250);

        SDL_RenderCopy(renderer, backButtonTexture, NULL, &backButtonRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(backButtonTexture);
}












/*
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

SDL_Texture *renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y)
{
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);

    return textTexture;
}

void showCreateAccountPage(SDL_Renderer *renderer, TTF_Font *font, enum ProgramState *currentState)
{
    SDL_Texture *backButtonTexture;
    SDL_Rect backButtonRect;

    backButtonTexture = createAccountButton(renderer, font, "Retour a la page d'accueil", &backButtonRect);

    const char *pageTitle = "Creation de votre compte";
    const char *usernameTitle = "Nom d'utilisateur";
    const char *passwordTitle = "Mot de passe";

    char usernameInput[50] = "";
    char passwordInput[50] = "";

    SDL_Event e;
    bool quit = false;

    SDL_Texture *pageTitleTexture = NULL;
    SDL_Texture *usernameTitleTexture = NULL;
    SDL_Texture *passwordTitleTexture = NULL;

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
                else if (e.key.keysym.sym == SDLK_BACKSPACE)
                {
                    if (SDL_strlen(usernameInput) > 0)
                    {
                        usernameInput[SDL_strlen(usernameInput) - 1] = '\0';
                    }
                }
            }
            else if (e.type == SDL_TEXTINPUT)
            {
                if (SDL_strlen(usernameInput) + SDL_strlen(e.text.text) < 49)
                {
                    SDL_strlcat(usernameInput, e.text.text, 50);
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

        renderText(renderer, font, pageTitle, (SCREEN_WIDTH - strlen(pageTitle)) / 2, 20);

        renderText(renderer, font, usernameTitle, 20, 100);
        renderText(renderer, font, passwordTitle, 20, 150);

        // Dessiner les éléments de la page de création de compte ici

        renderText(renderer, font, usernameInput, 150, 100);
        renderText(renderer, font, passwordInput, 150, 150);

        SDL_RenderCopy(renderer, backButtonTexture, NULL, &backButtonRect);

        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(pageTitleTexture);
        SDL_DestroyTexture(usernameTitleTexture);
        SDL_DestroyTexture(passwordTitleTexture);
    }

    SDL_DestroyTexture(backButtonTexture);
}
*/










































#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "../header/connect_account.h"
#include "../header/main_menu.h"

void showConnectAccountPage(SDL_Renderer *renderer, TTF_Font *font, enum ProgramState *currentState)
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

        SDL_RenderCopy(renderer, backButtonTexture, NULL, &backButtonRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(backButtonTexture);
}