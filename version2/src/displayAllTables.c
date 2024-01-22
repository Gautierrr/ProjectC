#include "../main.h"

int displayAllTables(MYSQL *conn, const char *dbName, SDL_Renderer *renderer) {
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

                TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Bold.ttf", 18);
                SDL_Color textColor = {0, 0, 0};
                SDL_Color textColor2 = {255, 255, 255};

                SDL_RenderClear(renderer);

                SDL_Texture *option1Texture = IMG_LoadTexture(renderer, "img/allTables.png");
                SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer, "img/banniere.png");
                SDL_Rect option1Rect = {550, 200, 450, 150};

                SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
                SDL_RenderCopy(renderer, option1Texture, NULL, &option1Rect);
                SDL_RenderPresent(renderer);

                SDL_Rect textRect = {50, 350, 300, 30};
                SDL_Surface *textSurface;
                SDL_Texture *textTexture;
                MYSQL_ROW row;

                int screenWidth = 1500;
                int tableWidth = 200;
                int tableHeight = 40;
                int margin = 40;
                int beginning = 350;

                int currentX = margin;
                int currentY = beginning;
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
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                    SDL_RenderDrawRect(renderer, &tableRect);
                    SDL_RenderFillRect(renderer, &tableRect);

                    textSurface = TTF_RenderText_Solid(font, tables[numTables].name, textColor2);
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
                            int currentColumnY = maxTableHeight + 10;
                            int currentColumnX = tables[numTables].x;

                            int numColumns = mysql_num_rows(columnResult);

                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                            SDL_Rect entityRect = {tables[numTables].x, tables[numTables].y, tableWidth, tableHeight + numColumns * 30 + 50};
                            SDL_RenderDrawRect(renderer, &entityRect);
                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

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
                        maxTableHeight = 0;
                    }

                    numTables++;
                }

                SDL_RenderPresent(renderer);

                int quit = 0;
                SDL_Event event;

                while (!quit) {
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                            quit = 1;
                            return 0;
                        } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                            for (int i = 0; i < numTables; i++) {
                                int tableX = tables[i].x;
                                int tableY = tables[i].y;
                                int tableWidth = 200;
                                int tableHeight = 40;

                                if (event.button.x >= tableX && event.button.x <= tableX + tableWidth &&
                                    event.button.y >= tableY && event.button.y <= tableY + tableHeight) {
                                    int modificationSuccessful = clickTable(conn, dbName, tables[i].name, renderer);
                                    if (modificationSuccessful || !modificationSuccessful) {
                                        quit = 1;
                                        return 0;
                                    }
                                }
                            }
                        }
                    }
                    SDL_Delay(10);
                }
                SDL_DestroyTexture(backgroundTexture);
                SDL_DestroyTexture(option1Texture);
                SDL_DestroyRenderer(renderer);
                mysql_free_result(result);
                return 0;
            } else {
                errorTable(renderer);
            }
        } else {
            errorDatabase(renderer);
        }
    } else {
        errorDatabase(renderer);
    }

    SDL_DestroyRenderer(renderer);
    return 0;
}
