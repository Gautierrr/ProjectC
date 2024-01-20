#include "../main.h"

int clickTable(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer) {
    if (mysql_select_db(conn, dbName) == 0) {
        char query[200];
        snprintf(query, sizeof(query), "SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = '%s'", dbName);

        if (mysql_query(conn, query) == 0) {
            MYSQL_RES *result = mysql_store_result(conn);

            if (result != NULL) {
                Table tables[50];
                int numTables = 0;

                SDL_Window *window;
                SDL_CreateWindowAndRenderer(800, 600, 0, &window, &renderer);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                SDL_Color textColor = { 255, 255, 255 };
                TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);

                
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

                SDL_Rect inputRect1 = {500, 100, 150, 30};  // Rect pour le champ de saisie de la colonne à modifier
                SDL_Rect inputRect2 = {500, 200, 150, 30};  // Rect pour le champ de saisie du nouveau nom
                SDL_Rect inputRect3 = {500, 300, 150, 30};  // Rect pour le champ de saisie du nouveau type

                char columnName[100];
                char newColumnName[100];
                char newColumnType[100];

                /*textSurface = TTF_RenderText_Solid(font, columnName, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &inputRect1);
                SDL_DestroyTexture(textTexture);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &inputRect1);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

                // Afficher le champ de saisie
                textSurface = TTF_RenderText_Solid(font, newColumnName, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &inputRect2);
                SDL_DestroyTexture(textTexture);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &inputRect2);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

                // Afficher le champ de saisie
                textSurface = TTF_RenderText_Solid(font, newColumnType, textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &inputRect3);
                SDL_DestroyTexture(textTexture);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &inputRect3);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);*/

                SDL_Rect textRect = {500, 75, 300, 30};
                
                textSurface = TTF_RenderText_Solid(font, "Enter the name of the column to modify : ", textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

                textRect.y = 175;

                textSurface = TTF_RenderText_Solid(font, "Enter the name of the new name column : ", textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

                textRect.y = 275;

                textSurface = TTF_RenderText_Solid(font, "Enter the data type of the new name column : ", textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

                SDL_RenderPresent(renderer);

                //while (row = mysql_fetch_row(result)) {
                strcpy(tables[numTables].name, tableName);
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
                //}

                SDL_RenderPresent(renderer);

                int done = 0;
                SDL_Event event;
                int isTypingTableName = 1;
                int isTypingColumnName = 0;
                int isTypingTypeName = 0;

                // Initialiser les chaînes de caractères à zéro
                memset(columnName, 0, sizeof(columnName));
                memset(newColumnName, 0, sizeof(newColumnName));
                memset(newColumnType, 0, sizeof(newColumnType));

                while (!done) {
                    while (SDL_PollEvent(&event) || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                        if (event.type == SDL_QUIT) {
                            done = 1;
                        } else if (event.type == SDL_KEYDOWN) {
                            if (event.key.keysym.sym == SDLK_RETURN) {
                            if (isTypingTableName) {
                                isTypingTableName = 0;
                                isTypingColumnName = 1;
                            } else if (isTypingColumnName) {
                                isTypingColumnName = 0;
                                isTypingTypeName = 1;
                            } else if (isTypingTypeName) {
                                isTypingTypeName = 1;
                                done = 1;
                            }
                            }
                        } else if (event.type == SDL_TEXTINPUT) {
                            if (isTypingTableName) {
                                strcat(columnName, event.text.text);
                            } else if (isTypingColumnName) {
                                strcat(newColumnName, event.text.text);
                            } else if (isTypingTypeName) {
                                strcat(newColumnType, event.text.text);
                            }

                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                            SDL_RenderFillRect(renderer, &inputRect1);
                            SDL_RenderFillRect(renderer, &inputRect2);
                            SDL_RenderFillRect(renderer, &inputRect3);

                            textSurface = TTF_RenderText_Solid(font, columnName, textColor);
                            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                            SDL_FreeSurface(textSurface);
                            SDL_RenderCopy(renderer, textTexture, NULL, &inputRect1);
                            
                            textSurface = TTF_RenderText_Solid(font, newColumnName, textColor);
                            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                            SDL_FreeSurface(textSurface);
                            SDL_RenderCopy(renderer, textTexture, NULL, &inputRect2);
                            
                            textSurface = TTF_RenderText_Solid(font, newColumnType, textColor);
                            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                            SDL_FreeSurface(textSurface);
                            SDL_RenderCopy(renderer, textTexture, NULL, &inputRect3);

                            SDL_RenderPresent(renderer);
                        }
                    }

                    SDL_Delay(10);
                }

                snprintf(query, sizeof(query), "ALTER TABLE %s CHANGE COLUMN %s %s %s", tableName, columnName, newColumnName, newColumnType);

                if (mysql_query(conn, query) == 0) {
                    printf("\n\n\t\t\tTable '%s' modified successfully.\n\n\n", tableName);

                    // displayTableColumns(conn, dbName, tableName, renderer);
                } else {
                    fprintf(stderr, "\n\n\t\t\tError modifying table: %s\n", mysql_error(conn));
                }

                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);

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
}



















































/*
void clickTable(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer) {
    char query[200];
    snprintf(query, sizeof(query), "SELECT * FROM %s", tableName);

    if (mysql_query(conn, query) == 0) {
        MYSQL_RES *result = mysql_store_result(conn);

        if (result != NULL) {
            int numColumns = mysql_num_fields(result);
            int numRows = mysql_num_rows(result);

            // Récupérer les noms des colonnes
            MYSQL_FIELD *field;
            field = mysql_fetch_fields(result);

            // Créer un tableau de TableColumnData pour stocker les données
            TableColumnData tableData[numRows][numColumns];

            // Récupérer et stocker les données de chaque ligne
            MYSQL_ROW row;
            int rowIndex = 0;

            while ((row = mysql_fetch_row(result)) != NULL) {
                for (int colIndex = 0; colIndex < numColumns; colIndex++) {
                    snprintf(tableData[rowIndex][colIndex].columnName, sizeof(tableData[rowIndex][colIndex].columnName), "%s", field[colIndex].name);
                    snprintf(tableData[rowIndex][colIndex].dataType, sizeof(tableData[rowIndex][colIndex].dataType), "%s", field[colIndex].type == MYSQL_TYPE_LONG ? "INT" : "VARCHAR");

                    if (row[colIndex] != NULL) {
                        snprintf(tableData[rowIndex][colIndex].value, sizeof(tableData[rowIndex][colIndex].value), "%s", row[colIndex]);
                    } else {
                        snprintf(tableData[rowIndex][colIndex].value, sizeof(tableData[rowIndex][colIndex].value), "NULL");
                    }
                }
                rowIndex++;
            }

            // Libérer le résultat
            mysql_free_result(result);

            // Afficher les données à l'aide de SDL
            TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 18);
            SDL_Color textColor = {255, 255, 255};
            int textHeight = 30; // Hauteur du texte pour chaque ligne

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            int startX = 50;
            int startY = 50;

            for (int rowIndex = 0; rowIndex < numRows; rowIndex++) {
                int currentY = startY + rowIndex * textHeight;

// ...
for (int colIndex = 0; colIndex < numColumns; colIndex++) {
    int currentX = startX + colIndex * 200; // Ajustez la largeur des colonnes selon vos besoins

    // Afficher le nom de la colonne
    SDL_Rect columnNameRect = {currentX, currentY, 100, textHeight};
    SDL_Surface *columnNameSurface = TTF_RenderText_Solid(font, tableData[rowIndex][colIndex].columnName, textColor);
    SDL_Texture *columnNameTexture = SDL_CreateTextureFromSurface(renderer, columnNameSurface);
    SDL_FreeSurface(columnNameSurface);
    SDL_RenderCopy(renderer, columnNameTexture, NULL, &columnNameRect);
    SDL_DestroyTexture(columnNameTexture);

    // Afficher le type de données
    SDL_Rect dataTypeRect = {currentX, currentY + textHeight, 100, textHeight};
    SDL_Surface *dataTypeSurface = TTF_RenderText_Solid(font, tableData[rowIndex][colIndex].dataType, textColor);
    SDL_Texture *dataTypeTexture = SDL_CreateTextureFromSurface(renderer, dataTypeSurface);
    SDL_FreeSurface(dataTypeSurface);
    SDL_RenderCopy(renderer, dataTypeTexture, NULL, &dataTypeRect);
    SDL_DestroyTexture(dataTypeTexture);

    // Afficher la valeur
    SDL_Rect valueRect = {currentX, currentY + 2 * textHeight, 100, textHeight};
    SDL_Surface *valueSurface = TTF_RenderText_Solid(font, tableData[rowIndex][colIndex].value, textColor);
    SDL_Texture *valueTexture = SDL_CreateTextureFromSurface(renderer, valueSurface);
    SDL_FreeSurface(valueSurface);
    SDL_RenderCopy(renderer, valueTexture, NULL, &valueRect);
    SDL_DestroyTexture(valueTexture);
}
            }

            SDL_RenderPresent(renderer);

            // Attendre la fermeture de la fenêtre
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

            // Fermer la police TTF
            TTF_CloseFont(font);
        } else {
            fprintf(stderr, "\n\t\t\tFailed to retrieve data from table: %s\n", mysql_error(conn));
        }
    } else {
        fprintf(stderr, "\n\t\t\tError querying table: %s\n", mysql_error(conn));
    }
}*/