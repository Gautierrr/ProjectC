#ifndef MAIN_H
#define MAIN_H

#include "mysql/include/mysql.h"
#include "sdl/include/SDL2/SDL.h"
#include "sdl/include/SDL2/SDL_image.h"
#include "sdl/include/SDL2/SDL_ttf.h"
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "curl/curl.h"

typedef struct Student
{
    char studentUsername[50];
    char studentPassword[20];
} Student;

typedef struct {
    char name[100];
    int x, y;
} Table;

typedef struct {
    int tableIndex1, tableIndex2;
} Link;

typedef struct {
    char sourceTable[50];
    char sourceColumn[50];
    char targetTable[50];
    char targetColumn[50];
} ForeignKey;

typedef struct {
    char columnName[50];
    char dataType[50];
    char value[100];
} TableColumnData;

typedef struct {
    char name[50];
    char type[50];
} ColumnInfo;

typedef struct {
    char name[100];
    char type[50];
    int x;
    int y;
    int width;
    int height;
} TableColumn;



SDL_Texture *renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y);


// Graphique fait  ->  mais esthétique à faire !!!
void createAccount(SDL_Renderer *renderer);
int authentificateUser(int *connect, char *loggedInUsername, char *loggedInPassword, SDL_Renderer *renderer);
int mainMenu(char *loggedInUsername, char *loggedInPassword, SDL_Renderer *renderer);
void changeSettings(char *currentUsername, char *currentPassword, SDL_Renderer *renderer);
int createDatabase(char *loggedInUsername);
int loadDatabase(char *loggedInUsername, SDL_Renderer *renderer);
void databaseMenu(MYSQL *conn, SDL_Renderer *renderer, const char *dbName, SDL_Renderer *renderer2);
int displayAllTables(MYSQL *conn, const char *dbName, SDL_Renderer *renderer);
int clickTable(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer2);
int viewMcd(MYSQL *conn, const char *dbName, SDL_Renderer *renderer);
void createTable(MYSQL *conn, const char *dbName, SDL_Renderer *renderer2);
void deleteTable(MYSQL *conn, const char *dbName, SDL_Renderer *renderer2);
void renameTable(MYSQL *conn, const char *dbName, SDL_Renderer *renderer2);
int editTable(MYSQL *conn, const char *dbName, SDL_Renderer *renderer, SDL_Renderer *renderer2);
int editTableMenu(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer);
int addColumn(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer2);
// void addPrimaryKey(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer2); // -> Id en auto-Increment donc impossible de changer de clé primaires
void addForeignKey(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer2);
int addContent(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer2);
int deleteContent(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer2);
int displayContent(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer);
int adminMenu(char *loggedInUsername, char *loggedInPassword, SDL_Renderer *renderer);
int viewAllDatabases(SDL_Renderer *renderer);
int deleteDatabase(SDL_Renderer *renderer);
int downloadImg(SDL_Renderer *renderer);

int exportDatabase(MYSQL *conn, const char *dbName);
void exportDatabaseValidation(SDL_Renderer *renderer);
void errorAccount(SDL_Renderer *renderer);
void errorDatabase(SDL_Renderer *renderer);
void errorTable(SDL_Renderer *renderer);
void errorContent(SDL_Renderer *renderer);







// void displayTableColumns(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer);




// int modifyColumn(MYSQL *conn, const char *dbName, const char *tableName, const char *columnName);
// int editContent(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer);










#endif
