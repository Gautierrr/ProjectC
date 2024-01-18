@echo off
gcc -I sdl/include -L sdl/lib -o main main.c src/authentificateUser.c src/changeSettings.c src/createAccount.c src/createDatabase.c src/createTable.c src/databaseMenu.c src/deleteTable.c src/displayAllTables.c src/displayTableColumns.c src/editTable.c src/loadDatabase.c src/mainMenu.c src/renameTable.c src/renderText.c src/testMySQLConnection.c src/viewMcd.c -I/mysql/include -L/mysql/lib libmysql.lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

