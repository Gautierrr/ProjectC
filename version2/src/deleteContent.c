#include "../main.h"

int deleteContent(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer2) {

    SDL_Window *window = SDL_CreateWindow("Graphical Database Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_RESIZABLE);
    renderer2 = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    char query[512];

    SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
    SDL_RenderClear(renderer2);
    
    SDL_Rect columnValueRect = {50, 200, 200, 30};

    SDL_Texture *option1Texture = IMG_LoadTexture(renderer2, "img/deleteContentP.png");
    SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer2, "img/background3.png");
    SDL_Rect option1Rect = {30, 75, 300, 100};

    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Color textColor = {255, 255, 255};
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Bold.ttf", 24);
    
    SDL_Event event;

    char rowId[255];
    int quit = 0;

    memset(rowId, 0, sizeof(rowId));

    SDL_RenderClear(renderer2);
    SDL_RenderCopy(renderer2, backgroundTexture, NULL, NULL);        
    SDL_RenderCopy(renderer2, option1Texture, NULL, &option1Rect);
    SDL_RenderPresent(renderer2);

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                SDL_DestroyTexture(option1Texture);
                SDL_DestroyTexture(backgroundTexture);
                SDL_DestroyRenderer(renderer2);
                SDL_DestroyWindow(window);
                return 0;
            } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN) {
                quit = 1;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    quit = 1;
                }
            } else if (event.type == SDL_TEXTINPUT) {
                strcat(rowId, event.text.text);
                SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer2, &columnValueRect);
                textSurface = TTF_RenderText_Solid(font, rowId, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer2, textTexture, NULL, &columnValueRect);
                SDL_RenderPresent(renderer2);
            }
        }
        SDL_Delay(10);
    }

    snprintf(query, sizeof(query), "DELETE FROM %s.%s WHERE id = %s", dbName, tableName, rowId);

    if (mysql_query(conn, query) != 0) {
        errorContent(renderer2);
        SDL_DestroyTexture(option1Texture);
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyRenderer(renderer2);
        SDL_DestroyWindow(window);
        return 1;
    }

    SDL_DestroyTexture(option1Texture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer2);
    SDL_DestroyWindow(window);

    return 0;
}
