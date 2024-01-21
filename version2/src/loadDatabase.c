#include "../main.h"

int loadDatabase(SDL_Renderer *renderer, char *loggedInUsername) {
    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "\n\n\t\t\tMySQL connection initialization error\n");
        return EXIT_FAILURE;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect databaseRect = { 50, 200, 200, 30 };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &databaseRect);

    SDL_Rect textRect = { 50, 100, 300, 30 };

    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Color textColor = { 255, 255, 255 };
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);
    textSurface = TTF_RenderText_Solid(font, "Enter the name of the database to load : ", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);SDL_RenderPresent(renderer);

    SDL_Event event;

    int done = 0;
    int isTypingUsername = 1;
    char oldDbName[100];

    memset(oldDbName, 0, sizeof(oldDbName));

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
                strcat(oldDbName, event.text.text);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &databaseRect);

                textSurface = TTF_RenderText_Solid(font, oldDbName, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &databaseRect);

                SDL_RenderPresent(renderer);
            }
        }

        SDL_Delay(10);
    }

    oldDbName[strcspn(oldDbName, "\n")] = '\0';
    char dbName[100];

    // Concaténer le nom de l'utilisateur avec le nom de la base de données -> sous la forme -> nomBdd_username
    // Si admin on conserve juste ce qu'il a tapé

    if (strcmp(loggedInUsername, "admin") == 0) {
        strcpy(dbName, oldDbName);
    } else {
        snprintf(dbName, sizeof(dbName), "%s_%s", oldDbName, loggedInUsername);
    }

    if (mysql_real_connect(conn, "localhost", "root", "root", dbName, 3306, NULL, 0))
    {
        printf("\n\n\t\t\tConnection to database '%s' successful.\n", dbName);
        printf("\n\n\t\t\tEnter any keys to continue.......");

        databaseMenu(conn, renderer, dbName);

        mysql_close(conn);
        return 0;
    }
    else
    {
        fprintf(stderr, "\n\n\t\t\tDatabase connection failed : %s\n", mysql_error(conn));
        printf("\n\n\t\t\tEnter any keys to continue.......");
        mysql_close(conn);
        return 1;
    }
}
