#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define SIZE 10

void printMaze(char maze[SIZE][SIZE]) {
    system("cls");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++)
            printf("%c", maze[i][j]);
        printf("\n");
    }
    printf("use up, down, left and right to move\n");
    printf("use esc to quit\n");
}

int main() {
    char maze[SIZE][SIZE];
    int px, py, ex, ey;

    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            maze[i][j] = '#';

    srand((unsigned)time(NULL));
    px = rand() % SIZE;
    py = rand() % SIZE;
    maze[px][py] = '@';

    do {
        ex = rand() % SIZE;
        ey = rand() % SIZE;
    }while (ex == px && ey == py);
    maze[ex][ey] = 'E';

    int dx;
    int dy;
    if (px < ex)
        dx = 1;
    else
        dx = -1;
    if (py < ey)
        dy = 1;
    else
        dy = -1;

    int curx = px;
    int cury = py;
    do {
        if (curx != ex && cury != ey) {
            int r = rand() % 2;
            if (r == 0)
                curx += dx;
            else
                cury += dy;
        }else if (curx != ex)
            curx += dx;
        else
            cury += dy;
        if (curx != ex || cury != ey)
            maze[curx][cury] = ' ';
    }while (curx != ex || cury != ey);

    for (int i = 0; i < SIZE*SIZE/2; ++i){
        curx = rand() % SIZE;
        cury = rand() % SIZE;
        if (maze[curx][cury] != 'E' && maze[curx][cury] != '@')
            maze[curx][cury] = ' ';
    }

    printMaze(maze);//生成初始迷宫


    while (1) {
        if (_kbhit()) {
            int ch = _getch();

            if (ch == 27)       //esc
                break;

            if (ch == 0 || ch == 224) {
                int key = _getch();
                switch (key) {
                    case 72:        //上
                        if (px != 0 && maze[px-1][py] != '#') {
                            maze[px][py] = ' ';
                            px -= 1;
                            maze[px][py] = '@';
                            printMaze(maze);
                        }
                        break;
                    case 80:        //下
                        if (px != SIZE-1 && maze[px+1][py] != '#') {
                            maze[px][py] = ' ';
                            px += 1;
                            maze[px][py] = '@';
                            printMaze(maze);
                        }
                        break;
                    case 75:        //左
                        if (py != 0 && maze[px][py-1] != '#') {
                            maze[px][py] = ' ';
                            py -= 1;
                            maze[px][py] = '@';
                            printMaze(maze);
                        }
                        break;
                    case 77:        //右
                        if (py != SIZE-1 && maze[px][py+1] != '#') {
                            maze[px][py] = ' ';
                            py += 1;
                            maze[px][py] = '@';
                            printMaze(maze);
                        }
                        break;
                }
            }
        }

        if (px == ex && py == ey) {
            printf("You win!\n");
            printf("quit automatically in 5 seconds\n");
            Sleep(5000);
            break;
        }
        Sleep(10);
    }
}