#include "../main.h"

int displayContent(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer) {
    char query[512];
    MYSQL_RES *result;

    snprintf(query, sizeof(query), "SELECT * FROM %s.%s", dbName, tableName);

    if (mysql_query(conn, query) != 0) {
        errorTable(renderer);
        return 1;
    }

    result = mysql_store_result(conn);

    if (result == NULL) {
        errorTable(renderer);
        return 1;
    }

    int numFields = mysql_num_fields(result);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Texture *option1Texture = IMG_LoadTexture(renderer, "img/allValues.png");
    SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer, "img/banniere.png");
    SDL_Rect option1Rect = {550, 200, 450, 150};

    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_RenderCopy(renderer, option1Texture, NULL, &option1Rect);
    SDL_RenderPresent(renderer);

    SDL_Rect textRect = {50, 340, 80, 30};
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Bold.ttf", 18);
    SDL_Color textColor = {0, 0, 0};

    MYSQL_FIELD *field;
    int currentX = 50;

    for (int i = 0; i < numFields; i++) {
        field = mysql_fetch_field(result);
        textRect.x = currentX;

        SDL_Surface *textSurface = TTF_RenderText_Solid(font, field->name, textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);

        currentX += 120;
    }

    int currentY = 380;
    int lineHeight = 20;
    int columnWidth = 80;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        currentX = 50;
        for (int i = 0; i < numFields; i++) {
            textRect.x = currentX;
            textRect.y = currentY;

            SDL_Surface *textSurface = TTF_RenderText_Solid(font, row[i] ? row[i] : "NULL", textColor);
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);

            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_DestroyTexture(textTexture);

            currentX += 120;
        }

        currentY += lineHeight + 20;

        if (currentY + lineHeight > 780) {
            currentY = 380;
            currentX += columnWidth + 50;
        }
    }

    SDL_RenderPresent(renderer);

    int quit = 0;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                quit = 1;
            }
        }
        SDL_Delay(10);
    }

    mysql_free_result(result);
    TTF_CloseFont(font);

    return 0;
}
