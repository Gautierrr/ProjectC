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


SDL_Texture *renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y);


// Graphique fait  ->  mais esthétique à faire !!!
void createAccount(SDL_Renderer *renderer);
int authenticateUser(int *connect, char *loggedInUsername, char *loggedInPassword, SDL_Renderer *renderer);
int mainMenu(char *loggedInUsername, char *loggedInPassword, SDL_Renderer *renderer);
void changeSettings(char *currentUsername, char *currentPassword, SDL_Renderer *renderer);
int testMySQLConnection(SDL_Renderer *renderer);
int createDatabase(SDL_Renderer *renderer);             // pour savoir a qui appartient la bdd =   mabdd/username  ->  ex   coucou/gautier
int loadDatabase(SDL_Renderer *renderer);               // comme ca identification de qui peux modifier tel et tel bdd
void databaseMenu(MYSQL *conn, SDL_Renderer *renderer, const char *dbName);
void displayAllTables(MYSQL *conn, const char *dbName, SDL_Renderer *renderer);
void createTable(MYSQL *conn, const char *dbName, SDL_Renderer *renderer);
void renameTable(MYSQL *conn, const char *dbName, SDL_Renderer *renderer);
void editTable(MYSQL *conn, const char *dbName, SDL_Renderer *renderer);
void deleteTable(MYSQL *conn, const char *dbName, SDL_Renderer *renderer);
void displayTableColumns(MYSQL *conn, const char *dbName, const char *tableName, SDL_Renderer *renderer);



// ########################################################################
// #################                                      #################
// #################           Voir pour le MCD           #################
// #################                                      #################
// ########################################################################


#endif
