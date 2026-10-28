#include "raylib.h"
#include "game.h"
#include "ui.h"

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(800, 800, "Raylib Gomoku - Modularized");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    GameContext ctx;
    bool showPVEHint = false;

    Rectangle btnPVE = { 250, 350, 300, 60 };
    Rectangle btnPVP = { 250, 450, 300, 60 };
    Rectangle btnBack = { 650, 30, 120, 40 };

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        if (ctx.currentState == MENU) {
            HandleMenuInput(ctx, mouse, btnPVE, btnPVP, showPVEHint);
        } else if (ctx.currentState == PVP || ctx.currentState == PVE) {
            HandleGameInput(ctx, mouse, btnBack);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (ctx.currentState == MENU) {
            DrawText("GOMOKU", 280, 150, 60, DARKGRAY);

            bool hoverPVE = CheckCollisionPointRec(mouse, btnPVE);
            DrawRectangleRounded(btnPVE, 0.3f, 10, hoverPVE ? LIGHTGRAY : GRAY);
            const char* textPVE = "Single Player (Coming Soon)";
            DrawText(textPVE, btnPVE.x + (btnPVE.width - MeasureText(textPVE, 20)) / 2, btnPVE.y + 20, 20, hoverPVE ? BLACK : WHITE);

            bool hoverPVP = CheckCollisionPointRec(mouse, btnPVP);
            DrawRectangleRounded(btnPVP, 0.3f, 10, hoverPVP ? LIGHTGRAY : GRAY);
            const char* textPVP = "Two Players";
            DrawText(textPVP, btnPVP.x + (btnPVP.width - MeasureText(textPVP, 20)) / 2, btnPVP.y + 20, 20, hoverPVP ? BLACK : WHITE);

            if (showPVEHint) DrawText("AI mode will be added later!", 270, 550, 20, RED);
        } else {
            DrawBoard();
            DrawPieces(ctx.board);
            DrawPreview(ctx, mouse);
            DrawBackButton(mouse, btnBack);
            DrawGameStatus(ctx);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
