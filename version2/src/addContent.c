#include "../main.h"

int addContent(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer) {
    char query[512];
    MYSQL_RES *result;

    // Récupérer les noms des colonnes de la table
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

    // Variables pour stocker les noms de colonnes et les valeurs correspondantes
    char columnName[255];
    char columnValue[255];

    // Construire la partie de la requête avec les noms des colonnes
    char columnsPart[1024] = "";
    MYSQL_ROW row;
    while (row = mysql_fetch_row(result)) {
        // Exclure la colonne 'id' de la liste des colonnes
        if (strcmp(row[0], "id") != 0) {
            strcat(columnsPart, row[0]);
            strcat(columnsPart, ",");
        }
    }
    columnsPart[strlen(columnsPart) - 1] = '\0'; // Supprimer la virgule à la fin

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect columnValueRect = { 50, 200, 200, 30 };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &columnValueRect);

    SDL_Rect textRect = { 50, 100, 300, 30 };

    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Color textColor = { 255, 255, 255 };
    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);
    textSurface = TTF_RenderText_Solid(font, "Enter the values for each column (separated by commas and in single quotes) : ", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_RenderPresent(renderer);

    SDL_Event event;

    int done = 0;
    int isTypingUsername = 1;

    // Initialiser les chaînes de caractères à zéro
    memset(columnValue, 0, sizeof(columnValue));

    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = 1;
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

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &columnValueRect);

                textSurface = TTF_RenderText_Solid(font, columnValue, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &columnValueRect);

                SDL_RenderPresent(renderer);
            }
        }

        SDL_Delay(10);
    }

    // Demander les valeurs pour chaque colonne (sauf 'id')
    /*printf("Enter the values for each column (separated by commas) : ");
    if (fgets(columnValue, sizeof(columnValue), stdin) == NULL) {
        fprintf(stderr, "Error reading column values.\n");
        mysql_free_result(result);
        return 1;
    }*/
    columnValue[strcspn(columnValue, "\n")] = '\0'; // Supprimer le caractère de nouvelle ligne

    // Construire la requête SQL avec les noms de colonnes et les valeurs
    snprintf(query, sizeof(query), "INSERT INTO %s.%s (%s) VALUES (%s)", dbName, tableName, columnsPart, columnValue);

    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Error adding content: %s\n", mysql_error(conn));
        mysql_free_result(result);
        return 1;
    }

    mysql_free_result(result);
    printf("Content added successfully to the table %s.\n", tableName);

    return 0;
}