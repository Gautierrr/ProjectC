#include "main.h"

SDL_Texture *renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y)
{
    SDL_Color textColor = {255, 255, 255, 255};  // Blanc
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);

    return textTexture;
}

int SDL_main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Graphical Database Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);    
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "SDL_ttf initialization failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Texture *option1Texture = IMG_LoadTexture(renderer, "img/createAnAccount.png");
    SDL_Texture *option2Texture = IMG_LoadTexture(renderer, "img/logAccount.png");
    SDL_Texture *option3Texture = IMG_LoadTexture(renderer, "img/accessDatabaseMenu.png");
    SDL_Texture *option0Texture = IMG_LoadTexture(renderer, "img/exit.png");

    SDL_Rect option1Rect = {200, 100, 400, 40};
    SDL_Rect option2Rect = {200, 200, 400, 40};
    SDL_Rect option3Rect = {150, 300, 500, 50};
    SDL_Rect option0Rect = {325, 400, 150, 50};


    char option;
    int connect = 0;
    char loggedInUsername[50];
    char loggedInPassword[20];
    int result = 0;

    while (option != '0') {
        SDL_Event event;
        SDL_PollEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                option = '0';
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    if (mouseX > option1Rect.x && mouseX < option1Rect.x + option1Rect.w &&
                        mouseY > option1Rect.y && mouseY < option1Rect.y + option1Rect.h) {
                        option = '1';
                        createAccount();
                    } else if (mouseX > option2Rect.x && mouseX < option2Rect.x + option2Rect.w &&
                               mouseY > option2Rect.y && mouseY < option2Rect.y + option2Rect.h) {
                        option = '2';
                        result = authenticateUser(&connect, loggedInUsername, loggedInPassword);
                        printf("Connect value after authentication: %d\n", connect);
                    } else if (mouseX > option3Rect.x && mouseX < option3Rect.x + option3Rect.w &&
                               mouseY > option3Rect.y && mouseY < option3Rect.y + option3Rect.h) {
                        option = '3';
                        if (result == 1) {
                            mainMenu(loggedInUsername, loggedInPassword, renderer);
                        } else {
                            printf("\n\n\t\t\tYou must be logged in to access the main menu.");
                            printf("\n\n\t\t\tEnter any keys to continue.......");
                        }
                        result = 0;
                    } else if (mouseX > option0Rect.x && mouseX < option0Rect.x + option0Rect.w &&
                               mouseY > option0Rect.y && mouseY < option0Rect.y + option0Rect.h) {
                        option = '0';
                        printf("\n\t\t\t====== Thank You ======");
                    }
                }
                break;
        }

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, option1Texture, NULL, &option1Rect);
        SDL_RenderCopy(renderer, option2Texture, NULL, &option2Rect);
        SDL_RenderCopy(renderer, option3Texture, NULL, &option3Rect);
        SDL_RenderCopy(renderer, option0Texture, NULL, &option0Rect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(option1Texture);
    SDL_DestroyTexture(option2Texture);
    SDL_DestroyTexture(option3Texture);
    SDL_DestroyTexture(option0Texture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

void createAccount()
{
    FILE *fp = fopen("bdd/studentInfo.bin", "ab+");

    if (fp == NULL)
    {
        printf("\n\t\t\tError !\n");
    }

    Student studentInformation;

    system("cls");

    printf("\t\t\t====== Create Student Account ======\n");

    printf("\n\t\t\tEnter Student's Username : ");
    fflush(stdin);
    fgets(studentInformation.studentUsername, sizeof(studentInformation.studentUsername), stdin);
    studentInformation.studentUsername[strcspn(studentInformation.studentUsername, "\n")] = '\0';

    printf("\t\t\tEnter Student's Password : ");
    fflush(stdin);
    fgets(studentInformation.studentPassword, sizeof(studentInformation.studentPassword), stdin);
    studentInformation.studentPassword[strcspn(studentInformation.studentPassword, "\n")] = '\0';

    fwrite(&studentInformation, sizeof(studentInformation), 1, fp);

    printf("\n\n\t\t\tInformations have been stored sucessfully\n");
    printf("\n\n\t\t\tEnter any keys to continue.......");

    fclose(fp);
}

int authenticateUser(int *connect, char *loggedInUsername, char *loggedInPassword)
{
    FILE *fp = fopen("bdd/studentInfo.bin", "rb");

    if (fp == NULL)
    {
        printf("\n\t\t\tError !\n");
    }

    Student studentInformation;

    system("cls");

    printf("\t\t\t====== Connect Student Account ======\n");

    char username[50];
    char password[20];

    printf("\n\t\t\tEnter your username: ");
    scanf("%s", username);

    printf("\t\t\tEnter your password: ");
    scanf("%s", password);

    printf("\n\t\t\tEntered username: %s\n", username);
    printf("\t\t\tEntered password: %s\n", password);

    while (fread(&studentInformation, sizeof(studentInformation), 1, fp) == 1) {

        printf("\n\t\t\tRead username: %s\n", studentInformation.studentUsername);
        printf("\t\t\tRead password: %s\n", studentInformation.studentPassword);

        if (strcasecmp(username, studentInformation.studentUsername) == 0 && strcmp(password, studentInformation.studentPassword) == 0) {
            *connect = 1;
            printf("\n\n\t\t\tLogin Successful!\n");

            strcpy(loggedInUsername, username);
            strcpy(loggedInPassword, password);

            printf("\n\t\t\ttkt: %s\n", loggedInUsername);
            printf("\t\t\ttkt: %s\n", loggedInPassword);

            fclose(fp);
            return *connect;
        }
    }
    printf("\n\n\t\t\tLogin failed. Please try again.\n");
    printf("\n\n\t\t\tEnter any keys to continue.......");

    *connect = 0;
    fclose(fp);
    return *connect;
}

int mainMenu(char *loggedInUsername, char *loggedInPassword, SDL_Renderer *renderer)
{
    SDL_Texture *option1Texture = IMG_LoadTexture(renderer, "img/createDatabase.png");
    SDL_Texture *option2Texture = IMG_LoadTexture(renderer, "img/loadDatabase.png");
    SDL_Texture *option3Texture = IMG_LoadTexture(renderer, "img/changeSettings.png");
    SDL_Texture *option4Texture = IMG_LoadTexture(renderer, "img/testMysql.png");
    SDL_Texture *option0Texture = IMG_LoadTexture(renderer, "img/exit.png");

    SDL_Rect option1Rect = {200, 100, 400, 40};
    SDL_Rect option2Rect = {200, 200, 400, 40};
    SDL_Rect option3Rect = {150, 300, 500, 50};
    SDL_Rect option4Rect = {150, 400, 500, 50};
    SDL_Rect option0Rect = {325, 500, 150, 50};


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
                        createDatabase(renderer);
                    } else if (mouseX > option2Rect.x && mouseX < option2Rect.x + option2Rect.w &&
                               mouseY > option2Rect.y && mouseY < option2Rect.y + option2Rect.h) {
                        option = '2';
                        loadDatabase(renderer);
                    } else if (mouseX > option3Rect.x && mouseX < option3Rect.x + option3Rect.w &&
                               mouseY > option3Rect.y && mouseY < option3Rect.y + option3Rect.h) {
                        option = '3';
                        changeSettings(loggedInUsername, loggedInPassword);
                    } else if (mouseX > option4Rect.x && mouseX < option4Rect.x + option4Rect.w &&
                               mouseY > option4Rect.y && mouseY < option4Rect.y + option4Rect.h) {
                        option = '4';
                        testMySQLConnection(renderer);
                    } else if (mouseX > option0Rect.x && mouseX < option0Rect.x + option0Rect.w &&
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
        SDL_RenderCopy(renderer, option3Texture, NULL, &option3Rect);
        SDL_RenderCopy(renderer, option4Texture, NULL, &option4Rect);
        SDL_RenderCopy(renderer, option0Texture, NULL, &option0Rect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(option1Texture);
    SDL_DestroyTexture(option2Texture);
    SDL_DestroyTexture(option3Texture);
    SDL_DestroyTexture(option4Texture);
    SDL_DestroyTexture(option0Texture);

    SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void changeSettings(char *currentUsername, char *currentPassword)
{
    FILE *fp = fopen("bdd/studentInfo.bin", "rb+");

    if (fp == NULL)
    {
        printf("\n\t\t\tError !\n");
        return;
    }

    Student currentUser;

    system("cls");
    printf("\t\t\t====== Change User Settings ======\n");

    int found = 0;

    while (fread(&currentUser, sizeof(currentUser), 1, fp) == 1)
    {
        if (strcmp(currentUsername, currentUser.studentUsername) == 0 && strcmp(currentPassword, currentUser.studentPassword) == 0)
        {
            found = 1;
            break;
        }
    }

    if (found)
    {
        printf("\n\t\t\tEnter your new username: ");
        scanf("%s", currentUser.studentUsername);

        printf("\t\t\tEnter your new password: ");
        scanf("%s", currentUser.studentPassword);

        fseek(fp, -((long)sizeof(currentUser)), SEEK_CUR);
        fwrite(&currentUser, sizeof(currentUser), 1, fp);

        printf("\n\n\t\t\tUser settings updated successfully!\n");
    }
    else
    {
        printf("\n\n\t\t\tUser not found. Please check your current username and password.\n");
    }

    fclose(fp);

    printf("\n\n\t\t\tEnter any keys to continue.......");
}

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

int createDatabase(SDL_Renderer *renderer) {
    system("cls");
    printf("\t\t\t====== Creation of your new database ======\n");

    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "\n\n\t\t\tMySQL connection initialization error\n");
        return 1;
    }

    if (mysql_real_connect(conn, "localhost", "root", "root", NULL, 3306, NULL, 0)) {
        char dbName[100];
        printf("\n\n\t\t\tEnter the name of the new database : ");
        scanf("%s", dbName);

        dbName[strcspn(dbName, "\n")] = '\0';

        char query[150];
        snprintf(query, sizeof(query), "CREATE DATABASE %s", dbName);

        if (mysql_query(conn, query) == 0) {
            printf("\n\n\t\t\tDatabase '%s' created successfully.\n", dbName);
            printf("\n\n\t\t\tEnter any keys to continue.......");
        } else {
            fprintf(stderr, "\n\n\t\t\tError creating database: %s\n", mysql_error(conn));
            printf("\n\n\t\t\tEnter any keys to continue.......");
        }

        databaseMenu(conn, renderer, dbName);

        mysql_close(conn);
        return 0;

    } else {
        fprintf(stderr, "\n\n\t\t\tFailed to connect to MySQL server: %s\n", mysql_error(conn));
        printf("\n\n\t\t\tEnter any keys to continue.......");
        mysql_close(conn);
        return 1;
    }
}

int loadDatabase(SDL_Renderer *renderer) {
    system("cls");
    printf("\t\t\t====== Loading of your database ======\n");

    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "\n\n\t\t\tMySQL connection initialization error\n");
        return EXIT_FAILURE;
    }

    char dbName[100];
    printf("\n\n\t\t\tEnter the name of the database to load : ");
    scanf("%s", dbName);

    if (mysql_real_connect(conn, "localhost", "root", "root", dbName, 3306, NULL, 0))
    {
        printf("\n\n\t\t\tConnection to database '%s' successful.\n", dbName);
        printf("\n\n\t\t\tEnter any keys to continue.......");

        /*const char *createTableQuery = "CREATE TABLE IF NOT EXISTS exemple_table (id INT PRIMARY KEY, nom VARCHAR(255))";
        if (mysql_query(conn, createTableQuery) == 0)
        {
            printf("\n\n\t\t\tTable loaded successfully.\n");
        }
        else
        {
            fprintf(stderr, "\n\n\t\t\tError loading the table : %s\n", mysql_error(conn));
        }*/

        databaseMenu(conn, renderer, dbName);

        mysql_close(conn);
        return 0;
    }
    else
    {
        fprintf(stderr, "\n\n\t\t\tDatabase connection failed : %s\n", mysql_error(conn));
        printf("\n\n\t\t\tEnter any keys to continue.......");
        mysql_close(conn);
        return 1;
    }
}

void databaseMenu(MYSQL *conn, SDL_Renderer *renderer, const char *dbName) {
    SDL_Texture *option1Texture = IMG_LoadTexture(renderer, "img/viewTables.png");
    SDL_Texture *option2Texture = IMG_LoadTexture(renderer, "img/createTable.png");
    SDL_Texture *option3Texture = IMG_LoadTexture(renderer, "img/renameTable.png");
    SDL_Texture *option4Texture = IMG_LoadTexture(renderer, "img/editTable.png");
    SDL_Texture *option5Texture = IMG_LoadTexture(renderer, "img/deleteTable.png");
    SDL_Texture *option0Texture = IMG_LoadTexture(renderer, "img/returnMenu.png");

    SDL_Rect option1Rect = {200, 50, 400, 40};
    SDL_Rect option2Rect = {200, 150, 400, 40};
    SDL_Rect option3Rect = {150, 250, 500, 50};
    SDL_Rect option4Rect = {150, 350, 500, 50};
    SDL_Rect option5Rect = {150, 450, 500, 50};
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
                        displayAllTables(conn, dbName);
                    } else if (mouseX > option2Rect.x && mouseX < option2Rect.x + option2Rect.w &&
                               mouseY > option2Rect.y && mouseY < option2Rect.y + option2Rect.h) {
                        option = '2';
                        createTable(conn, dbName);
                    } else if (mouseX > option3Rect.x && mouseX < option3Rect.x + option3Rect.w &&
                               mouseY > option3Rect.y && mouseY < option3Rect.y + option3Rect.h) {
                        option = '3';
                        renameTable(conn, dbName);
                    } else if (mouseX > option4Rect.x && mouseX < option4Rect.x + option4Rect.w &&
                               mouseY > option4Rect.y && mouseY < option4Rect.y + option4Rect.h) {
                        option = '4';
                        editTable(conn, dbName);
                    } else if (mouseX > option5Rect.x && mouseX < option5Rect.x + option5Rect.w &&
                               mouseY > option5Rect.y && mouseY < option5Rect.y + option5Rect.h) {
                        option = '5';
                        deleteTable(conn, dbName);
                    } else if (mouseX > option0Rect.x && mouseX < option0Rect.x + option0Rect.w &&
                               mouseY > option0Rect.y && mouseY < option0Rect.y + option0Rect.h) {
                        option = '0';
                        return;
                    }
                }
                break;
        }

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, option1Texture, NULL, &option1Rect);
        SDL_RenderCopy(renderer, option2Texture, NULL, &option2Rect);
        SDL_RenderCopy(renderer, option3Texture, NULL, &option3Rect);
        SDL_RenderCopy(renderer, option4Texture, NULL, &option4Rect);
        SDL_RenderCopy(renderer, option5Texture, NULL, &option5Rect);
        SDL_RenderCopy(renderer, option0Texture, NULL, &option0Rect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(option1Texture);
    SDL_DestroyTexture(option2Texture);
    SDL_DestroyTexture(option3Texture);
    SDL_DestroyTexture(option4Texture);
    SDL_DestroyTexture(option5Texture);
    SDL_DestroyTexture(option0Texture);

    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void displayAllTables(MYSQL *conn, const char *dbName) {
    if (mysql_select_db(conn, dbName) == 0) {
        MYSQL_RES *result = mysql_list_tables(conn, NULL);

        if (result != NULL) {
            MYSQL_ROW row;

            printf("\n\t\t\t====== All Tables in Database '%s' ======\n\n", dbName);
            while ((row = mysql_fetch_row(result)) != NULL) {
                printf("\t\t\t%s\n", row[0]);
            }

            mysql_free_result(result);
        } else {
            fprintf(stderr, "\n\t\t\tFailed to retrieve tables: %s\n", mysql_error(conn));
        }
    } else {
        fprintf(stderr, "\n\t\t\tFailed to select database '%s': %s\n", dbName, mysql_error(conn));
    }

    printf("\n\n\t\t\tEnter any keys to continue.......");
}

void createTable(MYSQL *conn, const char *dbName) {
    char tableName[100];
    char query[150];

    printf("\n\t\t\tEnter the name of the new table: ");
    scanf("%s", tableName);

    snprintf(query, sizeof(query), "CREATE TABLE IF NOT EXISTS %s (id INT PRIMARY KEY, name VARCHAR(255))", tableName);

    if (mysql_query(conn, query) == 0) {
        printf("\n\n\t\t\tTable '%s' created successfully.\n", tableName);
    } else {
        fprintf(stderr, "\n\n\t\t\tError creating table: %s\n", mysql_error(conn));
    }

    printf("\n\n\t\t\tEnter any keys to continue.......");
}

void renameTable(MYSQL *conn, const char *dbName) {
    char oldTableName[100];
    char newTableName[100];
    char query[250];

    printf("\n\t\t\tEnter the current name of the table: ");
    scanf("%s", oldTableName);

    printf("\n\t\t\tEnter the new name for the table: ");
    scanf("%s", newTableName);

    snprintf(query, sizeof(query), "RENAME TABLE %s TO %s", oldTableName, newTableName);

    if (mysql_query(conn, query) == 0) {
        printf("\n\n\t\t\tTable '%s' renamed to '%s' successfully.\n", oldTableName, newTableName);
    } else {
        fprintf(stderr, "\n\n\t\t\tError renaming table: %s\n", mysql_error(conn));
    }

    printf("\n\n\t\t\tEnter any keys to continue.......");
}

void editTable(MYSQL *conn, const char *dbName) {
    char tableName[100];
    char columnName[100];
    char columnType[50];
    char query[250];

    printf("\n\t\t\tEnter the name of the table to modify: ");
    scanf("%s", tableName);

    printf("\n\t\t\tEnter the name of the new column: ");
    scanf("%s", columnName);

    printf("\n\t\t\tEnter the data type of the new column: ");
    scanf("%s", columnType);

    snprintf(query, sizeof(query), "ALTER TABLE %s ADD COLUMN %s %s", tableName, columnName, columnType);

    if (mysql_query(conn, query) == 0) {
        printf("\n\n\t\t\tTable '%s' modified successfully.\n\n\n", tableName);

        displayTableColumns(conn, dbName, tableName);

        printf("\n\n\t\t\tEnter any keys to continue.......");
        getch();

    } else {
        fprintf(stderr, "\n\n\t\t\tError modifying table: %s\n", mysql_error(conn));
    }

    printf("\n\n\t\t\tEnter any keys to continue.......");
}

void deleteTable(MYSQL *conn, const char *dbName) {
    char tableName[100];
    char query[150];

    printf("\n\t\t\tEnter the name of the table to delete: ");
    scanf("%s", tableName);

    snprintf(query, sizeof(query), "DROP TABLE IF EXISTS %s", tableName);

    if (mysql_query(conn, query) == 0) {
        printf("\n\n\t\t\tTable '%s' deleted successfully.\n", tableName);
    } else {
        fprintf(stderr, "\n\n\t\t\tError deleting table: %s\n", mysql_error(conn));
    }

    printf("\n\n\t\t\tEnter any keys to continue.......");
}

void displayTableColumns(MYSQL *conn, const char *dbName, const char *tableName) {
    char query[200];
    MYSQL_RES *result;
    MYSQL_ROW row;

    snprintf(query, sizeof(query), "DESCRIBE %s", tableName);

    if (mysql_query(conn, query) == 0) {
        result = mysql_store_result(conn);

        if (result != NULL) {
            printf("\n\t\t\t====== Columns of Table '%s' in Database '%s' ======\n", tableName, dbName);
            printf("\n\t\t%-20s%-20s\n", "Column Name", "Data Type");
            printf("\t\t----------------------------------------\n");

            while ((row = mysql_fetch_row(result)) != NULL) {
                printf("\t\t%-20s%-20s\n", row[0], row[1]);
            }

            mysql_free_result(result);
        } else {
            fprintf(stderr, "\n\n\t\t\tError retrieving columns: %s\n", mysql_error(conn));
        }
    } else {
        fprintf(stderr, "\n\n\t\t\tError describing table: %s\n", mysql_error(conn));
    }

    printf("\n\n\t\t\tEnter any keys to continue.......");
}







// La fonction 'displayTableColumns' ne fonctionne pas encore, la revoir !!!!
















/*
void displayInfo()
{
    FILE *fp = fopen("bdd/studentInfo.bin", "rb");

    if (fp == NULL)
    {
        printf("\n\t\t\tError !\n");
    }

    Student studentInformation;

    system("cls");

    printf("\t\t\t\t====== All Students Information ======\n");

    printf("\n\n\t\t%-20s%-20s\n", "Username", "Password");
    printf("\t\t----------------------------------------------------------------------------------------");

    while (fread(&studentInformation, sizeof(studentInformation), 1, fp) == 1)
    {
        printf("\n\n\t\t%-20s%-20s",studentInformation.studentUsername, studentInformation.studentPassword);
    }

    printf("\n\n\t\tEnter any keys to continue.......");
    getch();

    fclose(fp);
}

stateConnect(int *connect)
{
    printf("%d", *connect);
    printf("\n\n\t\t\tEnter any keys to continue.......");
    getch();
}*/
