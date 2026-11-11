#include "ai.h"
#include "ui.h"
#include "ai_search.h"
#include "ai_kill.h"
#include "zobrist.h"

const int searchDepth = 4;
const int killSearchDepth = 10;

void AIMove(GameContext& ctx) {
    Point best = {-1,-1};

    uint64_t currentHash = Zobrist::hashBoard(ctx.board);
    bool hasKill = GetKillMove(ctx.board, currentHash, 2, 1, killSearchDepth, best);

    if (hasKill) {
        PlacePiece(ctx, best.row, best.col);
        return;
    }

    best = GetDeepeningMove(ctx.board, 2, 1, searchDepth);
    if (best.row != -1) {
        PlacePiece(ctx, best.row, best.col);
    }
}
