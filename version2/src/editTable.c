#include "../main.h"

int editTable(MYSQL *conn, const char *dbName, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect tableNameRect = { 50, 100, 200, 30 };
    SDL_Rect columnNameRect = { 50, 250, 200, 30 };
    SDL_Rect columnTypeRect = { 50, 400, 200, 30 };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &tableNameRect);
    SDL_RenderFillRect(renderer, &columnNameRect);
    SDL_RenderFillRect(renderer, &columnTypeRect);

    SDL_Rect textRect = { 50, 60, 300, 30 };

    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Color textColor = { 255, 255, 255 };
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);
    // SDL_Color color = {255, 255, 255, 255};
    textSurface = TTF_RenderText_Solid(font, "Enter the name of the table to modify : ", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    /*textRect.y = 200;

    textSurface = TTF_RenderText_Solid(font, "Enter the name of the new column : ", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    textRect.y = 360;

    textSurface = TTF_RenderText_Solid(font, "Enter the data type of the new column : ", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);*/

    SDL_RenderPresent(renderer);

    SDL_Event event;

    int done = 0;
    int isTypingTableName = 1;
    // int isTypingColumnName = 0;
    // int isTypingTypeName = 0;
    char tableName[100];
    // char columnName[100];
    // char columnType[50];
    char query[250];

    // Initialiser les chaînes de caractères à zéro
    memset(tableName, 0, sizeof(tableName));
    // memset(columnName, 0, sizeof(columnName));
    // memset(columnType, 0, sizeof(columnType));

    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 0;
            }  else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN) {
                    done = 1;
                } else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        if (isTypingTableName) {
                            isTypingTableName = 0;
                        } else {
                            done = 1;
                        }
                    }
                } else if (event.type == SDL_TEXTINPUT) {
                if (isTypingTableName) {
                    strcat(tableName, event.text.text);
                } /*else if (isTypingColumnName) {
                    strcat(columnName, event.text.text);
                } else if (isTypingTypeName) {
                    strcat(columnType, event.text.text);
                }*/

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &tableNameRect);
                SDL_RenderFillRect(renderer, &columnNameRect);
                SDL_RenderFillRect(renderer, &columnTypeRect);

                textSurface = TTF_RenderText_Solid(font, tableName, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &tableNameRect);
                
                /*textSurface = TTF_RenderText_Solid(font, columnName, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &columnNameRect);
                
                textSurface = TTF_RenderText_Solid(font, columnType, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &columnTypeRect);*/

                SDL_RenderPresent(renderer);
            }
        }

        SDL_Delay(10);
    }

    int modificationSuccessful = editTableMenu(conn, dbName, tableName, renderer);
    if (modificationSuccessful) {
        return 0;
    }

    /*snprintf(query, sizeof(query), "ALTER TABLE %s ADD COLUMN %s %s", tableName, columnName, columnType);

    if (mysql_query(conn, query) == 0) {
        printf("\n\n\t\t\tTable '%s' modified successfully.\n\n\n", tableName);

        displayTableColumns(conn, dbName, tableName, renderer);
    } else {
        fprintf(stderr, "\n\n\t\t\tError modifying table: %s\n", mysql_error(conn));
    }*/

    printf("\n\n\t\t\tEnter any keys to continue.......");
}
