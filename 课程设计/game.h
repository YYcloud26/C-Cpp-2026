#pragma once

const int BOARD_SIZE = 15;
const int CELL_SIZE = 40;
const int OFFSET_X = 100;
const int OFFSET_Y = 100;
const int BOARD_PIXELS = CELL_SIZE * (BOARD_SIZE - 1);

enum GameState {
    MENU,
    PVP,
    PVE
};

struct GameContext {
    int board[BOARD_SIZE][BOARD_SIZE] = {0};
    int currentPlayer = 1;
    bool gameOver = false;
    int winner = 0;
    GameState currentState = MENU;
};

bool CheckWin(const int board[BOARD_SIZE][BOARD_SIZE], int row, int col, int player);
void ResetGame(GameContext& ctx, GameState nextState);
void PlacePiece(GameContext& ctx, int row, int col);