#include "../main.h"

const char *essentialDatabases2[] = {"mysql", "information_schema", "performance_schema", "sys", "mabasededonnees"};

int isEssentialDatabase2(const char *dbName) {
    for (size_t i = 0; i < sizeof(essentialDatabases2) / sizeof(essentialDatabases2[0]); ++i) {
        if (strcmp(dbName, essentialDatabases2[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int loadDatabase(char *loggedInUsername, SDL_Renderer *renderer) {

    SDL_Window *window = SDL_CreateWindow("Graphical Database Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer2 = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderClear(renderer2);

    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "\n\n\t\t\tMySQL connection initialization error\n");
        return EXIT_FAILURE;
    }

    SDL_RenderClear(renderer2);

    SDL_Rect databaseRect = { 40, 200, 100, 25 };  // Adjusted position for a smaller window

    SDL_RenderFillRect(renderer2, &databaseRect);

    // SDL_Rect textRect = { 50, 325, 150, 25 };  // Adjusted position for a smaller window

    SDL_Texture *option1Texture = IMG_LoadTexture(renderer2, "img/loadDatabaseName.png");
    SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer2, "img/background3.png");
    SDL_Rect option1Rect = {30, 125, 200, 60};

    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Color textColor = { 255, 255, 255 };
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);

    SDL_Event event;

    int done = 0;
    int isTypingUsername = 1;
    char oldDbName[100];

    memset(oldDbName, 0, sizeof(oldDbName));

    SDL_RenderClear(renderer2);
    SDL_RenderCopy(renderer2, backgroundTexture, NULL, NULL);        
    SDL_RenderCopy(renderer2, option1Texture, NULL, &option1Rect);
    SDL_RenderPresent(renderer2);

    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                done = 1;
                SDL_DestroyTexture(option1Texture);
                SDL_DestroyTexture(backgroundTexture);
                SDL_DestroyRenderer(renderer2);
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

                SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer2, &databaseRect);

                textSurface = TTF_RenderText_Solid(font, oldDbName, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer2, textTexture, NULL, &databaseRect);

                SDL_RenderPresent(renderer2);
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

    if (isEssentialDatabase2(dbName)) {
        printf("Error: Cannot delete essential database '%s'.\n", dbName);
    } else {
        if (mysql_real_connect(conn, "localhost", "root", "root", dbName, 3306, NULL, 0))
        {
            printf("\n\n\t\t\tConnection to database '%s' successful.\n", dbName);
            printf("\n\n\t\t\tEnter any keys to continue.......");
            
            SDL_DestroyTexture(option1Texture);
            SDL_DestroyTexture(backgroundTexture);
            SDL_DestroyRenderer(renderer2);
            SDL_DestroyWindow(window);

            databaseMenu(conn, renderer, dbName, renderer2);

            mysql_close(conn);
            return 0;
        }
        else
        {
            fprintf(stderr, "\n\n\t\t\tDatabase connection failed : %s\n", mysql_error(conn));
            printf("\n\n\t\t\tEnter any keys to continue.......");
            
            SDL_DestroyTexture(option1Texture);
            SDL_DestroyTexture(backgroundTexture);
            SDL_DestroyRenderer(renderer2);
            SDL_DestroyWindow(window);
            mysql_close(conn);
            return 0;
        }
    }
}
