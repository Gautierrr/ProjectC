@echo off
gcc -I sdl/include -L sdl/lib -o main main.c -I/mysql/include -L/mysql/lib libmysql.lib -lmingw32 -lSDL2main -lSDL2

gcc -o main main.c -I/sdl/include -L/sdl/lib -lmingw32 -lSDL2 -lSDL2main -I/mysql/include -L/mysql/lib libmysql.lib
