#pragma once
#include <cstdint>
#include "game.h"

struct Zobrist {
    static uint64_t table[BOARD_SIZE][BOARD_SIZE][3];

    static void init();
    static uint64_t hashBoard(const int board[BOARD_SIZE][BOARD_SIZE]);

    static uint64_t go(uint64_t hash, int r, int c, int player);
};