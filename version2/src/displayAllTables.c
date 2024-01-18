#include "../main.h"

void displayAllTables(MYSQL *conn, const char *dbName, SDL_Renderer *renderer) {
    if (mysql_select_db(conn, dbName) == 0) {
        MYSQL_RES *result = mysql_list_tables(conn, NULL);

        if (result != NULL) {
            SDL_Window *window;
            SDL_CreateWindowAndRenderer(800, 600, 0, &window, &renderer);

            TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);
            SDL_Color textColor = { 255, 255, 255 };

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            SDL_Rect textRect = { 50, 60, 300, 30 };

            SDL_Surface *textSurface;
            SDL_Texture *textTexture;

            textSurface = TTF_RenderText_Solid(font, "All Tables in Database", textColor);
            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            SDL_RenderPresent(renderer);

            textRect.y += 40;

            MYSQL_ROW row;

            while ((row = mysql_fetch_row(result)) != NULL) {
                textSurface = TTF_RenderText_Solid(font, row[0], textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

                SDL_RenderPresent(renderer);

                textRect.y += 40;
            }

            int quit = 0;
            SDL_Event event;

            while (!quit) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                        quit = 1;
                    }
                }
                SDL_Delay(10);
            }

            SDL_DestroyTexture(textTexture);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);

            mysql_free_result(result);
        } else {
            fprintf(stderr, "\n\t\t\tFailed to retrieve tables: %s\n", mysql_error(conn));
        }
    } else {
        fprintf(stderr, "\n\t\t\tFailed to select database '%s': %s\n", dbName, mysql_error(conn));
    }
}
