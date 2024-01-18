#include "../main.h"

void deleteTable(MYSQL *conn, const char *dbName, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect tableRect = { 50, 200, 200, 30 };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &tableRect);

    SDL_Rect textRect = { 50, 100, 300, 30 };

    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Color textColor = { 255, 255, 255 };
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);
    textSurface = TTF_RenderText_Solid(font, "Enter the name of the table to delete : ", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_RenderPresent(renderer);

    SDL_Event event;

    int done = 0;
    int isTypingUsername = 1;
    char tableName[100];
    char query[150];

    // Initialiser les chaînes de caractères à zéro
    memset(tableName, 0, sizeof(tableName));

    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = 1;
            } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN) {
                done = 1;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    if (isTypingUsername) {
                        isTypingUsername = 0;
                    } else {
                        done = 1;
                    }
                }
            } else if (event.type == SDL_TEXTINPUT && isTypingUsername) {
                strcat(tableName, event.text.text);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &tableRect);

                textSurface = TTF_RenderText_Solid(font, tableName, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &tableRect);

                SDL_RenderPresent(renderer);
            }
        }

        SDL_Delay(10);
    }

    snprintf(query, sizeof(query), "DROP TABLE IF EXISTS %s", tableName);

    if (mysql_query(conn, query) == 0) {
        printf("\n\n\t\t\tTable '%s' deleted successfully.\n", tableName);
    } else {
        fprintf(stderr, "\n\n\t\t\tError deleting table: %s\n", mysql_error(conn));
    }

    printf("\n\n\t\t\tEnter any keys to continue.......");
}
