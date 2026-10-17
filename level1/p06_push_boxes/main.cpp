#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#define MAX 32
#define MAX_LEVEL 5

int px, py;                 // 玩家位置
int tx[MAX * MAX], ty[MAX * MAX]; // 目标位置
int numTargets;             // 目标数量
int steps;                  // 当前关卡步数

void initMap(char map[][MAX], int rows, int cols) {
    numTargets = 0;
    px = py = -1;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            char c = map[i][j];

            if (c == '@' || c == '+') {
                px = i;
                py = j;
                if (c == '+') {
                    tx[numTargets] = i;
                    ty[numTargets] = j;
                    numTargets++;
                }
            } else if (c == '.') {
                tx[numTargets] = i;
                ty[numTargets] = j;
                numTargets++;
            } else if (c == '*') {
                tx[numTargets] = i;
                ty[numTargets] = j;
                numTargets++;
            }
        }
    }
}

void printMap(char map[][MAX], int rows, int cols, int level, int steps) {
    system("cls");
    printf("Level %d    Steps: %d\n\n", level, steps);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            putchar(map[i][j]);
        }
        putchar('\n');
    }

    printf("\nUse arrow keys or WASD to move.\n");
    printf("Push all boxes ($) onto targets (.).\n");
    printf("Press ESC to quit.\n");
}

int movePlayer(char map[][MAX], int rows, int cols, int dx, int dy) {
    int nx = px + dx;
    int ny = py + dy;

    if (nx < 0 || nx >= rows || ny < 0 || ny >= cols)
        return 0;

    if (map[nx][ny] == '#')
        return 0;

    char dest = map[nx][ny];

    // 前方是箱子：尝试推箱子
    if (dest == '$' || dest == '*') {
        int nnx = nx + dx;
        int nny = ny + dy;

        if (nnx < 0 || nnx >= rows || nny < 0 || nny >= cols)
            return 0;

        char beyond = map[nnx][nny];

        if (beyond == '#' || beyond == '$' || beyond == '*')
            return 0;

        // 箱子推到新位置
        if (beyond == '.') {
            map[nnx][nny] = '*';
        } else {
            map[nnx][nny] = '$';
        }

        // 玩家离开原位置
        char old = map[px][py];
        map[px][py] = (old == '+') ? '.' : ' ';

        // 玩家进入箱子原来的位置
        map[nx][ny] = (dest == '*') ? '+' : '@';

        px = nx;
        py = ny;
        steps++;
        return 1;
    }

    // 普通移动
    char old = map[px][py];
    map[px][py] = (old == '+') ? '.' : ' ';

    if (dest == '.') {
        map[nx][ny] = '+';
    } else {
        map[nx][ny] = '@';
    }

    px = nx;
    py = ny;
    steps++;
    return 1;
}

int checkWin(char map[][MAX]) {
    if (numTargets <= 0)
        return 0;

    for (int i = 0; i < numTargets; i++) {
        if (map[tx[i]][ty[i]] != '*')
            return 0;
    }
    return 1;
}

void saveScore(int level, int steps) {
    FILE *fp = fopen("scores.txt", "a");
    if (fp == NULL)
        return;

    int score = 1000 - steps;
    if (score < 0)
        score = 0;

    fprintf(fp, "Level %d: steps = %d, score = %d\n", level, steps, score);
    fclose(fp);
}

int loadLevel(char map[][MAX], int level, int *rows, int *cols) {
    char filename[64];
    sprintf(filename, "levels/level%d.txt", level);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Cannot open %s\n", filename);
        return 0;
    }

    memset(map[0], ' ', MAX * MAX);

    char header[256];
    if (fgets(header, sizeof(header), fp) == NULL) {
        fclose(fp);
        return 0;
    }

    int r, c, num;
    if (sscanf(header, "%d %d %d", &r, &c, &num) < 2) {
        fclose(fp);
        return 0;
    }
    (void)num;

    if (r > MAX)
        r = MAX;
    if (c > MAX)
        c = MAX;

    int maxCols = c;
    char line[256];

    for (int i = 0; i < r; i++) {
        if (fgets(line, sizeof(line), fp) == NULL)
            break;

        line[strcspn(line, "\r\n")] = '\0';
        int len = (int)strlen(line);
        if (len > MAX)
            len = MAX;

        if (len > maxCols)
            maxCols = len;

        for (int j = 0; j < len; j++) {
            map[i][j] = line[j];
        }
    }

    fclose(fp);

    *rows = r;
    *cols = maxCols;
    return 1;
}

int main() {
    char map[MAX][MAX];
    int level = 1;
    int quit = 0;

    while (!quit && level <= MAX_LEVEL) {
        int rows, cols;

        if (!loadLevel(map, level, &rows, &cols)) {
            break;
        }

        steps = 0;
        initMap(map, rows, cols);
        printMap(map, rows, cols, level, steps);

        int won = 0;

        while (1) {
            if (_kbhit()) {
                int ch = _getch();

                if (ch == 27) { // ESC
                    quit = 1;
                    break;
                }

                int dx = 0, dy = 0;

                if (ch == 0 || ch == 224) {
                    int key = _getch();
                    switch (key) {
                        case 72: dx = -1; break; // 上
                        case 80: dx = 1;  break; // 下
                        case 75: dy = -1; break; // 左
                        case 77: dy = 1;  break; // 右
                    }
                } else {
                    switch (ch) {
                        case 'w':
                        case 'W': dx = -1; break;
                        case 's':
                        case 'S': dx = 1;  break;
                        case 'a':
                        case 'A': dy = -1; break;
                        case 'd':
                        case 'D': dy = 1;  break;
                    }
                }

                if (dx != 0 || dy != 0) {
                    if (movePlayer(map, rows, cols, dx, dy)) {
                        printMap(map, rows, cols, level, steps);

                        if (checkWin(map)) {
                            won = 1;
                            break;
                        }
                    }
                }
            }

            Sleep(10);
        }

        if (quit)
            break;

        if (won) {
            int score = 1000 - steps;
            if (score < 0)
                score = 0;

            printf("\nYou win! Level %d cleared.\n", level);
            printf("Steps: %d    Score: %d\n", steps, score);

            saveScore(level, steps);

            if (level < MAX_LEVEL) {
                printf("Go to next level in 5 seconds...\n");
                Sleep(5000);
                level++;
            } else {
                printf("All levels completed! Quit in 5 seconds...\n");
                Sleep(5000);
                break;
            }
        }
    }

    printf("Game over.\n");
    return 0;
}