#include "ai_eval.h"
#include <algorithm>

bool isWin(const int board[BOARD_SIZE][BOARD_SIZE], int row, int col, int player) {
    int dirs[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
    for (int i = 0; i < 4; ++i) {
        int count = 1;
        for (int step = 1; step <= 4; ++step) {
            int nr = row + dirs[i][0] * step, nc = col + dirs[i][1] * step;
            if (nr < 0 || nr >= BOARD_SIZE || nc < 0 || nc >= BOARD_SIZE || board[nr][nc] != player) break;
            count++;
        }
        for (int step = 1; step <= 4; ++step) {
            int nr = row - dirs[i][0] * step, nc = col - dirs[i][1] * step;
            if (nr < 0 || nr >= BOARD_SIZE || nc < 0 || nc >= BOARD_SIZE || board[nr][nc] != player) break;
            count++;
        }
        if (count >= 5) return true;
    }
    return false;
}

bool hasNeighbor(const int board[BOARD_SIZE][BOARD_SIZE], int r, int c, int dist) {
    for (int i = -dist; i <= dist; ++i) {
        for (int j = -dist; j <= dist; ++j) {
            if (i == 0 && j == 0) continue;
            int nr = r + i, nc = c + j;
            if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE && board[nr][nc] != 0) {
                return true;
            }
        }
    }
    return false;
}

int evaluateLine(int count, int block) {
    if (count >= 5) return SCORE_FIVE;

    if (block == 2) return 0;

    if (block == 1) {
        if (count == 4) return SCORE_FOUR / 2;
        if (count == 3) return SCORE_THREE / 3;
        if (count == 2) return SCORE_TWO / 3;
    }

    if (block == 0) {
        if (count == 4) return SCORE_FOUR;
        if (count == 3) return SCORE_THREE;
        if (count == 2) return SCORE_TWO;
    }

    return 1;
}

int scorePoint(const int board[BOARD_SIZE][BOARD_SIZE], int r, int c, int player) {
    int totalScore = 0;

    int dirs[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

    for (int i = 0; i < 4; ++i) {
        int count = 1;
        int block = 0;

        for (int step = 1; step <= 4; ++step) {
            int nr = r + dirs[i][0] * step;
            int nc = c + dirs[i][1] * step;

            if (nr < 0 || nr >= BOARD_SIZE || nc < 0 || nc >= BOARD_SIZE) {
                block++;
                break;
            }
            if (board[nr][nc] == player) {
                count++;
            } else if (board[nr][nc] == 0) {
                break;
            } else {
                block++;
                break;
            }
        }

        for (int step = 1; step <= 4; ++step) {
            int nr = r - dirs[i][0] * step;
            int nc = c - dirs[i][1] * step;
            if (nr < 0 || nr >= BOARD_SIZE || nc < 0 || nc >= BOARD_SIZE) {
                block++;
                break;
            }
            if (board[nr][nc] == player) {
                count++;
            } else if (board[nr][nc] == 0) {
                break;
            } else {
                block++;
                break;
            }
        }

        totalScore += evaluateLine(count, block);
    }

    return totalScore;
}

int evaluate(const int board[BOARD_SIZE][BOARD_SIZE], int aiPlayer) {
    int aiScore = 0, humanScore = 0;
    int dirs[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (board[r][c] == 0) continue;

            int player = board[r][c];
            int score = 0;

            for (int i = 0; i < 4; ++i) {
                int pr = r - dirs[i][0], pc = c - dirs[i][1];
                if (pr >= 0 && pr < BOARD_SIZE && pc >= 0 && pc < BOARD_SIZE && board[pr][pc] == player) {
                    continue;
                }

                int count = 1, block = 0;
                int nr = r + dirs[i][0], nc = c + dirs[i][1];
                while (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE && board[nr][nc] == player) {
                    count++; nr += dirs[i][0]; nc += dirs[i][1];
                }
                if (nr < 0 || nr >= BOARD_SIZE || nc < 0 || nc >= BOARD_SIZE || board[nr][nc] != 0) block++;

                score += evaluateLine(count, block);
            }

            if (player == aiPlayer) aiScore += score;
            else humanScore += score;
        }
    }
    return aiScore - humanScore * 2;
}