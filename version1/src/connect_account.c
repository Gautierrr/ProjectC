#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../header/connect_account.h"
#include "../header/create_account.h"
#include "../header/main_menu.h"

void login()
{
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
}

void showConnectAccountPage(SDL_Renderer *renderer, TTF_Font *font, enum ProgramState *currentState)
{
    login();

    SDL_Texture *backButtonTexture;
    SDL_Rect backButtonRect;
    backButtonTexture = createAccountButton(renderer, font, "Retour a la page d'accueil", &backButtonRect);

    SDL_Texture *databaseButtonTexture;
    SDL_Rect databaseButtonRect;
    databaseButtonTexture = createAccountButton(renderer, font, "Gestion de la base de donnees", &databaseButtonRect);
    
    const char *successfullConnexion = "Bonjour, felicitations vous avez reussi a vous connecter !!";

    
    renderText(renderer, font, successfullConnexion, 20, 100);
    databaseButtonRect.y = 250;
    backButtonRect.y = 400;

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

                if (mouseX >= databaseButtonRect.x && mouseX <= databaseButtonRect.x + databaseButtonRect.w &&
                    mouseY >= databaseButtonRect.y && mouseY <= databaseButtonRect.y + databaseButtonRect.h)
                {
                    // Rediriger vers la page de gestion de base de données
                    *currentState = STATE_DATABASE_MANAGEMENT;
                    quit = true;
                }
                else if (mouseX >= backButtonRect.x && mouseX <= backButtonRect.x + backButtonRect.w &&
                         mouseY >= backButtonRect.y && mouseY <= backButtonRect.y + backButtonRect.h)
                {
                    // Retour à l'accueil
                    *currentState = STATE_MAIN_MENU;
                    quit = true;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Rendu du bouton pour la page de gestion de base de données
        SDL_RenderCopy(renderer, databaseButtonTexture, NULL, &databaseButtonRect);

        // Rendu du bouton de retour à l'accueil
        SDL_RenderCopy(renderer, backButtonTexture, NULL, &backButtonRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(backButtonTexture);
    SDL_DestroyTexture(databaseButtonTexture);
}
