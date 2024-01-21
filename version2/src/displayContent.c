#include "../main.h"

int displayContent(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer) {
    char query[512];
    MYSQL_RES *result;

    // Construire la requête SQL pour afficher toutes les colonnes et valeurs
    snprintf(query, sizeof(query), "SELECT * FROM %s.%s", dbName, tableName);

    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Error querying database: %s\n", mysql_error(conn));
        return 1;
    }

    // Récupérer les résultats de la requête
    result = mysql_store_result(conn);

    if (result == NULL) {
        fprintf(stderr, "Error fetching table content: %s\n", mysql_error(conn));
        return 1;
    }

    // Récupérer le nombre de colonnes
    int numFields = mysql_num_fields(result);

    // Afficher les noms des colonnes dans la fenêtre graphique
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect textRect = {50, 60, 150, 30};  // Ajustez la largeur de la zone de texte pour les noms des colonnes
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 18);
    SDL_Color textColor = {255, 255, 255};

    MYSQL_FIELD *field;
    int currentX = 50;

    for (int i = 0; i < numFields; i++) {
        field = mysql_fetch_field(result);
        textRect.x = currentX;

        SDL_Surface *textSurface = TTF_RenderText_Solid(font, field->name, textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);

        currentX += 200;  // Ajustez la valeur pour l'espacement entre les colonnes
    }

    // Afficher les valeurs de chaque ligne dans la fenêtre graphique
    int currentY = 90;  // Ajustez la valeur pour l'espacement vertical entre les lignes

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        currentX = 50;

        for (int i = 0; i < numFields; i++) {
            textRect.x = currentX;
            textRect.y = currentY;

            SDL_Surface *textSurface = TTF_RenderText_Solid(font, row[i] ? row[i] : "NULL", textColor);
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);

            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_DestroyTexture(textTexture);

            currentX += 200;  // Ajustez la valeur pour l'espacement entre les colonnes
        }

        currentY += 30;  // Ajustez la valeur pour l'espacement vertical entre les lignes
    }

    SDL_RenderPresent(renderer);

    // Attendre que l'utilisateur ferme la fenêtre
    int quit = 0;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                quit = 1;
            }
        }
        SDL_Delay(10);
    }

    // Libérer la mémoire
    mysql_free_result(result);
    TTF_CloseFont(font);

    return 0;
}
