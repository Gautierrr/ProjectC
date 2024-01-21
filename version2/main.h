#ifndef MAIN_H
#define MAIN_H

#include "mysql/include/mysql.h"
#include "sdl/include/SDL2/SDL.h"
#include "sdl/include/SDL2/SDL_image.h"
#include "sdl/include/SDL2/SDL_ttf.h"
#include <stdio.h>
#include <string.h>
#include <conio.h>

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
// int testMySQLConnection(SDL_Renderer *renderer);
int createDatabase(SDL_Renderer *renderer, char *loggedInUsername);             // pour savoir a qui appartient la bdd =   mabdd/username  ->  ex   coucou/gautier
int loadDatabase(SDL_Renderer *renderer, char *loggedInUsername);               // comme ca identification de qui peux modifier tel et tel bdd
void databaseMenu(MYSQL *conn, SDL_Renderer *renderer, const char *dbName);
int displayAllTables(MYSQL *conn, const char *dbName, SDL_Renderer *renderer);
int viewMcd(MYSQL *conn, const char *dbName, SDL_Renderer *renderer);
void createTable(MYSQL *conn, const char *dbName, SDL_Renderer *renderer);
void renameTable(MYSQL *conn, const char *dbName, SDL_Renderer *renderer);
int editTable(MYSQL *conn, const char *dbName, SDL_Renderer *renderer);
void deleteTable(MYSQL *conn, const char *dbName, SDL_Renderer *renderer);
void displayTableColumns(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer);




int clickTable(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer);
// int modifyColumn(MYSQL *conn, const char *dbName, const char *tableName, const char *columnName);
int editTableMenu(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer);
void addPrimaryKey(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer);
void addForeignKey(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer);
// int editContent(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer);
int addColumn(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer);
int addContent(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer);
int displayContent(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer);
int deleteContent(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer);
int adminMenu(char *loggedInUsername, char *loggedInPassword, SDL_Renderer *renderer);
int viewAllDatabases(SDL_Renderer *renderer);
int deleteDatabase(SDL_Renderer *renderer);




// ########################################################################
// #################                                      #################
// #################           Voir pour le MCD           #################
// #################                                      #################
// ########################################################################


#endif
