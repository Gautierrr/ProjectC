#include "../main.h"

void renameTable(MYSQL *conn, const char *dbName, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect oldTableRect = { 50, 100, 200, 30 };
    SDL_Rect newTableRect = { 50, 350, 200, 30 };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &oldTableRect);
    SDL_RenderFillRect(renderer, &newTableRect);

    SDL_Rect textRect = { 50, 60, 300, 30 };

    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Color textColor = { 255, 255, 255 };
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);
    // SDL_Color color = {255, 255, 255, 255};
    textSurface = TTF_RenderText_Solid(font, "Enter the current name of the table : ", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    textRect.y = 300;

    textSurface = TTF_RenderText_Solid(font, "Enter the new name for the table : ", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_RenderPresent(renderer);

    SDL_Event event;

    int done = 0;
    int isTypingUsername = 1;
    char oldTableName[100];
    char newTableName[100];
    char query[250];

    // Initialiser les chaînes de caractères à zéro
    memset(oldTableName, 0, sizeof(oldTableName));
    memset(newTableName, 0, sizeof(newTableName));

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
                    strcat(oldTableName, event.text.text);
                } else {
                    strcat(newTableName, event.text.text);
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &oldTableRect);
                SDL_RenderFillRect(renderer, &newTableRect);

                textSurface = TTF_RenderText_Solid(font, oldTableName, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &oldTableRect);
                
                textSurface = TTF_RenderText_Solid(font, newTableName, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &newTableRect);

                SDL_RenderPresent(renderer);
            }
        }

        SDL_Delay(10);
    }
    
    snprintf(query, sizeof(query), "RENAME TABLE %s TO %s", oldTableName, newTableName);

    if (mysql_query(conn, query) == 0) {
        printf("\n\n\t\t\tTable '%s' renamed to '%s' successfully.\n", oldTableName, newTableName);
    } else {
        fprintf(stderr, "\n\n\t\t\tError renaming table: %s\n", mysql_error(conn));
    }

    printf("\n\n\t\t\tEnter any keys to continue.......");
}
