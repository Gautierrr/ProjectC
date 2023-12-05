#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "header/create_account.h"
#include "header/connect_account.h"
#include "header/main_menu.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 750;

SDL_Texture *createButton(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect *buttonRect)
{
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    SDL_QueryTexture(textTexture, NULL, NULL, &(buttonRect->w), &(buttonRect->h));
    buttonRect->x = (SCREEN_WIDTH - buttonRect->w) / 2;
    buttonRect->y += 50;

    return textTexture;
}

int main(int argc, char *args[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *font = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Votre gestionnaire de base de données !!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);
    if (font == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Rect connectButtonRect;
    SDL_Rect createAccountButtonRect;

    SDL_Texture *connectButtonTexture = createButton(renderer, font, "Se Connecter", &connectButtonRect);
    SDL_Texture *createAccountButtonTexture = createButton(renderer, font, "Creer un Compte", &createAccountButtonRect);

    enum ProgramState currentState = STATE_MAIN_MENU;

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

            switch (currentState)
            {
            case STATE_MAIN_MENU:
                if (e.type == SDL_MOUSEBUTTONDOWN)
                {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);

                    if (mouseX >= connectButtonRect.x && mouseX <= connectButtonRect.x + connectButtonRect.w &&
                        mouseY >= connectButtonRect.y && mouseY <= connectButtonRect.y + connectButtonRect.h)
                    {
                        currentState = STATE_CONNECT_ACCOUNT;
                    }

                    if (mouseX >= createAccountButtonRect.x && mouseX <= createAccountButtonRect.x + createAccountButtonRect.w &&
                        mouseY >= createAccountButtonRect.y && mouseY <= createAccountButtonRect.y + createAccountButtonRect.h)
                    {
                        currentState = STATE_CREATE_ACCOUNT;
                    }
                }
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        switch (currentState)
        {
        case STATE_MAIN_MENU:
            SDL_RenderCopy(renderer, connectButtonTexture, NULL, &connectButtonRect);
            SDL_RenderCopy(renderer, createAccountButtonTexture, NULL, &createAccountButtonRect);
            break;

        case STATE_CONNECT_ACCOUNT:
            showConnectAccountPage(renderer, font, &currentState);
            break;
        
        case STATE_CREATE_ACCOUNT:
            showCreateAccountPage(renderer, font, &currentState);
            break;
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(connectButtonTexture);
    SDL_DestroyTexture(createAccountButtonTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;

}
