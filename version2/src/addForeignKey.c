#include "../main.h"

void addForeignKey(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer2) {

    SDL_Window *window = SDL_CreateWindow("Graphical Database Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_RESIZABLE);
    renderer2 = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
    SDL_RenderClear(renderer2);

    SDL_Rect sourceColumnNameRect = { 50, 100, 200, 30 };
    SDL_Rect targetTableNameRect = { 50, 250, 200, 30 };

    SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer2, &sourceColumnNameRect);
    SDL_RenderFillRect(renderer2, &targetTableNameRect);

    SDL_Texture *option1Texture = IMG_LoadTexture(renderer2, "img/nameSourceColumn.png");
    SDL_Texture *option2Texture = IMG_LoadTexture(renderer2, "img/nameTargetColumn.png");
    SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer2, "img/background3.png");
    SDL_Rect option1Rect = {30, 75, 300, 100};
    SDL_Rect option2Rect = {30, 225, 300, 100};

    SDL_Rect textRect = { 50, 60, 300, 30 };

    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Color textColor = { 255, 255, 255 };
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Bold.ttf", 24);

    SDL_Event event;

    int quit = 0;
    int isTypingTableName = 1;
    char sourceColumnName[100];
    char targetTableName[100];
    char query[250];

    memset(sourceColumnName, 0, sizeof(sourceColumnName));
    memset(targetTableName, 0, sizeof(targetTableName));

    SDL_RenderClear(renderer2);
    SDL_RenderCopy(renderer2, backgroundTexture, NULL, NULL);
    SDL_RenderCopy(renderer2, option1Texture, NULL, &option1Rect);
    SDL_RenderCopy(renderer2, option2Texture, NULL, &option2Rect);
    SDL_RenderPresent(renderer2);

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                SDL_DestroyTexture(option1Texture);
                SDL_DestroyTexture(option2Texture);
                SDL_DestroyTexture(backgroundTexture);
                SDL_DestroyRenderer(renderer2);
                SDL_DestroyWindow(window);
                return;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    if (isTypingTableName) {
                        isTypingTableName = 0;
                    } else {
                        quit = 1;
                    }
                }
            } else if (event.type == SDL_TEXTINPUT) {
                if (isTypingTableName) {
                    strcat(sourceColumnName, event.text.text);
                } else {
                    strcat(targetTableName, event.text.text);
                }

                SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer2, &sourceColumnNameRect);
                SDL_RenderFillRect(renderer2, &targetTableNameRect);

                textSurface = TTF_RenderText_Solid(font, sourceColumnName, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer2, textTexture, NULL, &sourceColumnNameRect);
                
                textSurface = TTF_RenderText_Solid(font, targetTableName, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer2, textTexture, NULL, &targetTableNameRect);

                SDL_RenderPresent(renderer2);
            }
        }

        SDL_Delay(10);
    }

    snprintf(query, sizeof(query), "ALTER TABLE %s ADD FOREIGN KEY (%s) REFERENCES %s(id)", tableName, sourceColumnName, targetTableName);

    if (mysql_query(conn, query) == 0) {
        SDL_DestroyTexture(option1Texture);
        SDL_DestroyTexture(option2Texture);
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyRenderer(renderer2);
        SDL_DestroyWindow(window);
        return;
    } else {
        errorTable(renderer2);
        SDL_DestroyTexture(option1Texture);
        SDL_DestroyTexture(option2Texture);
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyRenderer(renderer2);
        SDL_DestroyWindow(window);
        return;
    }
}
