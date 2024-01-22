#include "../main.h"

int addColumn(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer2) {

    SDL_Window *window = SDL_CreateWindow("Graphical Database Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_RESIZABLE);
    renderer2 = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
    SDL_RenderClear(renderer2);

    SDL_Rect columnNameRect = { 50, 150, 200, 30 };
    SDL_Rect columnTypeRect = { 50, 300, 200, 30 };

    SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer2, &columnNameRect);
    SDL_RenderFillRect(renderer2, &columnTypeRect);

    SDL_Texture *option1Texture = IMG_LoadTexture(renderer2, "img/newNameC.png");
    SDL_Texture *option2Texture = IMG_LoadTexture(renderer2, "img/newTypeC.png");
    SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer2, "img/background3.png");
    SDL_Rect option1Rect = {30, 75, 300, 100};
    SDL_Rect option2Rect = {30, 225, 300, 100};

    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Color textColor = { 255, 255, 255 };
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);

    SDL_Event event;

    int done = 0;
    int isTypingTableName = 1;
    char columnName[100];
    char columnType[50];
    char query[250];

    memset(columnName, 0, sizeof(columnName));
    memset(columnType, 0, sizeof(columnType));

    SDL_RenderClear(renderer2);
    SDL_RenderCopy(renderer2, backgroundTexture, NULL, NULL);
    SDL_RenderCopy(renderer2, option1Texture, NULL, &option1Rect);
    SDL_RenderCopy(renderer2, option2Texture, NULL, &option2Rect);
    SDL_RenderPresent(renderer2);

    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                    SDL_DestroyTexture(option1Texture);
                    SDL_DestroyTexture(option2Texture);
                    SDL_DestroyTexture(backgroundTexture);
                    SDL_DestroyRenderer(renderer2);
                    SDL_DestroyWindow(window);
                    return 0;
                } else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        if (isTypingTableName) {
                            isTypingTableName = 0;
                        } else {
                            done = 1;
                        }
                    }
                } else if (event.type == SDL_TEXTINPUT) {
                if (isTypingTableName) {
                    strcat(columnName, event.text.text);
                } else {
                    strcat(columnType, event.text.text);
                }

                SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer2, &columnNameRect);
                SDL_RenderFillRect(renderer2, &columnTypeRect);

                textSurface = TTF_RenderText_Solid(font, columnName, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer2, textTexture, NULL, &columnNameRect);
                
                textSurface = TTF_RenderText_Solid(font, columnType, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer2, textTexture, NULL, &columnTypeRect);

                SDL_RenderPresent(renderer2);
            }
        }

        SDL_Delay(10);
    }
    
    snprintf(query, sizeof(query), "ALTER TABLE %s ADD COLUMN %s %s", tableName, columnName, columnType);

    if (mysql_query(conn, query) == 0) {
        printf("\n\n\t\t\tTable '%s' modified successfully.\n\n\n", tableName);
        SDL_DestroyTexture(option1Texture);
        SDL_DestroyTexture(option2Texture);
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyRenderer(renderer2);
        SDL_DestroyWindow(window);
        return 0;
    } else {
        fprintf(stderr, "\n\n\t\t\tError modifying table: %s\n", mysql_error(conn));
        SDL_DestroyTexture(option1Texture);
        SDL_DestroyTexture(option2Texture);
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyRenderer(renderer2);
        SDL_DestroyWindow(window);
        return 0;
    }

    printf("\n\n\t\t\tEnter any keys to continue.......");
}
