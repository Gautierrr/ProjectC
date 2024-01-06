#include "mysql/include/mysql.h"

#include <stdio.h>
#include <string.h>
#include <conio.h>

// Déclarations en amont ou prototypes de fonction
void createAccount();
int authenticateUser(int *connect, char *loggedInUsername, char *loggedInPassword);
int mainMenu(char *loggedInUsername, char *loggedInPassword);
// void displayInfo();
void changeSettings(char *currentUsername, char *currentPassword);
int testMySQLConnection();

typedef struct Student
{
    char studentUsername[50];
    char studentPassword[20];
} Student;

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
        case '4':
            testMySQLConnection();
            break;
        /*case '4':
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

            // Stocke les informations d'authentification
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
        printf("\n\t\t\t0. Sign out");

        printf("\n\n\n\t\t\tEnter Your Option: ");
        scanf(" %c", &option);
        getchar();

        switch (option)
        {
        case '1':
            //createDatabase();
            break;
        case '2':
            //loadDatabase();
            break;
        case '3':
            changeSettings(loggedInUsername, loggedInPassword);
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
        // The user is found, allow modification
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
    // Initialiser la connexion MySQL
    MYSQL *conn = mysql_init(NULL);

    // Établir la connexion à la base de données
    if (mysql_real_connect(conn, "localhost", "root", "root", "projetC", 3306, NULL, 0)) {
        printf("Connexion a la base de donnees reussie\n");
        printf("\n\n\t\t\tEnter any keys to continue.......");
        getch();

        // Exécuter des requêtes SQL ici

        // Fermer la connexion MySQL
        mysql_close(conn);
    } else {
        fprintf(stderr, "Echec de la connexion a la base de donnees: %s\n", mysql_error(conn));
        printf("\n\n\t\t\tEnter any keys to continue.......");
        getch();
    }

    return 0;
}




































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
