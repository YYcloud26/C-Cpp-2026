#include <stdio.h> //调用putchar()
#include <stdlib.h> //调用system("cls")
#include <conio.h>
#include <windows.h>
/*调用GetStdHandle(STD_OUTPUT_HANDLE)
GetConsoleScreenBufferInfo()
SetConsoleCursorPosition()
SetConsoleCursorInfo()
Sleep()*/

void getConsoleSize(int *w, int *h) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    *w = info.srWindow.Right - info.srWindow.Left + 1;
    *h = info.srWindow.Bottom - info.srWindow.Top + 1;
}

void gotoxy(int x, int y) {
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void showCursor() {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

int main() {
    int W, H;
    int x, y;
    int ox, oy;
    int dx, dy;
    char ch = 'A';

    getConsoleSize(&W, &H);
    showCursor();

    x = 1;
    y = 1;
    dx = 1;
    dy = 1;

    system("cls");
    while (!_kbhit()) {
        ox = x;
        oy = y;

        if (x + dx < 1 || x + dx > W - 2)
            dx = -dx;
        if (y + dy < 1 || y + dy > H - 2)
            dy = -dy;
        x += dx;
        y += dy;

        gotoxy(x, y);
        putchar(ch);

        gotoxy(ox, oy);
        putchar(' ');

        Sleep(200);
    }
}