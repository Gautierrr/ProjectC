#include "../main.h"

int isUsernameTaken(const char *username) {
    FILE *fp = fopen("bdd/studentInfo.bin", "rb");
    if (fp == NULL) {
        printf("\n\t\t\tError opening file!\n");
        return 1;
    }

    Student existingUser;
    while (fread(&existingUser, sizeof(existingUser), 1, fp) == 1) {
        if (strcmp(username, existingUser.studentUsername) == 0) {
            fclose(fp);
            return 1; // Username deja pris
        }
    }

    fclose(fp);
    return 0; // Username pas pris
}

void createAccount(SDL_Renderer *renderer) {
    Student studentInformation;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect usernameRect = { 50, 100, 200, 30 };
    SDL_Rect passwordRect = { 50, 350, 200, 30 };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &usernameRect);
    SDL_RenderFillRect(renderer, &passwordRect);

    SDL_Rect textRect = { 50, 60, 300, 30 };

    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Color textColor = { 255, 255, 255 };
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);
    // SDL_Color color = {255, 255, 255, 255};
    textSurface = TTF_RenderText_Solid(font, "Enter Student's Username:", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    textRect.y = 300;

    textSurface = TTF_RenderText_Solid(font, "Enter Student's Password:", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_RenderPresent(renderer);

    SDL_Event event;

    int done = 0;
    int isTypingUsername = 1;

    // Initialiser les chaînes de caractères à zéro
    memset(studentInformation.studentUsername, 0, sizeof(studentInformation.studentUsername));
    memset(studentInformation.studentPassword, 0, sizeof(studentInformation.studentPassword));

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
                    strcat(studentInformation.studentUsername, event.text.text);
                } else {
                    strcat(studentInformation.studentPassword, event.text.text);
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &usernameRect);
                SDL_RenderFillRect(renderer, &passwordRect);

                textSurface = TTF_RenderText_Solid(font, studentInformation.studentUsername, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &usernameRect);

                // Afficher des étoiles pour le mot de passe
                size_t passwordLength = strlen(studentInformation.studentPassword);
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

    if (isUsernameTaken(studentInformation.studentUsername)) {
        printf("\n\n\t\t\tUsername is already taken. Please choose a different username.\n");
        return;
    } else {
        FILE *fp = fopen("bdd/studentInfo.bin", "ab+");
        if (fp == NULL) {
            printf("\n\t\t\tError opening file!\n");
            return;
        }

        fwrite(&studentInformation, sizeof(studentInformation), 1, fp);
        fclose(fp);

        printf("\n\n\t\t\tInformation has been stored successfully\n");
        printf("\n\n\t\t\tEnter any keys to continue.......");
    }
}
