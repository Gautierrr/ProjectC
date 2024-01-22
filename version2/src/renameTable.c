#include "../main.h"

void renameTable(MYSQL *conn, const char *dbName, SDL_Renderer *renderer2) {

    SDL_Window *window = SDL_CreateWindow("Graphical Database Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_RESIZABLE);
    renderer2 = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
    SDL_RenderClear(renderer2);

    SDL_SetRenderDrawColor(renderer2, 255, 255, 255, 255);

    SDL_Rect oldTableRect = { 50, 150, 200, 30 };
    SDL_Rect newTableRect = { 50, 300, 200, 30 };

    SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer2, &oldTableRect);
    SDL_RenderFillRect(renderer2, &newTableRect);

    SDL_Texture *option1Texture = IMG_LoadTexture(renderer2, "img/currentNameTable.png");
    SDL_Texture *option2Texture = IMG_LoadTexture(renderer2, "img/newNameTable.png");
    SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer2, "img/background3.png");
    SDL_Rect option1Rect = {30, 75, 300, 100};
    SDL_Rect option2Rect = {30, 225, 300, 100};

    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Color textColor = { 255, 255, 255 };
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);

    // SDL_RenderPresent(renderer2);

    SDL_Event event;

    int done = 0;
    int isTypingUsername = 1;
    char oldTableName[100];
    char newTableName[100];
    char query[250];

    // Initialiser les chaînes de caractères à zéro
    memset(oldTableName, 0, sizeof(oldTableName));
    memset(newTableName, 0, sizeof(newTableName));

    SDL_RenderClear(renderer2);
    SDL_RenderCopy(renderer2, backgroundTexture, NULL, NULL);
    SDL_RenderCopy(renderer2, option1Texture, NULL, &option1Rect);
    SDL_RenderCopy(renderer2, option2Texture, NULL, &option2Rect);
    SDL_RenderPresent(renderer2);

    while (!done) {
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
                    if (isTypingUsername) {
                        isTypingUsername = 0;
                    } else {
                        done = 1;
                    }
                }
            } else if (event.type == SDL_TEXTINPUT) {
                if (isTypingUsername) {
                    strcat(oldTableName, event.text.text);
                } else {
                    strcat(newTableName, event.text.text);
                }

                SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer2, &oldTableRect);
                SDL_RenderFillRect(renderer2, &newTableRect);

                textSurface = TTF_RenderText_Solid(font, oldTableName, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer2, textTexture, NULL, &oldTableRect);
                
                textSurface = TTF_RenderText_Solid(font, newTableName, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer2, textTexture, NULL, &newTableRect);

                SDL_RenderPresent(renderer2);
            }
        }

        SDL_Delay(10);
    }
    
    snprintf(query, sizeof(query), "RENAME TABLE %s TO %s", oldTableName, newTableName);

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
