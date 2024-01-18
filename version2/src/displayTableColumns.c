#include "../main.h"

void displayTableColumns(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer) {
    char query[200];
    MYSQL_RES *result;
    MYSQL_ROW row;

    snprintf(query, sizeof(query), "DESCRIBE %s", tableName);

    if (mysql_query(conn, query) == 0) {
        result = mysql_store_result(conn);

        if (result != NULL) {
            SDL_Window *window;
            SDL_CreateWindowAndRenderer(1000, 700, 0, &window, &renderer);

            TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);
            SDL_Color textColor = { 255, 255, 255 };

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            SDL_Rect textRect = { 50, 60, 100, 30 };

            SDL_Surface *textSurface;
            SDL_Texture *textTexture;

            textSurface = TTF_RenderText_Solid(font, "Columns of Table", textColor);
            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            textRect.y += 40;

            textSurface = TTF_RenderText_Solid(font, "Column Name", textColor);
            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            textRect.x += 200;

            textSurface = TTF_RenderText_Solid(font, "Data Type", textColor);
            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            SDL_RenderPresent(renderer);

            textRect.y += 40;

            while ((row = mysql_fetch_row(result)) != NULL) {
                textRect.x = 50;

                textSurface = TTF_RenderText_Solid(font, row[0], textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

                textRect.x += 200;

                textSurface = TTF_RenderText_Solid(font, row[1], textColor);
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
            fprintf(stderr, "\n\n\t\t\tError retrieving columns: %s\n", mysql_error(conn));
        }
    } else {
        fprintf(stderr, "\n\n\t\t\tError describing table: %s\n", mysql_error(conn));
    }
}
