#pragma once
#include "raylib.h"
#include "game.h"

void DrawBoard();
void DrawPieces(const int board[BOARD_SIZE][BOARD_SIZE]);
void DrawPreview(const GameContext& ctx, Vector2 mouse);
void DrawBackButton(Vector2 mouse, Rectangle btnBack);
void DrawGameStatus(const GameContext& ctx);

void HandleMenuInput(GameContext& ctx, Vector2 mouse, Rectangle btnPVE, Rectangle btnPVP, bool& showPVEHint);
bool HandleGameInput(GameContext& ctx, Vector2 mouse, Rectangle btnBack);