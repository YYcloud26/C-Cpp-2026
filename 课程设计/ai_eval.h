#pragma once
#include "ai.h"

const int MAX_SCORE = 100000000;
const int MIN_SCORE = -100000000;

const int SCORE_FIVE = 100000;
const int SCORE_FOUR = 10000;
const int SCORE_THREE = 1000;
const int SCORE_TWO = 100;

bool isWin(const int board[BOARD_SIZE][BOARD_SIZE], int row, int col, int player);
bool hasNeighbor(const int board[BOARD_SIZE][BOARD_SIZE], int r, int c, int dist);
int evaluateLine(int count, int block);
int scorePoint(const int board[BOARD_SIZE][BOARD_SIZE], int r, int c, int player);
int evaluate(const int board[BOARD_SIZE][BOARD_SIZE], int aiPlayer);