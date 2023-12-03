#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> // Ajout de la bibliothèque SDL_ttf pour le texte
#include <stdio.h>
#include <stdbool.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Fonction pour créer un bouton simple
SDL_Texture *createSimpleButton(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect *buttonRect)
{
    // Couleur du texte du bouton
    SDL_Color textColor = {0, 0, 0, 255};

    // Surface pour le texte
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);

    // Texture du texte
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    // Libération de la surface
    SDL_FreeSurface(textSurface);

    // Obtenir les dimensions de la surface pour positionner le bouton au centre
    SDL_QueryTexture(textTexture, NULL, NULL, &(buttonRect->w), &(buttonRect->h));
    buttonRect->x = (SCREEN_WIDTH - buttonRect->w) / 2;
    buttonRect->y = (SCREEN_HEIGHT - buttonRect->h) / 2;

    return textTexture;
}

int main(int argc, char *args[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL; // Ajout du renderer

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }

    // Create window
    window = SDL_CreateWindow("Nouvelle fenêtre SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Create a font
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24); // Remplacez le chemin par le chemin de votre police

    if (font == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Create a button
    SDL_Rect buttonRect;
    SDL_Texture *buttonTexture = createSimpleButton(renderer, font, "Salut salut !!", &buttonRect);

    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    quit = true;
                }
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Dessiner le bouton
        SDL_RenderCopy(renderer, buttonTexture, NULL, &buttonRect);

        // Mettre à jour l'affichage
        SDL_RenderPresent(renderer);
    }

    // Libérer les ressources
    SDL_DestroyTexture(buttonTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
