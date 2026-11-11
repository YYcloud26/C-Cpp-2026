#include "ai.h"
#include "ui.h"
#include "ai_search.h"
#include "ai_kill.h"

const int searchDepth = 4;
const int killSearchDepth = 10;

void AIMove(GameContext& ctx) {
    Point best = {-1,-1};

    bool hasKill = GetKillMove(ctx.board, 2, 1, killSearchDepth, best);

    if (hasKill) {
        PlacePiece(ctx, best.row, best.col);
        return;
    }

    best = GetDeepeningMove(ctx.board, 2, 1, searchDepth);
    if (best.row != -1) {
        PlacePiece(ctx, best.row, best.col);
    }
}
