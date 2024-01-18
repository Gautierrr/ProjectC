#include "../main.h"

int authentificateUser(int *connect, char *loggedInUsername, char *loggedInPassword, SDL_Renderer *renderer) {
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
    char username[50];
    char password[20];

    // Initialiser les chaînes de caractères à zéro
    memset(username, 0, sizeof(username));
    memset(password, 0, sizeof(password));

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
                    strcat(username, event.text.text);
                } else {
                    strcat(password, event.text.text);
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &usernameRect);
                SDL_RenderFillRect(renderer, &passwordRect);

                textSurface = TTF_RenderText_Solid(font, username, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &usernameRect);

                // Afficher des étoiles pour le mot de passe
                size_t passwordLength = strlen(password);
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

    FILE *fp = fopen("bdd/studentInfo.bin", "rb");

    if (fp == NULL)
    {
        printf("\n\t\t\tError !\n");
    }

    while (fread(&studentInformation, sizeof(studentInformation), 1, fp) == 1) {

        printf("\n\t\t\tRead username: %s\n", studentInformation.studentUsername);
        printf("\t\t\tRead password: %s\n", studentInformation.studentPassword);

        if (strcasecmp(username, studentInformation.studentUsername) == 0 && strcmp(password, studentInformation.studentPassword) == 0) {
            *connect = 1;
            printf("\n\n\t\t\tLogin Successful!\n");

            strcpy(loggedInUsername, username);
            strcpy(loggedInPassword, password);

            printf("\n\t\t\ttkt: %s\n", loggedInUsername);
            printf("\t\t\ttkt: %s\n", loggedInPassword);

            fclose(fp);
            return *connect;
        }
    }
    printf("\n\n\t\t\tLogin failed. Please try again.\n");
    printf("\n\n\t\t\tEnter any keys to continue.......");

    *connect = 0;
    fclose(fp);
    return *connect;
}
