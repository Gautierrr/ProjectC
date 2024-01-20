#include "../main.h"

int editTableMenu(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer) {
    SDL_Texture *option1Texture = IMG_LoadTexture(renderer, "img/primaryKey.png");
    SDL_Texture *option2Texture = IMG_LoadTexture(renderer, "img/ForeignKey.png");
    // SDL_Texture *option3Texture = IMG_LoadTexture(renderer, "img/createTable.png");
    // SDL_Texture *option4Texture = IMG_LoadTexture(renderer, "img/renameTable.png");
    // SDL_Texture *option5Texture = IMG_LoadTexture(renderer, "img/editTable.png");
    // SDL_Texture *option6Texture = IMG_LoadTexture(renderer, "img/deleteTable.png");
    SDL_Texture *option0Texture = IMG_LoadTexture(renderer, "img/returnMenu.png");

    SDL_Rect option1Rect = {200, 50, 400, 40};
    SDL_Rect option2Rect = {200, 125, 400, 40};
    // SDL_Rect option3Rect = {200, 200, 400, 40};
    // SDL_Rect option4Rect = {150, 250, 500, 40};
    // SDL_Rect option5Rect = {150, 350, 500, 50};
    // SDL_Rect option6Rect = {150, 450, 500, 50};
    SDL_Rect option0Rect = {150, 550, 500, 50};

    char option;

    while (option != '0') {
        SDL_Event event;
        SDL_PollEvent(&event);

        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    if (mouseX > option1Rect.x && mouseX < option1Rect.x + option1Rect.w &&
                        mouseY > option1Rect.y && mouseY < option1Rect.y + option1Rect.h) {
                        option = '1';
                        addPrimaryKey(conn, dbName, tableName, renderer);
                    } else if (mouseX > option2Rect.x && mouseX < option2Rect.x + option2Rect.w &&
                               mouseY > option2Rect.y && mouseY < option2Rect.y + option2Rect.h) {
                        option = '2';
                        // viewMcd(conn, dbName, renderer);
                    } /*else if (mouseX > option3Rect.x && mouseX < option3Rect.x + option3Rect.w &&
                               mouseY > option3Rect.y && mouseY < option3Rect.y + option3Rect.h) {
                        option = '3';
                        createTable(conn, dbName, renderer);
                    } else if (mouseX > option4Rect.x && mouseX < option4Rect.x + option4Rect.w &&
                               mouseY > option4Rect.y && mouseY < option4Rect.y + option4Rect.h) {
                        option = '4';
                        renameTable(conn, dbName, renderer);
                    } else if (mouseX > option5Rect.x && mouseX < option5Rect.x + option5Rect.w &&
                               mouseY > option5Rect.y && mouseY < option5Rect.y + option5Rect.h) {
                        option = '5';
                        editTable(conn, dbName, renderer);
                    } else if (mouseX > option6Rect.x && mouseX < option6Rect.x + option6Rect.w &&
                               mouseY > option6Rect.y && mouseY < option6Rect.y + option6Rect.h) {
                        option = '6';
                        deleteTable(conn, dbName, renderer);
                    }*/ else if (mouseX > option0Rect.x && mouseX < option0Rect.x + option0Rect.w &&
                               mouseY > option0Rect.y && mouseY < option0Rect.y + option0Rect.h) {
                        option = '0';
                        return 0;
                    }
                }
                break;
        }

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, option1Texture, NULL, &option1Rect);
        SDL_RenderCopy(renderer, option2Texture, NULL, &option2Rect);
        // SDL_RenderCopy(renderer, option3Texture, NULL, &option3Rect);
        // SDL_RenderCopy(renderer, option4Texture, NULL, &option4Rect);
        // SDL_RenderCopy(renderer, option5Texture, NULL, &option5Rect);
        // SDL_RenderCopy(renderer, option6Texture, NULL, &option6Rect);
        SDL_RenderCopy(renderer, option0Texture, NULL, &option0Rect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(option1Texture);
    SDL_DestroyTexture(option2Texture);
    // SDL_DestroyTexture(option3Texture);
    // SDL_DestroyTexture(option4Texture);
    // SDL_DestroyTexture(option5Texture);
    // SDL_DestroyTexture(option6Texture);
    SDL_DestroyTexture(option0Texture);

    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
