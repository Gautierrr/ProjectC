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

int deleteDatabase(SDL_Renderer *renderer2) {

    SDL_Window *window = SDL_CreateWindow("Graphical Database Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_RESIZABLE);
    renderer2 = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "\n\n\t\t\tMySQL connection initialization error\n");
        return EXIT_FAILURE;
    }

        if (mysql_real_connect(conn, "localhost", "root", "root", NULL, 3306, NULL, 0))
    {

        SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
        SDL_RenderClear(renderer2);

        SDL_Rect dbNameRect = { 50, 175, 125, 30 };
        SDL_Rect confirmationRect = { 50, 325, 40, 20 };

        SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer2, &dbNameRect);
        SDL_RenderFillRect(renderer2, &confirmationRect);

        SDL_Texture *option1Texture = IMG_LoadTexture(renderer2, "img/deleteDatabaseP.png");
        SDL_Texture *option2Texture = IMG_LoadTexture(renderer2, "img/confirmation.png");
        SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer2, "img/background3.png");
        SDL_Rect option1Rect = {30, 75, 500, 125};
        SDL_Rect option2Rect = {30, 225, 500, 125};

        SDL_Surface *textSurface;
        SDL_Texture *textTexture;
        SDL_Color textColor = { 255, 255, 255 };
        TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);

        SDL_Event event;
        int done = 0;
        int isTypingDbName = 1;
        char dbName[50];
        char confirmation[2];

        memset(dbName, 0, sizeof(dbName));
        memset(confirmation, 0, sizeof(confirmation));

        SDL_RenderClear(renderer2);
        SDL_RenderCopy(renderer2, backgroundTexture, NULL, NULL);
        SDL_RenderCopy(renderer2, option1Texture, NULL, &option1Rect);
        SDL_RenderCopy(renderer2, option2Texture, NULL, &option2Rect);
        SDL_RenderPresent(renderer2);

        while (!done) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                    SDL_DestroyTexture(option1Texture);
                    SDL_DestroyTexture(option2Texture);
                    SDL_DestroyTexture(backgroundTexture);
                    SDL_DestroyRenderer(renderer2);
                    SDL_DestroyWindow(window);
                    done = 1;
                    return 0;
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

                    SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
                    SDL_RenderFillRect(renderer2, &dbNameRect);
                    SDL_RenderFillRect(renderer2, &confirmationRect);

                    textSurface = TTF_RenderText_Solid(font, dbName, textColor);
                    textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                    SDL_FreeSurface(textSurface);
                    SDL_RenderCopy(renderer2, textTexture, NULL, &dbNameRect);

                    textSurface = TTF_RenderText_Solid(font, confirmation, textColor);
                    textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                    SDL_FreeSurface(textSurface);
                    SDL_RenderCopy(renderer2, textTexture, NULL, &confirmationRect);

                    SDL_RenderPresent(renderer2);
                }
            }

            SDL_Delay(10);
        }

        if (isEssentialDatabase(dbName)) {
            printf("Error: Cannot delete essential database '%s'.\n", dbName);
            SDL_DestroyTexture(option1Texture);
            SDL_DestroyTexture(option2Texture);
            SDL_DestroyTexture(backgroundTexture);
            SDL_DestroyRenderer(renderer2);
            SDL_DestroyWindow(window);
            return 0;
        } else if (strcmp(confirmation, "y") == 0 || strcmp(confirmation, "Y") == 0) {
            char query[512];
            snprintf(query, sizeof(query), "DROP DATABASE %s", dbName);

            if (mysql_query(conn, query) != 0) {
                fprintf(stderr, "Error deleting database: %s\n", mysql_error(conn));
                SDL_DestroyTexture(option1Texture);
                SDL_DestroyTexture(option2Texture);
                SDL_DestroyTexture(backgroundTexture);
                SDL_DestroyRenderer(renderer2);
                SDL_DestroyWindow(window);
                return 0;
            }

            printf("Database '%s' deleted successfully.\n", dbName);
            SDL_DestroyTexture(option1Texture);
            SDL_DestroyTexture(option2Texture);
            SDL_DestroyTexture(backgroundTexture);
            SDL_DestroyRenderer(renderer2);
            SDL_DestroyWindow(window);
            return 0;
        } else {
            printf("Deletion cancelled.\n");
            SDL_DestroyTexture(option1Texture);
            SDL_DestroyTexture(option2Texture);
            SDL_DestroyTexture(backgroundTexture);
            SDL_DestroyRenderer(renderer2);
            SDL_DestroyWindow(window);
            return 0;
        }
    }
    else
    {
        fprintf(stderr, "\n\n\t\t\tDatabase connection failed : %s\n", mysql_error(conn));
        printf("\n\n\t\t\tEnter any keys to continue.......");
        mysql_close(conn);
        return 1;
    }
}
