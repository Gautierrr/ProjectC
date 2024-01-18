#include "../main.h"

int testMySQLConnection(SDL_Renderer *renderer) {

    const char *pageTitle = "Testing your connection with the MySql server";
    const char *success = "Database connection successful !!!  -------  Enter any keys to continue";
    const char *fail = "Database connection failed...  -------  Enter any keys to continue";

    TTF_Font *font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);

    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, "localhost", "root", "root", "projetC", 3306, NULL, 0)) {
        
        SDL_RenderClear(renderer);
        
        renderText(renderer, font, pageTitle, 150, 100);
        renderText(renderer, font, success, 50, 300);

        SDL_RenderPresent(renderer);

        SDL_Event keyEvent;
        while (SDL_WaitEvent(&keyEvent) && keyEvent.type != SDL_KEYDOWN) {
        }

        mysql_close(conn);
    } else {
        
        SDL_RenderClear(renderer);
        
        renderText(renderer, font, pageTitle, 150, 100);
        renderText(renderer, font, fail, 50, 300);

        SDL_RenderPresent(renderer);

        SDL_Event keyEvent;
        while (SDL_WaitEvent(&keyEvent) && keyEvent.type != SDL_KEYDOWN) {
        }
    }

    TTF_CloseFont(font);
    return 0;
}
