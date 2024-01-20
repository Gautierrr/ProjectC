#include "../main.h"

int viewMcd(MYSQL *conn, const char *dbName, SDL_Renderer *renderer) {
    if (mysql_select_db(conn, dbName) == 0) {
        char query[200];
        snprintf(query, sizeof(query),
                 "SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = '%s'", dbName);

        if (mysql_query(conn, query) == 0) {
            MYSQL_RES *result = mysql_store_result(conn);

            if (result != NULL) {
                Table tables[50];
                int numTables = 0;

                Link links[50];
                int numLinks = 0;

                ForeignKey foreignKeys[50];
                int numForeignKeys = 0;

                TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 18);
                SDL_Color textColor = {255, 255, 255};

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                SDL_Rect textRect = {50, 60, 300, 30};
                SDL_Surface *textSurface;
                SDL_Texture *textTexture;
                MYSQL_ROW row;

                int screenWidth = 800;
                int tableWidth = 200;
                int tableHeight = 40;
                int margin = 40;

                int currentX = margin;
                int currentY = margin;
                int maxTableHeight = 0;

                while (row = mysql_fetch_row(result)) {
                    strcpy(tables[numTables].name, row[0]);
                    tables[numTables].x = currentX;
                    tables[numTables].y = currentY;

                    int currentTableHeight = tables[numTables].y + tableHeight + margin;
                    if (currentTableHeight > maxTableHeight) {
                        maxTableHeight = currentTableHeight;
                    }

                    SDL_Rect tableRect = {tables[numTables].x, tables[numTables].y, tableWidth, tableHeight};
                    SDL_RenderDrawRect(renderer, &tableRect);
                    SDL_RenderFillRect(renderer, &tableRect);

                    textSurface = TTF_RenderText_Solid(font, tables[numTables].name, textColor);
                    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                    SDL_FreeSurface(textSurface);

                    textRect.x = tables[numTables].x + 10;
                    textRect.y = tables[numTables].y + 10;

                    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
                    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                    SDL_DestroyTexture(textTexture);

                    snprintf(query, sizeof(query),
                             "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = '%s' AND TABLE_NAME = '%s'", dbName, tables[numTables].name);

                    if (mysql_query(conn, query) == 0) {
                        MYSQL_RES *columnResult = mysql_store_result(conn);

                        if (columnResult != NULL) {
                            int currentColumnY = maxTableHeight + 10;  // Aligner verticalement en dessous de la hauteur maximale
                            int currentColumnX = tables[numTables].x;

                            int numColumns = mysql_num_rows(columnResult);

                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                            SDL_Rect entityRect = {tables[numTables].x, tables[numTables].y, tableWidth, tableHeight + numColumns * 30 + 50};
                            SDL_RenderDrawRect(renderer, &entityRect);
                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

                            MYSQL_ROW columnRow;

                            while (columnRow = mysql_fetch_row(columnResult)) {
                                textSurface = TTF_RenderText_Solid(font, columnRow[0], textColor);
                                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                                SDL_FreeSurface(textSurface);

                                textRect.x = currentColumnX + 10;
                                textRect.y = currentColumnY;

                                SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
                                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                                SDL_DestroyTexture(textTexture);

                                // Pour récupérer le type de la colonne
                                snprintf(query, sizeof(query),
                                        "SELECT DATA_TYPE FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = '%s' AND TABLE_NAME = '%s' AND COLUMN_NAME = '%s'",
                                        dbName, tables[numTables].name, columnRow[0]);

                                if (mysql_query(conn, query) == 0) {
                                    MYSQL_RES *dataTypeResult = mysql_store_result(conn);

                                    if (dataTypeResult != NULL) {
                                        MYSQL_ROW dataTypeRow = mysql_fetch_row(dataTypeResult);

                                        if (dataTypeRow != NULL) {
                                            textSurface = TTF_RenderText_Solid(font, dataTypeRow[0], textColor);
                                            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                                            SDL_FreeSurface(textSurface);

                                            textRect.x = currentColumnX + 10 + textRect.w + 5;
                                            textRect.y = currentColumnY;

                                            SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
                                            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                                            SDL_DestroyTexture(textTexture);
                                        }

                                        mysql_free_result(dataTypeResult);
                                    }
                                }

                                currentColumnY += 30;
                            }

                            mysql_free_result(columnResult);
                        }
                    }

                    currentX += tableWidth + margin;

                    if (currentX + tableWidth + margin > screenWidth) {
                        currentX = margin;
                        currentY += maxTableHeight + tableHeight + margin * 2;
                        maxTableHeight;
                    }

                    numTables++;
                }

                snprintf(query, sizeof(query),
                         "SELECT TABLE_NAME, COLUMN_NAME, REFERENCED_TABLE_NAME, REFERENCED_COLUMN_NAME FROM INFORMATION_SCHEMA.KEY_COLUMN_USAGE WHERE TABLE_SCHEMA = '%s' AND REFERENCED_TABLE_NAME IS NOT NULL", dbName);

                if (mysql_query(conn, query) == 0) {
                    MYSQL_RES *fkResult = mysql_store_result(conn);

                    if (fkResult != NULL) {
                        while (row = mysql_fetch_row(fkResult)) {
                            strcpy(foreignKeys[numForeignKeys].sourceTable, row[0]);
                            strcpy(foreignKeys[numForeignKeys].sourceColumn, row[1]);
                            strcpy(foreignKeys[numForeignKeys].targetTable, row[2]);
                            strcpy(foreignKeys[numForeignKeys].targetColumn, row[3]);

                            numForeignKeys++;
                        }

                        mysql_free_result(fkResult);
                    }
                }

                for (int i = 0; i < numForeignKeys; i++) {
                    for (int j = 0; j < numTables; j++) {
                        if (strcmp(tables[j].name, foreignKeys[i].sourceTable) == 0) {
                            for (int k = 0; k < numTables; k++) {
                                if (strcmp(tables[k].name, foreignKeys[i].targetTable) == 0) {
                                    int destX = tables[k].x + 10;
                                    int destY = tables[k].y + 10;
                                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Gérer la couleur
                                    SDL_RenderDrawLine(renderer, tables[j].x + 75, tables[j].y + 80, destX, destY);
                                }
                            }
                        }
                    }
                }

                SDL_RenderPresent(renderer);

                int quit = 0;
                SDL_Event event;

                while (!quit) {
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                            return 0;
                        } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                            // Vérifier si le clic est à l'intérieur de l'une des tables
                            for (int i = 0; i < numTables; i++) {
                                int tableX = tables[i].x;
                                int tableY = tables[i].y;
                                int tableWidth = 200;  // Ajustez la largeur de la table
                                int tableHeight = maxTableHeight + tableHeight + margin * 2;  // Ajustez la hauteur de la table

                                if (event.button.x >= tableX && event.button.x <= tableX + tableWidth &&
                                    event.button.y >= tableY && event.button.y <= tableY + tableHeight) {
                                    int modificationSuccessful = clickTable(conn, dbName, tables[i].name, renderer);
                                    if (modificationSuccessful) {
                                        return 0;
                                    }
                                }
                            }
                        }
                    }
                    SDL_Delay(10);
                }
                mysql_free_result(result);
            } else {
                fprintf(stderr, "\n\t\t\tFailed to retrieve tables: %s\n", mysql_error(conn));
            }
        } else {
            fprintf(stderr, "\n\t\t\tError querying database: %s\n", mysql_error(conn));
        }
    } else {
        fprintf(stderr, "\n\t\t\tFailed to select database '%s': %s\n", dbName, mysql_error(conn));
    }

    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
