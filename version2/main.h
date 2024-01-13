#ifndef MAIN_H
#define MAIN_H

#include "mysql/include/mysql.h"
#include <stdio.h>
#include <string.h>
#include <conio.h>

typedef struct Student
{
    char studentUsername[50];
    char studentPassword[20];
} Student;

void createAccount();
int authenticateUser(int *connect, char *loggedInUsername, char *loggedInPassword);
int mainMenu(char *loggedInUsername, char *loggedInPassword);
// void displayInfo();
void changeSettings(char *currentUsername, char *currentPassword);
int testMySQLConnection();
void displayAllTables(MYSQL *conn, const char *dbName);
void createTable(MYSQL *conn, const char *dbName);
void renameTable(MYSQL *conn, const char *dbName);
void editTable(MYSQL *conn, const char *dbName);
void deleteTable(MYSQL *conn, const char *dbName);
void displayTableColumns(MYSQL *conn, const char *dbName, const char *tableName);

int createDatabase();
int loadDatabase();

#endif
