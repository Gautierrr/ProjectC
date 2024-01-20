#include "../main.h"

void addPrimaryKey(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer) {
    if (mysql_select_db(conn, dbName) == 0) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_Rect keyRect = { 50, 200, 200, 30 };

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &keyRect);

        SDL_Rect textRect = { 50, 100, 300, 30 };

        SDL_Surface *textSurface;
        SDL_Texture *textTexture;
        SDL_Color textColor = { 255, 255, 255 };
        TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);
        textSurface = TTF_RenderText_Solid(font, "Enter the name of the column where to add a primary key : ", textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_RenderPresent(renderer);

        SDL_Event event;

        int done = 0;
        int isTyping = 1;
        char primaryKey[100];

        // Initialiser les chaînes de caractères à zéro
        memset(primaryKey, 0, sizeof(primaryKey));

        while (!done) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    done = 1;
                } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN) {
                    done = 1;
                } else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        if (isTyping) {
                            isTyping = 0;
                        } else {
                            done = 1;
                        }
                    }
                } else if (event.type == SDL_TEXTINPUT && isTyping) {
                    strcat(primaryKey, event.text.text);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &keyRect);

                    textSurface = TTF_RenderText_Solid(font, primaryKey, textColor);
                    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                    SDL_FreeSurface(textSurface);
                    SDL_RenderCopy(renderer, textTexture, NULL, &keyRect);

                    SDL_RenderPresent(renderer);
                }
            }

            SDL_Delay(10);
        } 
    
    
        char query[200];

        // Supprimer la clé primaire existante de la colonne 'id'
        snprintf(query, sizeof(query), "ALTER TABLE %s DROP PRIMARY KEY", tableName);
        if (mysql_query(conn, query) != 0) {
            fprintf(stderr, "Error deleting primary key : %s\n", mysql_error(conn));
            return;
        }

        // Ajouter une nouvelle clé primaire sur la colonne spécifiée par l'utilisateur
        snprintf(query, sizeof(query), "ALTER TABLE %s ADD PRIMARY KEY (%s)", tableName, primaryKey);
        if (mysql_query(conn, query) != 0) {
            fprintf(stderr, "Error adding new primary key : %s\n", mysql_error(conn));
            return;
        }

        printf("Primary key updated successfully.\n");

    
    } else {
        fprintf(stderr, "\n\t\t\tFailed to select database '%s': %s\n", dbName, mysql_error(conn));
    }
}