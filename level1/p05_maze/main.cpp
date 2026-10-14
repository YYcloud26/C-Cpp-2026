#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define SIZE 11

// 全局变量，记录玩家和出口位置
int px, py, ex, ey;

void initMaze(char maze[SIZE][SIZE]) {
    // 预定义的迷宫，'#' 为墙，' ' 为通路，'@' 为玩家，'E' 为出口
    const char* mazeStr[SIZE] = {
        "###########",
        "#@        #",
        "# ####### #",
        "# #     # #",
        "# # ### # #",
        "# # #   # #",
        "# # # ### #",
        "#   #     #",
        "######### #",
        "#        E#",
        "###########"
    };

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            maze[i][j] = mazeStr[i][j];
            if (maze[i][j] == '@') {
                px = i;
                py = j;
            } else if (maze[i][j] == 'E') {
                ex = i;
                ey = j;
            }
        }
    }
}

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
    initMaze(maze);
    printMaze(maze);

    while (1) {
        if (_kbhit()) {
            int ch = _getch();

            if (ch == 27)       // esc
                break;

            if (ch == 0 || ch == 224) {
                int key = _getch();
                switch (key) {
                    case 72:        // 上
                        if (px != 0 && maze[px-1][py] != '#') {
                            maze[px][py] = ' ';
                            px -= 1;
                            maze[px][py] = '@';
                            printMaze(maze);
                        }
                        break;
                    case 80:        // 下
                        if (px != SIZE-1 && maze[px+1][py] != '#') {
                            maze[px][py] = ' ';
                            px += 1;
                            maze[px][py] = '@';
                            printMaze(maze);
                        }
                        break;
                    case 75:        // 左
                        if (py != 0 && maze[px][py-1] != '#') {
                            maze[px][py] = ' ';
                            py -= 1;
                            maze[px][py] = '@';
                            printMaze(maze);
                        }
                        break;
                    case 77:        // 右
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
    return 0;
}