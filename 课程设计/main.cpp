#include "raylib.h"
#include <cmath>
#include <algorithm>

const int BOARD_SIZE = 15;
const int CELL_SIZE = 40;
const int OFFSET_X = 100;
const int OFFSET_Y = 100;

const int BOARD_PIXELS = CELL_SIZE * (BOARD_SIZE - 1);

void DrawBoard() {
    int margin = CELL_SIZE / 2;
    DrawRectangle(OFFSET_X - margin, OFFSET_Y - margin,
                  BOARD_PIXELS + CELL_SIZE, BOARD_PIXELS + CELL_SIZE, BEIGE);

    for (int i = 0; i < BOARD_SIZE; ++i) {
        int pos = i * CELL_SIZE;

        DrawLine(OFFSET_X, OFFSET_Y + pos,
                 OFFSET_X + BOARD_PIXELS, OFFSET_Y + pos, DARKGRAY);

        DrawLine(OFFSET_X + pos, OFFSET_Y,
                 OFFSET_X + pos, OFFSET_Y + BOARD_PIXELS, DARKGRAY);
    }

    int starPoints[5][2] = {{3,3}, {11,3}, {7,7}, {3,11}, {11,11}};
    for (auto& point : starPoints) {
        int row = point[0];
        int col = point[1];
        int x = OFFSET_X + col * CELL_SIZE;
        int y = OFFSET_Y + row * CELL_SIZE;
        DrawCircle(x, y, 4.0f, DARKGRAY);
    }
}

void DrawPieces(int board[BOARD_SIZE][BOARD_SIZE]) {
    float radius = CELL_SIZE * 0.4f;

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (board[row][col] == 0) continue;

            float centerX = OFFSET_X + col * CELL_SIZE;
            float centerY = OFFSET_Y + row * CELL_SIZE;

            if (board[row][col] == 1) {
                DrawCircle(centerX, centerY, radius, BLACK);
                DrawCircle(centerX - radius * 0.3f, centerY - radius * 0.3f, radius * 0.2f, LIGHTGRAY);
            }
            else if (board[row][col] == 2) {
                DrawCircle(centerX, centerY, radius, WHITE);
                DrawCircleLines(centerX, centerY, radius, DARKGRAY);
            }
        }
    }
}

bool CheckWin(int board[BOARD_SIZE][BOARD_SIZE], int row, int col, int player) {
    int dirs[4][2] = {
        {0, 1},
        {1, 0},
        {1, 1},
        {1, -1}
    };

    for (int i = 0; i < 4; ++i) {
        int count = 1;

        for (int step = 1; step <= 4; ++step) {
            int nr = row + dirs[i][0] * step;
            int nc = col + dirs[i][1] * step;
            if (nr < 0 || nr >= BOARD_SIZE || nc < 0 || nc >= BOARD_SIZE || board[nr][nc] != player) {
                break;
            }
            count++;
        }

        for (int step = 1; step <= 4; ++step) {
            int nr = row - dirs[i][0] * step;
            int nc = col - dirs[i][1] * step;
            if (nr < 0 || nr >= BOARD_SIZE || nc < 0 || nc >= BOARD_SIZE || board[nr][nc] != player) {
                break;
            }
            count++;
        }

        if (count >= 5) {
            return true;
        }
    }
    return false;
}

int main() {

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(800, 800, "Raylib Gomoku");
    SetTargetFPS(60);

    int board[BOARD_SIZE][BOARD_SIZE] = {0};
    int currentPlayer = 1;
    bool gameOver = false;
    int winner = 0;

    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (!gameOver && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mouse = GetMousePosition();
                int col = std::round((mouse.x - OFFSET_X) / CELL_SIZE);
                int row = std::round((mouse.y - OFFSET_Y) / CELL_SIZE);

                if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
                    if (board[row][col] == 0) {
                        board[row][col] = currentPlayer;

                        if (CheckWin(board, row, col, currentPlayer)) {
                            gameOver = true;
                            winner = currentPlayer;
                        } else {
                            currentPlayer = (currentPlayer == 1) ? 2 : 1;
                        }
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawBoard();
        DrawPieces(board);

        if (!WindowShouldClose()) {
            Vector2 mouse = GetMousePosition();
            int previewCol = std::round((mouse.x - OFFSET_X) / CELL_SIZE);
            int previewRow = std::round((mouse.y - OFFSET_Y) / CELL_SIZE);

            if (previewRow >= 0 && previewRow < BOARD_SIZE && previewCol >= 0 && previewCol < BOARD_SIZE) {
                if (board[previewRow][previewCol] == 0) {
                    Vector2 previewPos = {
                        (float)(OFFSET_X + previewCol * CELL_SIZE),
                        (float)(OFFSET_Y + previewRow * CELL_SIZE)
                    };
                    Color previewColor = (currentPlayer == 1) ? Fade(BLACK, 0.3f) : Fade(WHITE, 0.5f);
                    DrawCircleV(previewPos, CELL_SIZE * 0.4f, previewColor);
                }
            }
        }

        if (gameOver) {
            const char* resultText = (winner == 1) ? "Black Wins!" : "White Wins!";
            DrawText(resultText, 300, 30, 40, RED);

            DrawText("Press R to Restart", 280, 80, 20, DARKGRAY);
        } else {
            const char* turnText = (currentPlayer == 1) ? "Black's Turn" : "White's Turn";
            DrawText(turnText, 320, 30, 30, DARKGRAY);
        }

        if (gameOver && IsKeyPressed(KEY_R)) {
            for (int r = 0; r < BOARD_SIZE; ++r) {
                for (int c = 0; c < BOARD_SIZE; ++c) {
                    board[r][c] = 0;
                }
            }
            currentPlayer = 1;
            gameOver = false;
            winner = 0;
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
