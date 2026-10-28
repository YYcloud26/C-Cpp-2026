#include "ui.h"
#include <cmath>

void DrawBoard() {
    int margin = CELL_SIZE / 2;
    DrawRectangle(OFFSET_X - margin, OFFSET_Y - margin, BOARD_PIXELS + CELL_SIZE, BOARD_PIXELS + CELL_SIZE, BEIGE);

    for (int i = 0; i < BOARD_SIZE; ++i) {
        int pos = i * CELL_SIZE;
        DrawLine(OFFSET_X, OFFSET_Y + pos, OFFSET_X + BOARD_PIXELS, OFFSET_Y + pos, DARKGRAY);
        DrawLine(OFFSET_X + pos, OFFSET_Y, OFFSET_X + pos, OFFSET_Y + BOARD_PIXELS, DARKGRAY);
    }

    int starPoints[5][2] = {{3,3}, {11,3}, {7,7}, {3,11}, {11,11}};
    for (auto& point : starPoints) {
        DrawCircle(OFFSET_X + point[1] * CELL_SIZE, OFFSET_Y + point[0] * CELL_SIZE, 4.0f, DARKGRAY);
    }
}

void DrawPieces(const int board[BOARD_SIZE][BOARD_SIZE]) {
    float radius = CELL_SIZE * 0.4f;
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (board[row][col] == 0) continue;
            float centerX = OFFSET_X + col * CELL_SIZE;
            float centerY = OFFSET_Y + row * CELL_SIZE;
            if (board[row][col] == 1) {
                DrawCircle(centerX, centerY, radius, BLACK);
                DrawCircle(centerX - radius * 0.3f, centerY - radius * 0.3f, radius * 0.2f, LIGHTGRAY);
            } else if (board[row][col] == 2) {
                DrawCircle(centerX, centerY, radius, WHITE);
                DrawCircleLines(centerX, centerY, radius, DARKGRAY);
            }
        }
    }
}

void DrawPreview(const GameContext& ctx, Vector2 mouse) {
    if (ctx.gameOver) return;
    int col = std::round((mouse.x - OFFSET_X) / CELL_SIZE);
    int row = std::round((mouse.y - OFFSET_Y) / CELL_SIZE);
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && ctx.board[row][col] == 0) {
        Vector2 pos = { (float)(OFFSET_X + col * CELL_SIZE), (float)(OFFSET_Y + row * CELL_SIZE) };
        Color color = (ctx.currentPlayer == 1) ? Fade(BLACK, 0.3f) : Fade(WHITE, 0.5f);
        DrawCircleV(pos, CELL_SIZE * 0.4f, color);
    }
}

void DrawBackButton(Vector2 mouse, Rectangle btnBack) {
    bool hover = CheckCollisionPointRec(mouse, btnBack);
    DrawRectangleRounded(btnBack, 0.3f, 10, hover ? LIGHTGRAY : GRAY);
    const char* text = "Menu (ESC)";
    int textWidth = MeasureText(text, 18);
    DrawText(text, btnBack.x + (btnBack.width - textWidth) / 2, btnBack.y + 10, 18, hover ? BLACK : WHITE);
}

void DrawGameStatus(const GameContext& ctx) {
    if (ctx.gameOver) {
        const char* resultText = (ctx.winner == 1) ? "Black Wins!" : "White Wins!";
        DrawText(resultText, 300, 30, 40, RED);
        DrawText("Press R to Restart", 280, 80, 20, DARKGRAY);
    } else {
        const char* turnText = (ctx.currentPlayer == 1) ? "Black's Turn" : "White's Turn";
        DrawText(turnText, 320, 30, 30, DARKGRAY);
    }
}

void HandleMenuInput(GameContext& ctx, Vector2 mouse, Rectangle btnPVE, Rectangle btnPVP, bool& showPVEHint) {
    if (CheckCollisionPointRec(mouse, btnPVE) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        showPVEHint = true;
    }
    if (CheckCollisionPointRec(mouse, btnPVP) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        ResetGame(ctx, PVP);
    }
}

bool HandleGameInput(GameContext& ctx, Vector2 mouse, Rectangle btnBack) {
    if (IsKeyPressed(KEY_ESCAPE) || (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) {
        ctx.currentState = MENU;
        return true;
    }
    if (ctx.gameOver && IsKeyPressed(KEY_R)) {
        ResetGame(ctx, ctx.currentState);
        return true;
    }
    if (!ctx.gameOver && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int col = std::round((mouse.x - OFFSET_X) / CELL_SIZE);
        int row = std::round((mouse.y - OFFSET_Y) / CELL_SIZE);
        PlacePiece(ctx, row, col);
    }
    return false;
}