#include <stdio.h>

int main() {
    int number;
    int count = 3;

    printf("Enter a number: ");
    scanf("%i", &number);

    if (number == 2 || number ==3)
        printf("prime");
    else if (number < 2)
        printf("cannot be sorted");
    else {
        for (int i = 3; i*i <= number; i += 2) {
            if (number % i == 0) {
                printf("not prime");
                break;
            }
            count += 2;
        }
        if ((count + 1)*(count + 1) > number)
            printf("prime");
    }
    return 0;
}