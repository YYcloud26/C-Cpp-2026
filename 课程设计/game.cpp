#include "game.h"
#include <algorithm>

bool CheckWin(const int board[BOARD_SIZE][BOARD_SIZE], int row, int col, int player) {
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

void ResetGame(GameContext& ctx, GameState nextState) {
    std::fill(&ctx.board[0][0], &ctx.board[0][0] + BOARD_SIZE * BOARD_SIZE, 0);
    ctx.currentPlayer = 1;
    ctx.gameOver = false;
    ctx.winner = 0;
    ctx.currentState = nextState;
}

void PlacePiece(GameContext& ctx, int row, int col) {
    if (ctx.gameOver || row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || ctx.board[row][col] != 0) {
        return;
    }
    ctx.board[row][col] = ctx.currentPlayer;
    if (CheckWin(ctx.board, row, col, ctx.currentPlayer)) {
        ctx.gameOver = true;
        ctx.winner = ctx.currentPlayer;
    } else {
        ctx.currentPlayer = (ctx.currentPlayer == 1) ? 2 : 1;
    }
}