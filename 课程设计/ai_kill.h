#pragma once
#include "ai_eval.h"
#include <vector>
#include <cstdint>

bool GetKillMove(const int board[BOARD_SIZE][BOARD_SIZE], uint64_t hash, int aiPlayer, int humanPlayer, int maxDepth, Point& bestMove);
