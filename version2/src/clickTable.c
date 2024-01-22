#include "../main.h"

int clickTable(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer2) {

    SDL_Window *window = SDL_CreateWindow("Graphical Database Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 500, SDL_WINDOW_RESIZABLE);
    renderer2 = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (mysql_select_db(conn, dbName) == 0) {
        char query[200];
        snprintf(query, sizeof(query), "SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = '%s'", dbName);

        if (mysql_query(conn, query) == 0) {
            MYSQL_RES *result = mysql_store_result(conn);

            if (result != NULL) {
                Table tables[50];
                int numTables = 0;

                SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
                SDL_RenderClear(renderer2);
                SDL_Color textColor = { 0, 0, 0 };
                SDL_Color textColor2 = { 255, 255, 255 };
                TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);

                SDL_Surface *textSurface;
                SDL_Texture *textTexture;
                MYSQL_ROW row;

                int screenWidth = 800;
                int tableWidth = 200;
                int tableHeight = 40;
                int margin = 40;
                int beginning = 200;

                int currentX = margin;
                int currentY = beginning;
                int maxTableHeight = 0;

                SDL_Rect inputRect1 = {500, 75, 150, 30};  // Rect pour le champ de saisie de la colonne à modifier
                SDL_Rect inputRect2 = {500, 255, 150, 30};  // Rect pour le champ de saisie du nouveau nom
                SDL_Rect inputRect3 = {500, 375, 150, 30};  // Rect pour le champ de saisie du nouveau type

                char columnName[100];
                char newColumnName[100];
                char newColumnType[100];

                SDL_Rect textRect = {500, 75, 300, 30};

                SDL_Texture *option1Texture = IMG_LoadTexture(renderer2, "img/nameC.png");
                SDL_Texture *option2Texture = IMG_LoadTexture(renderer2, "img/newNameC.png");
                SDL_Texture *option3Texture = IMG_LoadTexture(renderer2, "img/newTypeC.png");
                SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer2, "img/background2.png");

                SDL_Rect option1Rect = {400, 25, 300, 70};
                SDL_Rect option2Rect = {400, 175, 300, 70};
                SDL_Rect option3Rect = {400, 325, 300, 70};

                SDL_RenderClear(renderer2);
                SDL_RenderCopy(renderer2, backgroundTexture, NULL, NULL);
                SDL_RenderCopy(renderer2, option1Texture, NULL, &option1Rect);
                SDL_RenderCopy(renderer2, option2Texture, NULL, &option2Rect);
                SDL_RenderCopy(renderer2, option3Texture, NULL, &option3Rect);
                SDL_RenderPresent(renderer2);
                
                /*textSurface = TTF_RenderText_Solid(font, "Enter the name of the column to modify : ", textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer2, textTexture, NULL, &textRect);

                textRect.y = 175;

                textSurface = TTF_RenderText_Solid(font, "Enter the name of the new name column : ", textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer2, textTexture, NULL, &textRect);

                textRect.y = 275;

                textSurface = TTF_RenderText_Solid(font, "Enter the data type of the new name column : ", textColor);
                textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                SDL_FreeSurface(textSurface);
                SDL_RenderCopy(renderer2, textTexture, NULL, &textRect);*/


                //while (row = mysql_fetch_row(result)) {
                strcpy(tables[numTables].name, tableName);
                tables[numTables].x = currentX;
                tables[numTables].y = currentY;

                int currentTableHeight = tables[numTables].y + tableHeight + margin;
                if (currentTableHeight > maxTableHeight) {
                    maxTableHeight = currentTableHeight;
                }

                SDL_Rect tableRect = {tables[numTables].x, tables[numTables].y, tableWidth, tableHeight};
                SDL_RenderDrawRect(renderer2, &tableRect);
                SDL_RenderFillRect(renderer2, &tableRect);

                textSurface = TTF_RenderText_Solid(font, tables[numTables].name, textColor2);
                textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                SDL_FreeSurface(textSurface);

                textRect.x = tables[numTables].x + 10;
                textRect.y = tables[numTables].y + 10;

                SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
                SDL_RenderCopy(renderer2, textTexture, NULL, &textRect);
                SDL_DestroyTexture(textTexture);

                snprintf(query, sizeof(query),
                         "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = '%s' AND TABLE_NAME = '%s'", dbName, tables[numTables].name);

                if (mysql_query(conn, query) == 0) {
                    MYSQL_RES *columnResult = mysql_store_result(conn);

                    if (columnResult != NULL) {
                        int currentColumnY = maxTableHeight + 10;  // Aligner verticalement en dessous de la hauteur maximale
                        int currentColumnX = tables[numTables].x;

                        int numColumns = mysql_num_rows(columnResult);

                        SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
                        SDL_Rect entityRect = {tables[numTables].x, tables[numTables].y, tableWidth, tableHeight + numColumns * 30 + 50};
                        SDL_RenderDrawRect(renderer2, &entityRect);
                        SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);

                        MYSQL_ROW columnRow;

                        while (columnRow = mysql_fetch_row(columnResult)) {
                            textSurface = TTF_RenderText_Solid(font, columnRow[0], textColor);
                            textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                            SDL_FreeSurface(textSurface);

                            textRect.x = currentColumnX + 10;
                            textRect.y = currentColumnY;

                            SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
                            SDL_RenderCopy(renderer2, textTexture, NULL, &textRect);
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
                                        textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                                        SDL_FreeSurface(textSurface);

                                        textRect.x = currentColumnX + 10 + textRect.w + 5;
                                        textRect.y = currentColumnY;

                                        SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
                                        SDL_RenderCopy(renderer2, textTexture, NULL, &textRect);
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

                SDL_RenderPresent(renderer2);

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
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                            SDL_DestroyTexture(backgroundTexture);
                            SDL_DestroyTexture(option1Texture);
                            SDL_DestroyTexture(option2Texture);
                            SDL_DestroyTexture(option3Texture);
                            SDL_DestroyRenderer(renderer2);
                            SDL_DestroyWindow(window);
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

                            SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
                            SDL_RenderFillRect(renderer2, &inputRect1);
                            SDL_RenderFillRect(renderer2, &inputRect2);
                            SDL_RenderFillRect(renderer2, &inputRect3);

                            textSurface = TTF_RenderText_Solid(font, columnName, textColor2);
                            textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                            SDL_FreeSurface(textSurface);
                            SDL_RenderCopy(renderer2, textTexture, NULL, &inputRect1);
                            
                            textSurface = TTF_RenderText_Solid(font, newColumnName, textColor2);
                            textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                            SDL_FreeSurface(textSurface);
                            SDL_RenderCopy(renderer2, textTexture, NULL, &inputRect2);
                            
                            textSurface = TTF_RenderText_Solid(font, newColumnType, textColor2);
                            textTexture = SDL_CreateTextureFromSurface(renderer2, textSurface);
                            SDL_FreeSurface(textSurface);
                            SDL_RenderCopy(renderer2, textTexture, NULL, &inputRect3);

                            SDL_RenderPresent(renderer2);
                        }
                    }                
                    SDL_Delay(10);
                }

                snprintf(query, sizeof(query), "ALTER TABLE %s CHANGE COLUMN %s %s %s", tableName, columnName, newColumnName, newColumnType);

                if (mysql_query(conn, query) == 0) {
                    printf("\n\n\t\t\tTable '%s' modified successfully.\n\n\n", tableName);
                    SDL_DestroyTexture(backgroundTexture);
                    SDL_DestroyTexture(option1Texture);
                    SDL_DestroyTexture(option2Texture);
                    SDL_DestroyTexture(option3Texture);
                    SDL_DestroyRenderer(renderer2);
                    SDL_DestroyWindow(window);
                } else {
                    fprintf(stderr, "\n\n\t\t\tError modifying table: %s\n", mysql_error(conn));
                }
                SDL_DestroyTexture(backgroundTexture);
                SDL_DestroyTexture(option1Texture);
                SDL_DestroyTexture(option2Texture);
                SDL_DestroyTexture(option3Texture);
                SDL_DestroyRenderer(renderer2);
                SDL_DestroyWindow(window);
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
