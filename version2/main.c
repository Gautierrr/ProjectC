#include "main.h"

int SDL_main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Graphical Database Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1500, 780, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);    
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "SDL_ttf initialization failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }    

    SDL_RenderClear(renderer);

    SDL_Texture *option1Texture = IMG_LoadTexture(renderer, "img/signUp.png");
    SDL_Texture *option2Texture = IMG_LoadTexture(renderer, "img/logIn.png");
    SDL_Texture *option3Texture = IMG_LoadTexture(renderer, "img/accessDatabaseMenu.png");
    SDL_Texture *option4Texture = IMG_LoadTexture(renderer, "img/logoCbv.png");
    SDL_Texture *option0Texture = IMG_LoadTexture(renderer, "img/exit.png");
    SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer, "img/background1.jpg");

    SDL_Rect option1Rect = {40, 30, 150, 50};
    SDL_Rect option2Rect = {250, 30, 150, 50};
    SDL_Rect option3Rect = {600, 7, 650, 95};
    SDL_Rect option4Rect = {80, 500, 350, 350};
    SDL_Rect option0Rect = {1700, 925, 150, 50};


    char option;
    int connect = 0;
    char loggedInUsername[50];
    char loggedInPassword[20];
    int result = 0;

    while (option != '0') {
        SDL_Event event;
        SDL_PollEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                option = '0';
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    if (mouseX > option1Rect.x && mouseX < option1Rect.x + option1Rect.w &&
                        mouseY > option1Rect.y && mouseY < option1Rect.y + option1Rect.h) {
                        option = '1';
                        createAccount(renderer);
                    } else if (mouseX > option2Rect.x && mouseX < option2Rect.x + option2Rect.w &&
                               mouseY > option2Rect.y && mouseY < option2Rect.y + option2Rect.h) {
                        option = '2';
                        result = authentificateUser(&connect, loggedInUsername, loggedInPassword, renderer);
                    } else if (mouseX > option3Rect.x && mouseX < option3Rect.x + option3Rect.w &&
                               mouseY > option3Rect.y && mouseY < option3Rect.y + option3Rect.h) {
                        option = '3';
                        if (result == 1) {
                            if (strcmp(loggedInUsername, "admin") == 0) {
                                adminMenu(loggedInUsername, loggedInPassword, renderer);
                            } else {
                                mainMenu(loggedInUsername, loggedInPassword, renderer);
                            }
                        }
                        result = 0;
                    } else if (mouseX > option4Rect.x && mouseX < option4Rect.x + option4Rect.w &&
                               mouseY > option4Rect.y && mouseY < option4Rect.y + option4Rect.h) {
                        option = '4';
                        downloadImg(renderer);
                    } else if (mouseX > option0Rect.x && mouseX < option0Rect.x + option0Rect.w &&
                               mouseY > option0Rect.y && mouseY < option0Rect.y + option0Rect.h) {
                        option = '0';
                    }
                }
                break;
        }

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        SDL_RenderCopy(renderer, option1Texture, NULL, &option1Rect);
        SDL_RenderCopy(renderer, option2Texture, NULL, &option2Rect);
        SDL_RenderCopy(renderer, option3Texture, NULL, &option3Rect);
        SDL_RenderCopy(renderer, option4Texture, NULL, &option4Rect);
        SDL_RenderCopy(renderer, option0Texture, NULL, &option0Rect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(option1Texture);
    SDL_DestroyTexture(option2Texture);
    SDL_DestroyTexture(option3Texture);
    SDL_DestroyTexture(option4Texture);
    SDL_DestroyTexture(option0Texture);


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
