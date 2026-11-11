#pragma once
#include "ai_eval.h"
#include <vector>

bool GetKillMove(const int board[BOARD_SIZE][BOARD_SIZE], int aiPlayer, int humanPlayer, int maxDepth, Point& bestMove);