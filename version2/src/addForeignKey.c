#include "../main.h"

void addForeignKey(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect sourceColumnNameRect = { 50, 100, 200, 30 };
    SDL_Rect targetTableNameRect = { 50, 250, 200, 30 };
    SDL_Rect targetColumnNameRect = { 50, 400, 200, 30 };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &sourceColumnNameRect);
    SDL_RenderFillRect(renderer, &targetTableNameRect);
    SDL_RenderFillRect(renderer, &targetColumnNameRect);

    SDL_Rect textRect = { 50, 60, 300, 30 };

    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Color textColor = { 255, 255, 255 };
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);
    // SDL_Color color = {255, 255, 255, 255};
    textSurface = TTF_RenderText_Solid(font, "Enter the name of the source column for the foreign key : ", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    textRect.y = 200;

    textSurface = TTF_RenderText_Solid(font, "Enter the name of the target table for the foreign key : ", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    textRect.y = 360;

    textSurface = TTF_RenderText_Solid(font, "Enter the name of the target column for the foreign key : ", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_RenderPresent(renderer);

    SDL_Event event;

    int done = 0;
    int isTypingTableName = 1;
    int isTypingColumnName = 0;
    int isTypingTypeName = 0;
    char sourceColumnName[100];
    char targetTableName[100];
    char targetColumnName[100];
    char query[250];

    // Initialiser les chaînes de caractères à zéro
    memset(sourceColumnName, 0, sizeof(sourceColumnName));
    memset(targetTableName, 0, sizeof(targetTableName));
    memset(targetColumnName, 0, sizeof(targetColumnName));

    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = 1;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                if (isTypingTableName) {
                    isTypingTableName = 0;
                    isTypingColumnName = 1;
                } else if (isTypingColumnName) {
                    isTypingColumnName = 0;
                    isTypingTypeName = 1;
                } else if (isTypingTypeName) {
                    isTypingTypeName = 1;
                    done = 1;
                }
                }
            } else if (event.type == SDL_TEXTINPUT) {
                if (isTypingTableName) {
                    strcat(sourceColumnName, event.text.text);
                } else if (isTypingColumnName) {
                    strcat(targetTableName, event.text.text);
                } else if (isTypingTypeName) {
                    strcat(targetColumnName, event.text.text);
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &sourceColumnNameRect);
                SDL_RenderFillRect(renderer, &targetTableNameRect);
                SDL_RenderFillRect(renderer, &targetColumnNameRect);

                textSurface = TTF_RenderText_Solid(font, sourceColumnName, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &sourceColumnNameRect);
                
                textSurface = TTF_RenderText_Solid(font, targetTableName, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &targetTableNameRect);
                
                textSurface = TTF_RenderText_Solid(font, targetColumnName, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &targetColumnNameRect);

                SDL_RenderPresent(renderer);
            }
        }

        SDL_Delay(10);
    }





















/*
    // Saisie des informations de la clé étrangère par l'utilisateur
    printf("Enter the name of the source column for the foreign key : ");
    scanf("%s", sourceColumnName);

    printf("Enter the name of the target table for the foreign key : ");
    scanf("%s", targetTableName);

    printf("Enter the name of the target column for the foreign key : ");
    scanf("%s", targetColumnName);*/

    // Construction de la requête SQL pour ajouter la clé étrangère
    //char query[200];
    snprintf(query, sizeof(query), "ALTER TABLE %s ADD FOREIGN KEY (%s) REFERENCES %s(%s)",
             tableName, sourceColumnName, targetTableName, targetColumnName);

    // Exécution de la requête
    if (mysql_query(conn, query) == 0) {
        printf("Foreign key added successfully.\n");
    } else {
        fprintf(stderr, "Error adding foreign key: %s\n", mysql_error(conn));
    }

    // Vous pouvez ajouter ici le code pour revenir à la page précédente (le MCD par exemple)
}
