#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
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



    char username[20];
    char userpassword[20];
    int loginSuccess = 0;  // Variable pour vérifier si la connexion est réussie
    int attempts = 3;      // Nombre de tentatives autorisées

    while (attempts > 0 && !loginSuccess) {
        printf("Enter the User Name : ");
        fgets(username, sizeof(username), stdin);

        printf("Enter the Password : ");
        fgets(userpassword, sizeof(userpassword), stdin);

        // Supprimer le saut de ligne ajouté par fgets
        username[strcspn(username, "\n")] = '\0';
        userpassword[strcspn(userpassword, "\n")] = '\0';

        if (!strcmp(username, "user") && !strcmp(userpassword, "123")) {
            printf("\n\n****\tLogin Successful\t****");
            loginSuccess = 1;
        } else {
            attempts--;
            printf("\n\n****\tProvided Information Incorrect. %d attempts remaining.\t****\n", attempts);
        }
    }

    if (!loginSuccess) {
        printf("\n\n****\tLogin failed. Please try again later.\t****");
    }
    





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

        renderText(renderer, font, pageTitle, (SCREEN_WIDTH - strlen(pageTitle)) / 2.5, 20);
        renderText(renderer, font, usernameTitle, 20, 150);
        renderText(renderer, font, passwordTitle, 20, 250);


        SDL_RenderCopy(renderer, backButtonTexture, NULL, &backButtonRect);
        SDL_RenderPresent(renderer);

    }

    SDL_DestroyTexture(backButtonTexture);
}

