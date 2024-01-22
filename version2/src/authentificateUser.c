#include "../main.h"

int authentificateUser(int *connect, char *loggedInUsername, char *loggedInPassword, SDL_Renderer *renderer) {
    Student studentInformation;

    SDL_RenderClear(renderer);

    SDL_Rect usernameRect = { 590, 575, 125, 50 };
    SDL_Rect passwordRect = { 590, 775, 125, 50 };

    SDL_RenderFillRect(renderer, &usernameRect);
    SDL_RenderFillRect(renderer, &passwordRect);

    SDL_Texture *option1Texture = IMG_LoadTexture(renderer, "img/loginPage.png");
    SDL_Texture *option2Texture = IMG_LoadTexture(renderer, "img/username.png");
    SDL_Texture *option3Texture = IMG_LoadTexture(renderer, "img/password.png");
    SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer, "img/banniere.png");

    SDL_Rect option1Rect = {750, 300, 400, 125};
    SDL_Rect option2Rect = {575, 500, 250, 70};
    SDL_Rect option3Rect = {575, 700, 250, 70};

    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Color textColor = { 0, 0, 0 };
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);

    SDL_Event event;

    int done = 0;
    int isTypingUsername = 1;
    char username[50];
    char password[20];

    memset(username, 0, sizeof(username));
    memset(password, 0, sizeof(password));
    
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_RenderCopy(renderer, option1Texture, NULL, &option1Rect);
    SDL_RenderCopy(renderer, option2Texture, NULL, &option2Rect);
    SDL_RenderCopy(renderer, option3Texture, NULL, &option3Rect);

    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                SDL_DestroyTexture(backgroundTexture);
                SDL_DestroyTexture(option1Texture);
                SDL_DestroyTexture(option2Texture);
                SDL_DestroyTexture(option3Texture);
                *connect = 0;
                return *connect;
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

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
                SDL_RenderFillRect(renderer, &usernameRect);
                SDL_RenderFillRect(renderer, &passwordRect);

                textSurface = TTF_RenderText_Solid(font, username, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &usernameRect);

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

        SDL_RenderPresent(renderer);
    }

    FILE *fp = fopen("bdd/studentInfo.bin", "rb");

    if (fp == NULL) {
        errorAccount(renderer);
    }

    while (fread(&studentInformation, sizeof(studentInformation), 1, fp) == 1) {
        if (strcasecmp(username, studentInformation.studentUsername) == 0 && strcmp(password, studentInformation.studentPassword) == 0) {
            *connect = 1;

            strcpy(loggedInUsername, username);
            strcpy(loggedInPassword, password);
            
            fclose(fp);
            return *connect;
        }
    }
    
    errorAccount(renderer);
        
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(option1Texture);
    SDL_DestroyTexture(option2Texture);
    SDL_DestroyTexture(option3Texture);

    *connect = 0;
    fclose(fp);
    return *connect;
}
