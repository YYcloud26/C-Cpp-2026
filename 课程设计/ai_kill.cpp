#include "ai_kill.h"
#include <algorithm>

bool maxKillSearch(int board[BOARD_SIZE][BOARD_SIZE], int role, int depth, int aiPlayer, int humanPlayer, std::vector<Point>& path);

bool minKillSearch(int board[BOARD_SIZE][BOARD_SIZE], int role, int depth, int aiPlayer, int humanPlayer, std::vector<Point>& path);

std::vector<Point> findKillPoints(const int board[BOARD_SIZE][BOARD_SIZE], int role, int scoreThreshold, int aiPlayer, int humanPlayer) {
    std::vector<Point> result;
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (board[r][c] == 0 && hasNeighbor(board, r, c, 1)) {
                if (role == 0) {
                    int s1 = scorePoint(board, r, c, aiPlayer);
                    int s2 = scorePoint(board, r, c, humanPlayer);
                    if (s1 + s2 >= scoreThreshold) {
                        result.push_back({r, c, s1+s2});
                    }
                }else {
                    int s = scorePoint(board, r, c, role);
                    if (s >= scoreThreshold) {
                        result.push_back({r, c, s});
                    }
                }
            }
        }
    }

    std::sort(result.begin(), result.end(), [](const Point& a, const Point& b) {
        return a.score > b.score;
    });
    return result;
}

bool maxKillSearch(int board[BOARD_SIZE][BOARD_SIZE], int role, int depth, int aiPlayer, int humanPlayer, std::vector<Point>& path) {
    if (depth < 0) return false;

    std::vector<Point> points = findKillPoints(board, role, SCORE_FOUR / 2, aiPlayer, humanPlayer);
    if (points.empty()) return false;

    for (const auto& p : points) {
        board[p.row][p.col] = role;

        if (isWin(board, p.row, p.col, role)) {
            board[p.row][p.col] = 0;
            path.push_back(p);
            return true;
        }

        if (minKillSearch(board, role, depth - 1, aiPlayer, humanPlayer, path)) {
            board[p.row][p.col] = 0;
            path.push_back(p);
            return true;
        }
        board[p.row][p.col] = 0;
    }
    return false;
}

bool minKillSearch(int board[BOARD_SIZE][BOARD_SIZE], int role, int depth, int aiPlayer, int humanPlayer, std::vector<Point>& path) {
    if (depth < 0) return false;

    int oppRole = (role == 1) ? 2 : 1;

    std::vector<Point> points = findKillPoints(board, 0, SCORE_FOUR / 2, aiPlayer, humanPlayer);
    if (points.empty()) return false;

    for (const auto& p : points) {
        board[p.row][p.col] = oppRole;

        if (isWin(board, p.row, p.col, oppRole)) {
            board[p.row][p.col] = 0;
            return false;
        }

       bool m = maxKillSearch(board, role, depth - 1, aiPlayer, humanPlayer, path);
        board[p.row][p.col] = 0;

        if (!m) return false;
    }
    return true;
}

bool GetKillMove(const int board[BOARD_SIZE][BOARD_SIZE], int aiPlayer, int humanPlayer, int maxDepth, Point& bestMove) {
    for (int depth = 1; depth <= maxDepth; depth += 2) {
        std::vector<Point> path;
        int tempBoard[BOARD_SIZE][BOARD_SIZE];
        std::copy(&board[0][0], &board[0][0] + BOARD_SIZE * BOARD_SIZE, &tempBoard[0][0]);

        if (maxKillSearch(tempBoard, aiPlayer, depth, aiPlayer, humanPlayer, path)) {
            if (!path.empty()) {
                bestMove = path.back();
                return true;
            }
        }
    }
    return false;
}