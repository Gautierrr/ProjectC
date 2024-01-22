#include "../main.h"

void exportDatabaseValidation(SDL_Renderer *renderer) {

    SDL_Window *window = SDL_CreateWindow("Graphical Database Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer2 = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderClear(renderer2);

    SDL_Texture *option1Texture = IMG_LoadTexture(renderer2, "img/exportDatabaseValidation.png");
    SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer2, "img/background3.png");
    SDL_Rect option1Rect = {50, 200, 500, 150};

    SDL_RenderClear(renderer2);
    SDL_RenderCopy(renderer2, backgroundTexture, NULL, NULL);
    SDL_RenderCopy(renderer2, option1Texture, NULL, &option1Rect);
    SDL_RenderPresent(renderer2);

    SDL_Event event;
    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                quit = 1;
                SDL_DestroyTexture(option1Texture);
                SDL_DestroyTexture(backgroundTexture);
                SDL_DestroyRenderer(renderer2);
                SDL_DestroyWindow(window);
                return;
            }
        }
        SDL_Delay(10);
    }
}