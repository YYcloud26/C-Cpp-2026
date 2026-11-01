#include "ai.h"
#include "ui.h"
#include <vector>
#include <cmath>
#include <algorithm>

const int MAX_SCORE = 100000000;
const int MIN_SCORE = -100000000;

const int SCORE_FIVE = 100000;
const int SCORE_FOUR = 10000;
const int SCORE_THREE = 1000;
const int SCORE_TWO = 100;

const int searchDepth = 4;

struct Point {
    int row, col;
};

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

int scorePoint(const int board[BOARD_SIZE][BOARD_SIZE], int r, int c, int player);

std::vector<Point> gen(const int board[BOARD_SIZE][BOARD_SIZE], int depth, int aiPlayer, int humanPlayer) {
    bool hasPiece = false;
    std::vector<Point> fives, foursai, fourshum, twothreesai, twothreeshum;
    std::vector<Point> threesai, threeshum, twosai, twoshum, neighbors, nextNeighbors;

    for (int r = 0; r < BOARD_SIZE; ++r)
        for (int c = 0; c < BOARD_SIZE; ++c)
            if (board[r][c] != 0) hasPiece = true;
    if (!hasPiece) return {{7, 7}};

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (board[r][c] == 0) {
                if (hasNeighbor(board, r, c, 1)) {
                    int scoreAI = scorePoint(board, r, c, aiPlayer);
                    int scoreHum = scorePoint(board, r, c, humanPlayer);

                    if (scoreAI >= SCORE_FIVE) return {{r, c}};
                    if (scoreHum >= SCORE_FIVE) { fives.push_back({r, c}); continue; }

                    if (scoreAI >= SCORE_FOUR) { foursai.push_back({r, c}); continue; }
                    if (scoreHum >= SCORE_FOUR) { fourshum.push_back({r, c}); continue; }

                    if (scoreAI >= 2 * SCORE_THREE) { twothreesai.push_back({r, c}); continue; }
                    if (scoreHum >= 2 * SCORE_THREE) { twothreeshum.push_back({r, c}); continue; }

                    if (scoreAI >= SCORE_THREE) threesai.push_back({r, c});
                    if (scoreHum >= SCORE_THREE) threeshum.push_back({r, c});
                    if (scoreAI >= SCORE_THREE || scoreHum >= SCORE_THREE) continue;

                    if (scoreAI >= SCORE_TWO) twosai.push_back({r, c});
                    if (scoreHum >= SCORE_TWO) twoshum.push_back({r, c});
                    if (scoreAI >= SCORE_TWO || scoreHum >= SCORE_TWO) continue;

                    neighbors.push_back({r, c});
                } else if (depth >= 2 && hasNeighbor(board, r, c, 2)) {
                    nextNeighbors.push_back({r, c});
                }
            }
        }
    }

    if (!fives.empty()) return {fives[0]};
    if (!foursai.empty()) return foursai;
    if (!fourshum.empty()) return fourshum;
    if (!twothreesai.empty()) return twothreesai;
    if (!twothreeshum.empty()) return twothreeshum;

    std::vector<Point> result;
    result.insert(result.end(), threeshum.begin(), threeshum.end());
    result.insert(result.end(), threesai.begin(), threesai.end());
    result.insert(result.end(), twosai.begin(), twosai.end());
    result.insert(result.end(), twoshum.begin(), twoshum.end());
    result.insert(result.end(), neighbors.begin(), neighbors.end());
    result.insert(result.end(), nextNeighbors.begin(), nextNeighbors.end());

    return result;
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

int maxSearch(int board[BOARD_SIZE][BOARD_SIZE], int depth, int alpha, int beta, int aiPlayer, int humanPlayer);

int minSearch(int board[BOARD_SIZE][BOARD_SIZE], int depth, int alpha, int beta, int aiPlayer, int humanPlayer) {
    int val = evaluate(board, aiPlayer);
    if (depth <= 0 || val >= 100000 || val <= -100000) return val;

    std::vector<Point> points = gen(board, depth, aiPlayer, humanPlayer);
    int best = MAX_SCORE;

    for (const auto& p : points) {
        board[p.row][p.col] = humanPlayer;
        int v = maxSearch(board, depth - 1, alpha, beta, aiPlayer, humanPlayer);
        board[p.row][p.col] = 0;

        best = std::min(best, v);
        beta = std::min(beta, best);
        if (beta <= alpha) break;
    }
    return best;
}

int maxSearch(int board[BOARD_SIZE][BOARD_SIZE], int depth, int alpha, int beta, int aiPlayer, int humanPlayer) {
    int val = evaluate(board, aiPlayer);
    if (depth <= 0 || val >= 100000 || val <= -100000) return val;

    std::vector<Point> points = gen(board, depth, aiPlayer, humanPlayer);
    int best = MIN_SCORE;

    for (const auto& p : points) {
        board[p.row][p.col] = aiPlayer;
        int v = minSearch(board, depth - 1, alpha, beta, aiPlayer, humanPlayer);
        board[p.row][p.col] = 0;

        best = std::max(best, v);
        alpha = std::max(alpha, best);
        if (beta <= alpha) break;
    }
    return best;
}

Point GetBestMove(const int board[BOARD_SIZE][BOARD_SIZE], int aiPlayer, int humanPlayer, int depth) {
    std::vector<Point> points = gen(board, depth, aiPlayer, humanPlayer);
    if (points.empty()) return {-1, -1};
    if (points.size() == 1) return points[0];

    int bestVal = MIN_SCORE;
    std::vector<Point> bestPoints;

    int tempBoard[BOARD_SIZE][BOARD_SIZE];
    std::copy(&board[0][0], &board[0][0] + BOARD_SIZE * BOARD_SIZE, &tempBoard[0][0]);

    for (const auto& p : points) {
        tempBoard[p.row][p.col] = aiPlayer;
        int v = minSearch(tempBoard, depth - 1, MIN_SCORE, MAX_SCORE, aiPlayer, humanPlayer);
        tempBoard[p.row][p.col] = 0;

        if (v > bestVal) {
            bestVal = v;
            bestPoints.clear();
            bestPoints.push_back(p);
        } else if (v == bestVal) {
            bestPoints.push_back(p);
        }
    }

    int index = rand() % bestPoints.size();
    return bestPoints[index];
}

void AIMove(GameContext& ctx) {

    Point best = GetBestMove(ctx.board, 2, 1, searchDepth);
    if (best.row != -1) {
        PlacePiece(ctx, best.row, best.col);
    }
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