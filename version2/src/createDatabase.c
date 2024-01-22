#include "../main.h"

int createDatabase(char *loggedInUsername) {

    SDL_Window *window = SDL_CreateWindow("Graphical Database Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderClear(renderer);

    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "\n\n\t\t\tMySQL connection initialization error\n");
        return 1;
    }

    if (mysql_real_connect(conn, "localhost", "root", "root", NULL, 3306, NULL, 0)) {
        SDL_RenderClear(renderer);

        SDL_Rect databaseRect = { 40, 200, 100, 25 };  // Adjusted position for a smaller window

        SDL_RenderFillRect(renderer, &databaseRect);

        // SDL_Rect textRect = { 50, 325, 150, 25 };  // Adjusted position for a smaller window

        SDL_Texture *option1Texture = IMG_LoadTexture(renderer, "img/createDatabaseName.png");
        SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer, "img/background3.png");
        SDL_Rect option1Rect = {30, 125, 200, 60};

        SDL_Surface *textSurface;
        SDL_Texture *textTexture;
        SDL_Color textColor = { 255, 255, 255 };
        TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);
        /*textSurface = TTF_RenderText_Solid(font, "Enter Database name : ", textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_RenderPresent(renderer);*/

        SDL_Event event;

        int done = 0;
        int isTypingUsername = 1;
        char oldDbName[100];

        // Initialiser les chaînes de caractères à zéro
        memset(oldDbName, 0, sizeof(oldDbName));

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);        
        SDL_RenderCopy(renderer, option1Texture, NULL, &option1Rect);
        SDL_RenderPresent(renderer);

        while (!done) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                    done = 1;
                    SDL_DestroyTexture(option1Texture);
                    SDL_DestroyTexture(backgroundTexture);
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    mysql_close(conn);
                    return 0;
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

        // Concaténer le nom de l'utilisateur avec le nom de la base de données -> sous la forme -> nomBdd_username
        char dbName[100];
        snprintf(dbName, sizeof(dbName), "%s_%s", oldDbName, loggedInUsername);

        char query[150];
        snprintf(query, sizeof(query), "CREATE DATABASE %s", dbName);

        if (mysql_query(conn, query) == 0) {
            printf("\n\n\t\t\tDatabase '%s' created successfully.\n", dbName);
            printf("\n\n\t\t\tEnter any keys to continue.......");
        } else {
            fprintf(stderr, "\n\n\t\t\tError creating database: %s\n", mysql_error(conn));
            printf("\n\n\t\t\tEnter any keys to continue.......");
        }

        // databaseMenu(conn, renderer, dbName);

        mysql_close(conn);
        SDL_DestroyTexture(option1Texture);
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return 0;

    } else {
        fprintf(stderr, "\n\n\t\t\tFailed to connect to MySQL server: %s\n", mysql_error(conn));
        printf("\n\n\t\t\tEnter any keys to continue.......");
        mysql_close(conn);
        return 1;
    }
}
