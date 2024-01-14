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


void createAccount(SDL_Renderer *renderer);



int authenticateUser(int *connect, char *loggedInUsername, char *loggedInPassword);
int mainMenu(char *loggedInUsername, char *loggedInPassword, SDL_Renderer *renderer);
// void displayInfo();
void changeSettings(char *currentUsername, char *currentPassword);
int testMySQLConnection(SDL_Renderer *renderer);
void displayAllTables(MYSQL *conn, const char *dbName);
void createTable(MYSQL *conn, const char *dbName);
void renameTable(MYSQL *conn, const char *dbName);
void editTable(MYSQL *conn, const char *dbName);
void deleteTable(MYSQL *conn, const char *dbName);
void displayTableColumns(MYSQL *conn, const char *dbName, const char *tableName);

int createDatabase(SDL_Renderer *renderer);
int loadDatabase(SDL_Renderer *renderer);

void databaseMenu(MYSQL *conn, SDL_Renderer *renderer, const char *dbName);








#endif
