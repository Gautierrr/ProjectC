#include "../main.h"

const char *essentialDatabases[] = {"mysql", "information_schema", "performance_schema", "sys", "mabasededonnees"};

int isEssentialDatabase(const char *dbName) {
    for (size_t i = 0; i < sizeof(essentialDatabases) / sizeof(essentialDatabases[0]); ++i) {
        if (strcmp(dbName, essentialDatabases[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int deleteDatabase(SDL_Renderer *renderer) {

    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "\n\n\t\t\tMySQL connection initialization error\n");
        return EXIT_FAILURE;
    }

        if (mysql_real_connect(conn, "localhost", "root", "root", NULL, 3306, NULL, 0))
    {

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_Rect dbNameRect = { 50, 100, 200, 30 };
        SDL_Rect confirmationRect = { 50, 350, 40, 20 };

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &dbNameRect);
        SDL_RenderFillRect(renderer, &confirmationRect);

        SDL_Rect textRect = { 50, 60, 300, 30 };

        SDL_Surface *textSurface;
        SDL_Texture *textTexture;
        SDL_Color textColor = { 255, 255, 255 };
        TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);
        // SDL_Color color = {255, 255, 255, 255};
        textSurface = TTF_RenderText_Solid(font, "Enter the name of the database to delete : ", textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        textRect.y = 300;

        textSurface = TTF_RenderText_Solid(font, "Are you sure you want to delete this database? (y/n) : ", textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_RenderPresent(renderer);

        SDL_Event event;
        int done = 0;
        int isTypingDbName = 1;
        char dbName[50];
        char confirmation[2];

        memset(dbName, 0, sizeof(dbName));
        memset(confirmation, 0, sizeof(confirmation));

        while (!done) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    done = 1;
                } else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        if (isTypingDbName) {
                            isTypingDbName = 0;
                        } else {
                            done = 1;
                        }
                    }
                } else if (event.type == SDL_TEXTINPUT) {
                    if (isTypingDbName) {
                        strcat(dbName, event.text.text);
                    } else {
                        strcat(confirmation, event.text.text);
                    }

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &dbNameRect);
                    SDL_RenderFillRect(renderer, &confirmationRect);

                    textSurface = TTF_RenderText_Solid(font, dbName, textColor);
                    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                    SDL_FreeSurface(textSurface);
                    SDL_RenderCopy(renderer, textTexture, NULL, &dbNameRect);

                    textSurface = TTF_RenderText_Solid(font, confirmation, textColor);
                    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                    SDL_FreeSurface(textSurface);
                    SDL_RenderCopy(renderer, textTexture, NULL, &confirmationRect);

                    SDL_RenderPresent(renderer);
                }
            }

            SDL_Delay(10);
        }

        if (isEssentialDatabase(dbName)) {
            printf("Error: Cannot delete essential database '%s'.\n", dbName);
        } else if (strcmp(confirmation, "y") == 0 || strcmp(confirmation, "Y") == 0) {
            char query[512];
            snprintf(query, sizeof(query), "DROP DATABASE %s", dbName);

            if (mysql_query(conn, query) != 0) {
                fprintf(stderr, "Error deleting database: %s\n", mysql_error(conn));
                return 0;
            }

            printf("Database '%s' deleted successfully.\n", dbName);
        } else {
            printf("Deletion cancelled.\n");
        }

        TTF_CloseFont(font);

    mysql_close(conn);
    }
    else
    {
        fprintf(stderr, "\n\n\t\t\tDatabase connection failed : %s\n", mysql_error(conn));
        printf("\n\n\t\t\tEnter any keys to continue.......");
        mysql_close(conn);
        return 1;
    }
}
