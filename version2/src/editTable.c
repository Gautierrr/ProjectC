#include "../main.h"

int tableExist(MYSQL *conn, const char *dbName, const char *tableName) {
    char query[512];
    snprintf(query, sizeof(query), "SHOW TABLES FROM %s LIKE '%s'", dbName, tableName);

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Error fetching table information: %s\n", mysql_error(conn));
        return 0;
    }

    mysql_free_result(result);

    return 1;
}

int editTable(MYSQL *conn, const char *dbName, SDL_Renderer *renderer, SDL_Renderer *renderer2) {

    SDL_Window *window = SDL_CreateWindow("Graphical Database Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_RESIZABLE);
    renderer2 = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
    SDL_RenderClear(renderer2);

    SDL_Rect tableNameRect = { 50, 200, 200, 30 };

    SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer2, &tableNameRect);

    SDL_Texture *option1Texture = IMG_LoadTexture(renderer2, "img/tableNameModify.png");
    SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer2, "img/background3.png");
    SDL_Rect option1Rect = {30, 125, 300, 100};

    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Color textColor = { 255, 255, 255 };
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);

    SDL_Event event;

    int done = 0;
    int isTypingTableName = 1;
    char tableName[100];
    char query[250];

    memset(tableName, 0, sizeof(tableName));

    SDL_RenderClear(renderer2);
    SDL_RenderCopy(renderer2, backgroundTexture, NULL, NULL);        
    SDL_RenderCopy(renderer2, option1Texture, NULL, &option1Rect);
    SDL_RenderPresent(renderer2);

    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                SDL_DestroyTexture(option1Texture);
                SDL_DestroyTexture(backgroundTexture);
                SDL_DestroyRenderer(renderer2);
                SDL_DestroyWindow(window);
                return 0;
            }  else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN) {
                    done = 1;
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
                    strcat(tableName, event.text.text);
                }

                SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer2, &tableNameRect);

                textSurface = TTF_RenderText_Solid(font, tableName, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer2, textTexture, NULL, &tableNameRect);

                SDL_RenderPresent(renderer2);
            }
        }

        SDL_Delay(10);
    }

    if (!tableExist(conn, dbName, tableName)) {
        printf("\n\n\t\t\tThe table '%s' does not exist.\n", tableName);
        SDL_DestroyTexture(option1Texture);
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyRenderer(renderer2);
        SDL_DestroyWindow(window);
        return 0;
    }

    SDL_DestroyTexture(option1Texture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer2);
    SDL_DestroyWindow(window);

    int modificationSuccessful = editTableMenu(conn, dbName, tableName, renderer);
    return 0;    
}
