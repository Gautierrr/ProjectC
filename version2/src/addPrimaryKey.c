#include "../main.h"

void addPrimaryKey(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer2) {

    SDL_Window *window = SDL_CreateWindow("Graphical Database Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_RESIZABLE);
    renderer2 = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (mysql_select_db(conn, dbName) == 0) {
        SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
        SDL_RenderClear(renderer2);

        SDL_Rect keyRect = { 50, 200, 200, 30 };

        SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer2, &keyRect);

        SDL_Texture *option1Texture = IMG_LoadTexture(renderer2, "img/primaryKeyP.png");
        SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer2, "img/background3.png");
        SDL_Rect option1Rect = {30, 125, 300, 100};

        SDL_Surface *textSurface;
        SDL_Texture *textTexture;
        SDL_Color textColor = { 255, 255, 255 };
        TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Bold.ttf", 24);

        SDL_Event event;

        int quit = 0;
        int isTyping = 1;
        char primaryKey[100];

        memset(primaryKey, 0, sizeof(primaryKey));

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
                    return;
                } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN) {
                    quit = 1;
                } else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        if (isTyping) {
                            isTyping = 0;
                        } else {
                            quit = 1;
                        }
                    }
                } else if (event.type == SDL_TEXTINPUT && isTyping) {
                    strcat(primaryKey, event.text.text);

                    SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
                    SDL_RenderFillRect(renderer2, &keyRect);

                    textSurface = TTF_RenderText_Solid(font, primaryKey, textColor);
                    textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                    SDL_FreeSurface(textSurface);
                    SDL_RenderCopy(renderer2, textTexture, NULL, &keyRect);

                    SDL_RenderPresent(renderer2);
                }
            }

            SDL_Delay(10);
        } 
    
    
        char query[200];

        // Supprimer la clé primaire existante de la colonne 'id'
        snprintf(query, sizeof(query), "ALTER TABLE %s DROP PRIMARY KEY", tableName);
        if (mysql_query(conn, query) != 0) {
            fprintf(stderr, "Error deleting primary key : %s\n", mysql_error(conn));
            return;
        }

        // Ajouter une nouvelle clé primaire sur la colonne spécifiée par l'utilisateur
        snprintf(query, sizeof(query), "ALTER TABLE %s ADD PRIMARY KEY (%s)", tableName, primaryKey);
        if (mysql_query(conn, query) != 0) {
            fprintf(stderr, "Error adding new primary key : %s\n", mysql_error(conn));
            return;
        }

        printf("Primary key updated successfully.\n");
        SDL_DestroyTexture(option1Texture);
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyRenderer(renderer2);
        SDL_DestroyWindow(window);
        return;

    
    } else {
        fprintf(stderr, "\n\t\t\tFailed to select database '%s': %s\n", dbName, mysql_error(conn));
    }
}