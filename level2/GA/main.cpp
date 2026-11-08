#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define SIZE       11
#define MAX_PATH   200
#define POP_SIZE   80
#define MAX_GEN    300
#define ELITE      4
#define CX_RATE    0.80
#define MUT_RATE   0.50
#define PATIENCE   60

int sx, sy, ex, ey;

typedef struct {
    int len;
    int r[MAX_PATH];
    int c[MAX_PATH];
} Indiv;

void initMaze(char maze[SIZE][SIZE]) {
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
            if (maze[i][j] == '@') { sx = i; sy = j; }
            else if (maze[i][j] == 'E') { ex = i; ey = j; }
        }
    }
}

static int walkable(char maze[SIZE][SIZE], int r, int c) {
    if (r < 0 || r >= SIZE || c < 0 || c >= SIZE) return 0;
    return maze[r][c] != '#';
}

static int pick_greedy(int nr[], int nc[], int cnt) {
    int best_d = 1 << 30, cand[4], n = 0;
    for (int k = 0; k < cnt; k++) {
        int d = abs(nr[k] - ex) + abs(nc[k] - ey);
        if (d < best_d) { best_d = d; n = 0; cand[n++] = k; }
        else if (d == best_d) cand[n++] = k;
    }
    return cand[rand() % n];
}

static void random_path(char maze[SIZE][SIZE], Indiv *p) {
    int visited[SIZE][SIZE] = {0};
    int r = sx, c = sy;
    p->len = 0;
    p->r[0] = r; p->c[0] = c; p->len = 1;
    visited[r][c] = 1;

    for (int step = 0; step < MAX_PATH - 1; step++) {
        if (r == ex && c == ey) break;
        int nr[4], nc[4], cnt = 0;
        int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
        for (int k = 0; k < 4; k++) {
            int rr = r + dr[k], cc = c + dc[k];
            if (walkable(maze, rr, cc) && !visited[rr][cc]) {
                nr[cnt] = rr; nc[cnt] = cc; cnt++;
            }
        }
        if (cnt == 0) break;
        int pick = (rand() % 100 < 50) ? pick_greedy(nr, nc, cnt) : rand() % cnt;
        r = nr[pick]; c = nc[pick];
        visited[r][c] = 1;
        p->r[p->len] = r; p->c[p->len] = c; p->len++;
    }
}

static int fitness(const Indiv *p) {
    int lr = p->r[p->len - 1], lc = p->c[p->len - 1];
    if (lr == ex && lc == ey) return 10000 - p->len;
    return -p->len - 5 * (abs(lr - ex) + abs(lc - ey));
}

static void copy_indiv(const Indiv *src, Indiv *dst) {
    if (src == dst) return;
    *dst = *src;
}

static int crossover(const Indiv *p1, const Indiv *p2, Indiv *child) {
    int best_i = -1, best_j = -1, cnt = 0;
    for (int i = 1; i < p1->len - 1; i++)
        for (int j = 1; j < p2->len - 1; j++)
            if (p1->r[i] == p2->r[j] && p1->c[i] == p2->c[j])
                if (rand() % (++cnt) == 0) { best_i = i; best_j = j; }

    if (best_i < 0) { copy_indiv(p1, child); return 0; }

    child->len = 0;
    for (int i = 0; i <= best_i; i++) {
        child->r[child->len] = p1->r[i];
        child->c[child->len] = p1->c[i];
        child->len++;
    }
    for (int j = best_j + 1; j < p2->len; j++) {
        child->r[child->len] = p2->r[j];
        child->c[child->len] = p2->c[j];
        child->len++;
    }
    return 1;
}

static void mutate(char maze[SIZE][SIZE], Indiv *p) {
    if (p->len < 3) return;
    int i = 1 + rand() % (p->len - 2);

    int visited[SIZE][SIZE] = {0};
    for (int k = 0; k <= i; k++) visited[p->r[k]][p->c[k]] = 1;

    int r = p->r[i], c = p->c[i];
    Indiv tail; tail.len = 0;
    tail.r[0] = r; tail.c[0] = c; tail.len = 1;

    for (int step = 0; step < 50 && !(r == ex && c == ey); step++) {
        int nr[4], nc[4], cnt = 0;
        int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
        for (int k = 0; k < 4; k++) {
            int rr = r + dr[k], cc = c + dc[k];
            if (walkable(maze, rr, cc) && !visited[rr][cc]) {
                nr[cnt] = rr; nc[cnt] = cc; cnt++;
            }
        }
        if (cnt == 0) break;
        int pick = (rand() % 100 < 60) ? pick_greedy(nr, nc, cnt) : rand() % cnt;
        r = nr[pick]; c = nc[pick];
        visited[r][c] = 1;
        tail.r[tail.len] = r; tail.c[tail.len] = c; tail.len++;
    }

    p->len = i + 1;
    for (int k = 1; k < tail.len && p->len < MAX_PATH; k++) {
        p->r[p->len] = tail.r[k]; p->c[p->len] = tail.c[k]; p->len++;
    }
}

static void remove_loops(Indiv *p) {
    int last[SIZE][SIZE];
    memset(last, -1, sizeof last);
    Indiv out; out.len = 0;

    for (int i = 0; i < p->len; i++) {
        int r = p->r[i], c = p->c[i];
        if (last[r][c] >= 0) {
            out.len = last[r][c] + 1;
            memset(last, -1, sizeof last);
            for (int k = 0; k < out.len; k++) last[out.r[k]][out.c[k]] = k;
        } else {
            last[r][c] = out.len;
            out.r[out.len] = r; out.c[out.len] = c; out.len++;
        }
    }
    copy_indiv(&out, p);
}

static Indiv* tournament(Indiv pop[], int fit[]) {
    int best = rand() % POP_SIZE;
    for (int k = 0; k < 2; k++) {
        int cand = rand() % POP_SIZE;
        if (fit[cand] > fit[best]) best = cand;
    }
    return &pop[best];
}

static Indiv g_pop[POP_SIZE], g_next[POP_SIZE];
static int   g_fit[POP_SIZE];

static int ga_solve(char maze[SIZE][SIZE], Indiv *best_out) {
    for (int i = 0; i < POP_SIZE; i++) random_path(maze, &g_pop[i]);

    int best_fit = -(1 << 30), no_improve = 0;

    for (int gen = 0; gen < MAX_GEN; gen++) {
        for (int i = 0; i < POP_SIZE; i++) g_fit[i] = fitness(&g_pop[i]);

        int bi = 0;
        for (int i = 1; i < POP_SIZE; i++)
            if (g_fit[i] > g_fit[bi]) bi = i;

        if (g_fit[bi] > best_fit) {
            best_fit = g_fit[bi];
            copy_indiv(&g_pop[bi], best_out);
            no_improve = 0;
        } else {
            no_improve++;
        }

        int used[POP_SIZE] = {0};
        for (int k = 0; k < ELITE; k++) {
            int m = -1;
            for (int i = 0; i < POP_SIZE; i++)
                if (!used[i] && (m < 0 || g_fit[i] > g_fit[m])) m = i;
            used[m] = 1;
            copy_indiv(&g_pop[m], &g_next[k]);
        }

        for (int i = ELITE; i < POP_SIZE; i++) {
            Indiv *p1 = tournament(g_pop, g_fit);
            Indiv *p2 = tournament(g_pop, g_fit);
            Indiv child;
            if ((rand() % 1000) / 1000.0 < CX_RATE) crossover(p1, p2, &child);
            else                                    copy_indiv(p1, &child);
            if ((rand() % 1000) / 1000.0 < MUT_RATE) mutate(maze, &child);
            remove_loops(&child);
            copy_indiv(&child, &g_next[i]);
        }

        for (int i = 0; i < POP_SIZE; i++) copy_indiv(&g_next[i], &g_pop[i]);

        if (no_improve >= PATIENCE) break;
    }
    return best_fit;
}

void printMazeWithPath(char maze[SIZE][SIZE], const Indiv *path) {
    char tmp[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            tmp[i][j] = maze[i][j];

    if (path) {
        for (int i = 0; i < path->len; i++) {
            int r = path->r[i], c = path->c[i];
            if (tmp[r][c] == ' ') tmp[r][c] = '*';
        }
        tmp[sx][sy] = 'S';
        tmp[ex][ey] = 'E';
    }

    printf("\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) printf("%c", tmp[i][j]);
        printf("\n");
    }
    printf("\nS = start, E = end, * = path\n");
}

void animatePath(char maze[SIZE][SIZE], const Indiv *path) {
    char tmp[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            tmp[i][j] = maze[i][j];
    tmp[sx][sy] = 'S';
    tmp[ex][ey] = 'E';

    for (int i = 1; i < path->len; i++) {
        int r = path->r[i], c = path->c[i];
        if (tmp[r][c] == ' ') tmp[r][c] = '*';

        system("cls");
        for (int a = 0; a < SIZE; a++) {
            for (int b = 0; b < SIZE; b++) printf("%c", tmp[a][b]);
            printf("\n");
        }
        printf("\nStep %d / %d\n", i, path->len - 1);
        Sleep(100);
    }
}

int main() {
    srand((unsigned)time(NULL));

    char maze[SIZE][SIZE];
    initMaze(maze);

    printf("Maze (level1/p09_maze):\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) printf("%c", maze[i][j]);
        printf("\n");
    }
    printf("\nStart = (%d,%d), End = (%d,%d)\n", sx, sy, ex, ey);
    printf("Press any key to run GA...\n");
    _getch();

    clock_t t0 = clock();

    Indiv best;
    int best_fit = ga_solve(maze, &best);

    clock_t t1 = clock();

    double elapsed_ms = 1000.0 * (double)(t1 - t0) / CLOCKS_PER_SEC;

    int reached = (best.r[best.len - 1] == ex && best.c[best.len - 1] == ey);

    printf("\n=========================================\n");
    printf("GA finished\n");
    printf("  Reached end : %s\n", reached ? "YES" : "NO");
    printf("  Path length : %d  (steps = %d)\n", best.len, best.len - 1);
    printf("  Fitness     : %d\n", best_fit);
    printf("  Time used   : %.3f ms\n", elapsed_ms);
    printf("=========================================\n");

    if (reached) {
        printMazeWithPath(maze, &best);
        printf("\nPress any key to animate the path...\n");
        _getch();
        animatePath(maze, &best);
    }

    printf("\nPress any key to exit...\n");
    _getch();
    return 0;
}