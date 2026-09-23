#include <stdio.h>
#include <chrono>

int main() {
    int number;
    int primes[400];
    primes[0] = 2;
    int count = 0;

    auto start = std::chrono::high_resolution_clock::now();//记录开始时间

    for (number = 3; number < 1000; number += 2) {
        bool flag = true;
        for (int i = 0; primes[i] * primes[i] <= number; ++i)
            if (number % primes[i] == 0) {
                flag = false;
                break;
            }
        if (flag == true) {
            ++count;
            primes[count] = number;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();//记录结束时间

    for (int i = 0; i <= count; ++i) {
        printf("%i ", primes[i]);
    }

    std::chrono::duration<double> sec = end - start;
    printf("\n%f\n", sec.count());

    return 0;
}