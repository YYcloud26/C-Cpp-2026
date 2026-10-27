#include <stdio.h>

int main() {
    int number;
    int is_prime = 1; 

    printf("Enter a number: ");
    scanf("%d", &number);

    if (number <= 1) {
        is_prime = 0;
    } 
    else if (number == 2 || number == 3) {
        is_prime = 1;
    } 
    else if (number % 2 == 0) {
        is_prime = 0;
    } 
    else {
        for (int i = 3; i <= number / i; i += 2) {
            if (number % i == 0) {
                is_prime = 0;
                break;
            }
        }
    }

    if (is_prime) {
        printf("prime\n");
    } else {
        printf("not prime\n");
    }

    return 0;
}
