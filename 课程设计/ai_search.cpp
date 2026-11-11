#include "ai_search.h"
#include <cstdlib>

int maxSearch(int board[BOARD_SIZE][BOARD_SIZE], int depth, int alpha, int beta, int aiPlayer, int humanPlayer);

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
    Point result = bestPoints[index];
    result.score = bestVal;
    return result;
}

Point GetDeepeningMove(const int board[BOARD_SIZE][BOARD_SIZE], int aiPlayer, int humanPlayer, int maxDepth) {
    Point bestMove = {-1, -1, MIN_SCORE};

    for (int depth = 2; depth <= maxDepth; depth += 2) {
        Point currentMove = GetBestMove(board, aiPlayer, humanPlayer, depth);
        bestMove = currentMove;

        if (bestMove.score >= SCORE_FOUR) {
            break;
        }
    }
    return bestMove;
}