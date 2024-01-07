#include "main.h"

int main()
{
    char option;
    int connect = 0;
    char loggedInUsername[50];
    char loggedInPassword[20];
    int result = 0;

    while (option != '0')
    {
        system("cls");
        printf("\t\t====== Graphical database generator and manager ======\n");
        printf("\n\t\t====== Choose what you want : ======\n");
        printf("\n\t\t\t1. Create an account");
        printf("\n\t\t\t2. Log in to an account");
        printf("\n\t\t\t3. Access the database management menu");
        printf("\n\t\t\t0. Exit");

        printf("\n\n\n\t\t\tEnter Your Option: ");
        scanf(" %c", &option);
        getchar();

        switch (option)
        {
        case '1':
            createAccount();
            break;
        case '2':
            result = authenticateUser(&connect, loggedInUsername, loggedInPassword);
            break;
        case '3':
            if (result == 1){
                mainMenu(loggedInUsername, loggedInPassword);
            } else {
                system("cls");
                printf("\n\n\t\t\tYou must be logged in to access the main menu.");
                printf("\n\n\t\t\tEnter any keys to continue.......");
                getch();
            }
            result = 0;
            break;
        /*case '4':
            testMySQLConnection();
            break;
        case '4':
            stateConnect(&connect);
            break;*/
        case '0':
            printf("\n\t\t\t====== Thank You ======");
            break;
        default:
            printf("\n\t\t\tInvalid Option, Please Enter Right Option !\n");
            printf("\n\n\t\t\tEnter any keys to continue.......");
            getch();
        }
    }
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
    getchar();
    gets(studentInformation.studentUsername);
    printf("\t\t\tEnter Student's Password : ");
    gets(studentInformation.studentPassword);

    fwrite(&studentInformation, sizeof(studentInformation), 1, fp);

    printf("\n\n\t\t\tInformations have been stored sucessfully\n");
    printf("\n\n\t\t\tEnter any keys to continue.......");
    getch();

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

        if (strcmp(username, studentInformation.studentUsername) == 0 && strcmp(password, studentInformation.studentPassword) == 0) {
            printf("\n\n\t\t\tLogin Successful!\n");
            *connect = 1;

            strcpy(loggedInUsername, username);
            strcpy(loggedInPassword, password);

            printf("\n\t\t\ttkt: %s\n", loggedInUsername);
            printf("\t\t\ttkt: %s\n", loggedInPassword);

            // fclose(fp);
            // return 1;
        } else {
            printf("\n\n\t\t\tLogin failed. Please try again.\n");
        }
    }

    printf("\n\n\t\t\tEnter any keys to continue.......");
    getch();

    fclose(fp);
    return *connect;
}

int mainMenu(char *loggedInUsername, char *loggedInPassword)
{
    char option;

    while (option != '0')
    {
        system("cls");
        printf("\t\t====== Graphical database generator and manager ======\n");
        printf("\n\t\t====== Choose what you want : ======\n");
        printf("\n\t\t\t1. Create a database");
        printf("\n\t\t\t2. Load a database");
        printf("\n\t\t\t3. Change my settings");
        printf("\n\t\t\t4. Know the database connection status");
        printf("\n\t\t\t0. Sign out");

        printf("\n\n\n\t\t\tEnter Your Option: ");
        scanf(" %c", &option);
        getchar();

        switch (option)
        {
        case '1':
            createDatabase();
            break;
        case '2':
            loadDatabase();
            break;
        case '3':
            changeSettings(loggedInUsername, loggedInPassword);
            break;
        case '4':
            testMySQLConnection();
            break;
        case '0':
            printf("\n\t\t\t====== Thank You ======");
            break;
        default:
            printf("\n\t\t\tInvalid Option, Please Enter Right Option !\n");
            printf("\n\n\t\t\tEnter any keys to continue.......");
            getch();
        }
    }
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

        fseek(fp, -sizeof(currentUser), SEEK_CUR);
        fwrite(&currentUser, sizeof(currentUser), 1, fp);

        printf("\n\n\t\t\tUser settings updated successfully!\n");
    }
    else
    {
        printf("\n\n\t\t\tUser not found. Please check your current username and password.\n");
    }

    fclose(fp);

    printf("\n\n\t\t\tEnter any keys to continue.......");
    getch();
}

int testMySQLConnection()
{
    MYSQL *conn = mysql_init(NULL);

    if (mysql_real_connect(conn, "localhost", "root", "root", "projetC", 3306, NULL, 0)) {
        printf("\n\t\t\tDatabase connection successful\n");
        printf("\n\n\t\t\tEnter any keys to continue.......");
        getch();

        // Exécuter des requêtes SQL ici

        mysql_close(conn);
    } else {
        fprintf(stderr, "\n\t\t\tDatabase connection failed: %s\n", mysql_error(conn));
        printf("\n\n\t\t\tEnter any keys to continue.......");
        getch();
    }

    return 0;
}

int createDatabase()
{
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
        fgets(dbName, sizeof(dbName), stdin);

        dbName[strcspn(dbName, "\n")] = '\0';

        char query[150];
        snprintf(query, sizeof(query), "CREATE DATABASE %s", dbName);

        if (mysql_query(conn, query) == 0) {
            printf("\n\n\t\t\tDatabase '%s' created successfully.\n", dbName);
            printf("\n\n\t\t\tEnter any keys to continue.......");
            getch();
        } else {
            fprintf(stderr, "\n\n\t\t\tError creating database: %s\n", mysql_error(conn));
            printf("\n\n\t\t\tEnter any keys to continue.......");
            getch();
        }

        char option;

        while (option != '0')
        {
            system("cls");
            printf("\t\t====== Manager of your database '%s' ======\n", dbName);
            printf("\n\t\t====== Choose what you want : ======\n");
            printf("\n\t\t\t1. View existing tables");
            printf("\n\t\t\t2. Create a table");
            printf("\n\t\t\t3. Rename a table");
            printf("\n\t\t\t4. Edit a table");
            printf("\n\t\t\t5. Delete a table");
            printf("\n\t\t\t0. Return to previous menu");

            printf("\n\n\n\t\t\tEnter Your Option: ");
            scanf(" %c", &option);
            getchar();

            switch (option)
            {
            case '1':
                displayAllTables(conn, dbName);
                break;
            case '2':
                createTable(conn, dbName);
                break;
            case '3':
                renameTable(conn, dbName);
                break;
            case '4':
                editTable(conn, dbName);
                break;
            case '5':
                deleteTable(conn, dbName);
                break;
            case '0':
                printf("\n\t\t\t====== Thank You ======");
                break;
            default:
                printf("\n\t\t\tInvalid Option, Please Enter Right Option !\n");
                printf("\n\n\t\t\tEnter any keys to continue.......");
                getch();
            }
        }

        mysql_close(conn);
        return 0;
    } else {
        fprintf(stderr, "\n\n\t\t\tFailed to connect to MySQL server: %s\n", mysql_error(conn));
        printf("\n\n\t\t\tEnter any keys to continue.......");
        getch();
        mysql_close(conn);
        return 1;
    }
}

int loadDatabase()
{
    system("cls");
    printf("\t\t\t====== Loading of your new database ======\n");

    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL)
    {
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
        getch();

        const char *createTableQuery = "CREATE TABLE IF NOT EXISTS exemple_table (id INT PRIMARY KEY, nom VARCHAR(255))";
        if (mysql_query(conn, createTableQuery) == 0)
        {
            printf("\n\n\t\t\tTable created successfully.\n");
        }
        else
        {
            fprintf(stderr, "\n\n\t\t\tError creating the table : %s\n", mysql_error(conn));
        }

        char option;

        while (option != '0')
        {
            system("cls");
            printf("\t\t====== Manager of your database '%s' ======\n", dbName);
            printf("\n\t\t====== Choose what you want : ======\n");
            printf("\n\t\t\t1. View existing tables");
            printf("\n\t\t\t2. Create a table");
            printf("\n\t\t\t3. Rename a table");
            printf("\n\t\t\t4. Edit a table");
            printf("\n\t\t\t5. Delete a table");
            printf("\n\t\t\t0. Return to previous menu");

            printf("\n\n\n\t\t\tEnter Your Option: ");
            scanf(" %c", &option);
            getchar();

            switch (option)
            {
            case '1':
                displayAllTables(conn, dbName);
                break;
            case '2':
                createTable(conn, dbName);
                break;
            case '3':
                renameTable(conn, dbName);
                break;
            case '4':
                editTable(conn, dbName);
                break;
            case '5':
                deleteTable(conn, dbName);
                break;
            case '0':
                printf("\n\t\t\t====== Thank You ======");
                break;
            default:
                printf("\n\t\t\tInvalid Option, Please Enter Right Option !\n");
                printf("\n\n\t\t\tEnter any keys to continue.......");
                getch();
            }
        }

        mysql_close(conn);
    }
    else
    {
        fprintf(stderr, "\n\n\t\t\tDatabase connection failed : %s\n", mysql_error(conn));
        printf("\n\n\t\t\tEnter any keys to continue.......");
        getch();
    }

    return EXIT_SUCCESS;
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
    getch();
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
    getch();
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
    getch();
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

        void displayTableColumns(conn, dbName, tableName);

        printf("\n\n\t\t\tEnter any keys to continue.......");
        getch();

    } else {
        fprintf(stderr, "\n\n\t\t\tError modifying table: %s\n", mysql_error(conn));
    }

    printf("\n\n\t\t\tEnter any keys to continue.......");
    getch();
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
    getch();
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
    getch();
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
