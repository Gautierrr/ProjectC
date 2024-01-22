#include "../main.h"

int viewAllDatabases(SDL_Renderer *renderer) {

    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL) {
        errorDatabase(renderer);
        return EXIT_FAILURE;
    }
        
    if (mysql_real_connect(conn, "localhost", "root", "root", NULL, 3306, NULL, 0)) {

        const char *query = "SHOW DATABASES";
        if (mysql_query(conn, query) != 0) {
            errorDatabase(renderer);
            return 0;
        }

        MYSQL_RES *result = mysql_store_result(conn);
        if (result == NULL) {
            errorDatabase(renderer);
            return 0;
        }

        SDL_RenderClear(renderer);

        SDL_Texture *option1Texture = IMG_LoadTexture(renderer, "img/allDatabases.png");
        SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer, "img/banniere.png");
        SDL_Rect option1Rect = {525, 200, 450, 150};

        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        SDL_RenderCopy(renderer, option1Texture, NULL, &option1Rect);
        SDL_RenderPresent(renderer);

        SDL_Rect textRect = {50, 400, 180, 30};
        SDL_Color textColor = {0, 0, 0};
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

            textRect.x += 250;

            if (textRect.x + textRect.w > 1500) {
                textRect.x = 50;
                textRect.y += 40;
            }
        }

        SDL_RenderPresent(renderer);        

        while (!done) {
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                            done = 1;
                            return 0;
                        break;
                    case SDL_KEYDOWN:
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                            done = 1;
                            return 0;
                        }
                        break;
                }
            }
            SDL_Delay(10);
        }

        mysql_close(conn);
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyTexture(option1Texture);
        SDL_DestroyRenderer(renderer);
        return 0;
    } else {
        errorDatabase(renderer);
        mysql_close(conn);
        return 1;
    }
}
