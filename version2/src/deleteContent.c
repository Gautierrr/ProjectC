#include "../main.h"

int deleteContent(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer) {
    char query[512];
    SDL_Rect textRect = {50, 100, 300, 30};
    SDL_Rect columnValueRect = {50, 200, 200, 30};
    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Color textColor = {255, 255, 255};
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);

    // Demander à l'utilisateur d'entrer l'ID de la ligne à supprimer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    textSurface = TTF_RenderText_Solid(font, "Enter the ID of the row you want to delete: ", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_RenderPresent(renderer);

    char rowId[255];
    memset(rowId, 0, sizeof(rowId));
    int done = 0;
    SDL_Event event;

    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = 1;
            } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN) {
                done = 1;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    done = 1;
                }
            } else if (event.type == SDL_TEXTINPUT) {
                strcat(rowId, event.text.text);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &columnValueRect);
                textSurface = TTF_RenderText_Solid(font, rowId, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &columnValueRect);
                SDL_RenderPresent(renderer);
            }
        }
        SDL_Delay(10);
    }

    // Construire la requête SQL pour supprimer la ligne avec l'ID fourni
    snprintf(query, sizeof(query), "DELETE FROM %s.%s WHERE id = %s", dbName, tableName, rowId);

    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Error deleting content: %s\n", mysql_error(conn));
        return 1;
    }

    printf("Content deleted successfully from the table %s.\n", tableName);

    return 0;
}
