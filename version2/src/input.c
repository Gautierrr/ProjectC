#include "../main.h"

void input(SDL_Renderer *renderer, char *inputText, int maxInputLength) {
    SDL_Surface *background = load_Pathed_Media("menu/MenuBack", 1);
    display_background(background);

    int done = 0;
    SDL_Event e;

    // Autres initialisations...

    FadeEffect(0, 0);

    while (!done) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT:
                    done = 1;
                case SDL_KEYDOWN:
                    if (strlen(inputText) == 0 && e.key.keysym.sym == SDLK_RETURN) {
                        // Si l'utilisateur appuie sur Enter sans saisie préalable, ne rien faire
                        continue;
                    }

                    if (e.key.keysym.sym == SDLK_RETURN) {
                        done = 1;
                    } else if (e.key.keysym.sym == SDLK_BACKSPACE) {
                        int size = strlen(inputText);
                        if (size > 0) {
                            inputText[size - 1] = '\0';
                        }
                    } else if (e.key.keysym.sym != SDLK_RETURN &&
                               e.key.keysym.sym != SDLK_BACKSPACE &&
                               strlen(inputText) < maxInputLength) {
                        // Si la longueur de la saisie est inférieure à la longueur maximale
                        // et que la touche pressée n'est pas Backspace ou Return
                        sprintf(inputText, "%s%s", inputText, SDL_GetKeyName(e.key.keysym.sym));
                    }

                    // Mises à jour d'affichage...

                    SDL_RenderPresent(renderer);
            }
        }
    }
}
