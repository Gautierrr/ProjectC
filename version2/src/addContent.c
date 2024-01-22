#include "../main.h"

int addContent(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer2) {

    SDL_Window *window = SDL_CreateWindow("Graphical Database Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_RESIZABLE);
    renderer2 = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    char query[512];
    MYSQL_RES *result;

    snprintf(query, sizeof(query), "DESCRIBE %s.%s", dbName, tableName);

    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Error describing table: %s\n", mysql_error(conn));
        return 1;
    }

    result = mysql_store_result(conn);

    if (result == NULL) {
        fprintf(stderr, "Error fetching table description: %s\n", mysql_error(conn));
        return 1;
    }

    char columnName[255];
    char columnValue[255];

    char columnsPart[1024] = "";
    MYSQL_ROW row;
    while (row = mysql_fetch_row(result)) {
        // Exclure la colonne 'id' de la liste des colonnes
        if (strcmp(row[0], "id") != 0) {
            strcat(columnsPart, row[0]);
            strcat(columnsPart, ",");
        }
    }
    columnsPart[strlen(columnsPart) - 1] = '\0'; // Supprimer la virgule à la fin si elle est mise

    SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
    SDL_RenderClear(renderer2);

    SDL_Rect columnValueRect = { 30, 200, 150, 30 };

    SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer2, &columnValueRect);

    SDL_Texture *option1Texture = IMG_LoadTexture(renderer2, "img/addContentP.png");
    SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer2, "img/background3.png");
    SDL_Rect option1Rect = {10, 75, 600, 200};

    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Color textColor = { 255, 255, 255 };
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);

    SDL_Event event;

    int done = 0;
    int isTypingUsername = 1;

    memset(columnValue, 0, sizeof(columnValue));

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
            } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN) {
                done = 1;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    if (isTypingUsername) {
                        isTypingUsername = 0;
                    } else {
                        done = 1;
                    }
                }
            } else if (event.type == SDL_TEXTINPUT && isTypingUsername) {
                strcat(columnValue, event.text.text);

                SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer2, &columnValueRect);

                textSurface = TTF_RenderText_Solid(font, columnValue, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer2, textTexture, NULL, &columnValueRect);

                SDL_RenderPresent(renderer2);
            }
        }

        SDL_Delay(10);
    }

    columnValue[strcspn(columnValue, "\n")] = '\0';

    snprintf(query, sizeof(query), "INSERT INTO %s.%s (%s) VALUES (%s)", dbName, tableName, columnsPart, columnValue);

    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Error adding content: %s\n", mysql_error(conn));
        mysql_free_result(result);
        SDL_DestroyTexture(option1Texture);
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyRenderer(renderer2);
        SDL_DestroyWindow(window);
        return 1;
    }

    mysql_free_result(result);
    printf("Content added successfully to the table %s.\n", tableName);
    SDL_DestroyTexture(option1Texture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer2);
    SDL_DestroyWindow(window);

    return 0;
}