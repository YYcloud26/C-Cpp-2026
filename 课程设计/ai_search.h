#pragma once
#include "ai_eval.h"
#include <vector>

std::vector<Point> gen(const int board[BOARD_SIZE][BOARD_SIZE], int depth, int aiPlayer, int humanPlayer);
Point GetDeepeningMove(const int board[BOARD_SIZE][BOARD_SIZE], int aiPLayer, int humanPlayer, int maxDepth);