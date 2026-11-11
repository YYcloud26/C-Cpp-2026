#pragma once
#include "game.h"

struct Point {
    int row, col;
    int score = 0;
};

void AIMove(GameContext& ctx);
