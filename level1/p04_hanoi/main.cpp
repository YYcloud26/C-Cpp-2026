#include <stdio.h>

void hanoi(int n, char a, char b, char c) {
    if (n == 1) {
        printf("%c -> %c\n", a, b);
    }else if (n > 1) {
        hanoi(n-1, a, c, b);
        printf("%c -> %c\n", a, b);
        hanoi(n-1, c, b, a);
    }else {
        printf("not a proper number\n");
    }
}

int main() {
    char x = 'A';
    char y = 'B';
    char z = 'C';
    int i;
    printf("enter a number\n");
    scanf("%i", &i);
    hanoi(i, x, y, z);
}