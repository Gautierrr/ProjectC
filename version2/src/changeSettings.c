#include "../main.h"

int isUsernameTaken2(const char *username, char *currentUsername) {
    FILE *fp = fopen("bdd/studentInfo.bin", "rb");
    if (fp == NULL) {
        printf("\n\t\t\tError opening file!\n");
        return 1;
    }

    Student existingUser;
    while (fread(&existingUser, sizeof(existingUser), 1, fp) == 1) {
        if (strcmp(username, existingUser.studentUsername) == 0 &&
            strcmp(username, currentUsername) != 0) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void changeSettings(char *currentUsername, char *currentPassword, SDL_Renderer *renderer) {

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
            strcmp(currentPassword, currentUser.studentPassword) == 0)
        {
            found = 1;
            break;
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect usernameRect = { 590, 575, 125, 50 };
    SDL_Rect passwordRect = { 590, 775, 125, 50 };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &usernameRect);
    SDL_RenderFillRect(renderer, &passwordRect);

    SDL_Texture *option1Texture = IMG_LoadTexture(renderer, "img/pageSettings.png");
    SDL_Texture *option2Texture = IMG_LoadTexture(renderer, "img/StudentU.png");
    SDL_Texture *option3Texture = IMG_LoadTexture(renderer, "img/studentP.png");
    SDL_Texture *option4Texture = IMG_LoadTexture(renderer, "img/adminP.png");
    SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer, "img/banniere.png");

    SDL_Rect option1Rect = {710, 270, 500, 150};
    SDL_Rect option2Rect = {575, 500, 250, 70};
    SDL_Rect option3Rect = {575, 700, 250, 70};
    SDL_Rect option4Rect = {575, 500, 250, 70};

    SDL_Rect textRect = {50, 100, 300, 30};

    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Color textColor = {0, 0, 0};
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Bold.ttf", 24);

    if (strcmp(currentUsername, "admin") == 0) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        SDL_RenderCopy(renderer, option1Texture, NULL, &option1Rect);
        SDL_RenderCopy(renderer, option4Texture, NULL, &option4Rect);
        SDL_Rect passwordRect = {500, 425, 200, 30};
    } else {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        SDL_RenderCopy(renderer, option1Texture, NULL, &option1Rect);
        SDL_RenderCopy(renderer, option2Texture, NULL, &option2Rect);
        SDL_RenderCopy(renderer, option3Texture, NULL, &option3Rect);
    
    }

    SDL_RenderPresent(renderer);

    SDL_Event event;

    int quit = 0;
    int isTypingUsername = 1;

    memset(currentUser.studentUsername, 0, sizeof(currentUser.studentUsername));
    memset(currentUser.studentPassword, 0, sizeof(currentUser.studentPassword));

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {                
                if (strcmp(currentUsername, "admin") == 0) {
                    SDL_DestroyTexture(backgroundTexture);
                    SDL_DestroyTexture(option1Texture);
                    SDL_DestroyTexture(option4Texture);
                    quit = 1;
                    return;
                } else {
                    SDL_DestroyTexture(backgroundTexture);
                    SDL_DestroyTexture(option1Texture);
                    SDL_DestroyTexture(option2Texture);
                    SDL_DestroyTexture(option3Texture);
                    quit = 1;
                    return;
                }
            } else if (event.type == SDL_KEYDOWN) {
                if (strcmp(currentUsername, "admin") == 0) { 
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        quit = 1;
                    }
                } else {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        if (isTypingUsername) {
                            isTypingUsername = 0;
                        } else {
                            quit = 1;
                        }
                    }
                }
            }
            else if (event.type == SDL_TEXTINPUT)
            {
                if (strcmp(currentUsername, "admin") == 0) { 
                    strcat(currentUser.studentPassword, event.text.text);               
                } else {
                    if (isTypingUsername) {
                        strcat(currentUser.studentUsername, event.text.text);
                    } else {
                        strcat(currentUser.studentPassword, event.text.text);
                    }
                }

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
                SDL_RenderFillRect(renderer, &usernameRect);
                SDL_RenderFillRect(renderer, &passwordRect);

                if (strcmp(currentUsername, "admin") != 0) {
                    textSurface = TTF_RenderText_Solid(font, currentUser.studentUsername, textColor);
                    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                    SDL_FreeSurface(textSurface);
                    SDL_RenderCopy(renderer, textTexture, NULL, &usernameRect);
                }

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
    
    if (strcmp(currentUsername, "admin") == 0) { 
        strcat(currentUser.studentUsername, currentUsername);
    }

    if (isUsernameTaken2(currentUser.studentUsername, currentUsername)) {
        errorAccount(renderer);
        return;
    } else {
        if (found) {

            fseek(fp, -((long)sizeof(currentUser)), SEEK_CUR);
            fwrite(&currentUser, sizeof(currentUser), 1, fp);
        } else {
            errorAccount(renderer);
        }

        fclose(fp);                
        if (strcmp(currentUsername, "admin") == 0) {
            SDL_DestroyTexture(backgroundTexture);
            SDL_DestroyTexture(option1Texture);
            SDL_DestroyTexture(option4Texture);
        } else {
            SDL_DestroyTexture(backgroundTexture);
            SDL_DestroyTexture(option1Texture);
            SDL_DestroyTexture(option2Texture);
            SDL_DestroyTexture(option3Texture);        
        }
    }
}
