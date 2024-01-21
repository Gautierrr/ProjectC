#include "../main.h"

int viewAllDatabases(SDL_Renderer *renderer) {

    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "\n\n\t\t\tMySQL connection initialization error\n");
        return EXIT_FAILURE;
    }

        if (mysql_real_connect(conn, "localhost", "root", "root", NULL, 3306, NULL, 0))
    {

    // Requête pour récupérer la liste des bases de données
    const char *query = "SHOW DATABASES";
    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Error fetching databases: %s\n", mysql_error(conn));
        return 0;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Error fetching database result: %s\n", mysql_error(conn));
        return 0;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect textRect = {50, 50, 300, 30};
    SDL_Color textColor = {255, 255, 255};
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);

    SDL_Surface *textSurface;
    SDL_Texture *textTexture;

    SDL_Event event;
    int done = 0;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        char databaseName[255];
        strcpy(databaseName, row[0]);

        textSurface = TTF_RenderText_Solid(font, databaseName, textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        textRect.y += 40;  // Espacement entre chaque base de données
    }

    mysql_free_result(result);

    TTF_CloseFont(font);

    SDL_RenderPresent(renderer);

    while (!done) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    done = 1;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        done = 1;
                    }
                    break;
            }
        }
        SDL_Delay(10);
    }

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
