#include "zobrist.h"
#include <random>

uint64_t Zobrist::table[BOARD_SIZE][BOARD_SIZE][3];

void Zobrist::init() {
    std::mt19937_64 rng(123456789);
    std::uniform_int_distribution<uint64_t> dist;

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            table[r][c][1] = dist(rng);
            table[r][c][2] = dist(rng);
        }
    }
}

uint64_t Zobrist::hashBoard(const int board[BOARD_SIZE][BOARD_SIZE]) {
    uint64_t hash = 0;
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (board[r][c] != 0) {
                hash ^= table[r][c][board[r][c]];
            }
        }
    }
    return hash;
}

uint64_t Zobrist::go(uint64_t hash, int r, int c, int player) {
    return hash ^ table[r][c][player];
}