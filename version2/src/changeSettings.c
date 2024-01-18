#include "../main.h"

void changeSettings(char *currentUsername, char *currentPassword, SDL_Renderer *renderer)
{
    Student currentUser;

    FILE *fp = fopen("bdd/studentInfo.bin", "rb+");

    if (fp == NULL)
    {
        printf("\n\t\t\tError opening file!\n");
        return;
    }

    int found = 0;

    while (fread(&currentUser, sizeof(currentUser), 1, fp) == 1)
    {
        if (strcmp(currentUsername, currentUser.studentUsername) == 0 &&
            strcmp(currentPassword, currentUser.studentPassword) == 0) {
            found = 1;
            break;
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect usernameRect = { 50, 100, 200, 30 };
    SDL_Rect passwordRect = { 50, 350, 200, 30 };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &usernameRect);
    SDL_RenderFillRect(renderer, &passwordRect);

    SDL_Rect textRect = { 50, 300, 300, 30 };

    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Color textColor = { 255, 255, 255 };
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);
    // SDL_Color color = {255, 255, 255, 255};
    textSurface = TTF_RenderText_Solid(font, "Enter your new Student's Username:", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    textRect.y = 60;

    textSurface = TTF_RenderText_Solid(font, "Enter your new Student's Password:", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_RenderPresent(renderer);

    SDL_Event event;

    int done = 0;
    int isTypingUsername = 1;

    // Initialiser les chaînes de caractères à zéro
    memset(currentUser.studentUsername, 0, sizeof(currentUser.studentUsername));
    memset(currentUser.studentPassword, 0, sizeof(currentUser.studentPassword));

    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = 1;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    if (isTypingUsername) {
                        isTypingUsername = 0;
                    } else {
                        done = 1;
                    }
                }
            } else if (event.type == SDL_TEXTINPUT) {
                if (isTypingUsername) {
                    strcat(currentUser.studentUsername, event.text.text);
                } else {
                    strcat(currentUser.studentPassword, event.text.text);
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &usernameRect);
                SDL_RenderFillRect(renderer, &passwordRect);

                textSurface = TTF_RenderText_Solid(font, currentUser.studentUsername, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &usernameRect);

                // Afficher des étoiles pour le mot de passe
                size_t passwordLength = strlen(currentUser.studentPassword);
                char maskedPassword[passwordLength + 1];
                memset(maskedPassword, '*', passwordLength);
                maskedPassword[passwordLength] = '\0';
                
                textSurface = TTF_RenderText_Solid(font, maskedPassword, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &passwordRect);

                SDL_RenderPresent(renderer);
            }
        }

        SDL_Delay(10);
    }

    if (found) {
        fseek(fp, -((long)sizeof(currentUser)), SEEK_CUR);
        fwrite(&currentUser, sizeof(currentUser), 1, fp);

        printf("\n\n\t\t\tUser settings updated successfully!\n");
    } else {
        printf("\n\n\t\t\tUser not found. Please check your current username and password.\n");
    }

    fclose(fp);

    printf("\n\n\t\t\tEnter any keys to continue.......");
}
