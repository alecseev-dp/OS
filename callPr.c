#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define N_ITER 100000000  // 100 млн итераций для нагрузки процессора

// Объявление внешних функций
extern int64_t addCalc(int, int*, short, long long, char, uint64_t, int*, int16_t, int);
extern double delCalc(float, double, float, double, float, double, float, double, float, double, float);

int main() {
    int v2 = 5, v7 = 10;
    
    // --- 1. Проверка корректности вычислений ---
    int64_t iRes = addCalc(10, &v2, 3, 40LL, 'A', 100ULL, &v7, 5, 2);
    printf("Result addCalc: %ld\n", iRes);

    double dRes = delCalc(4.0f, 2.0, 1.0f, 3.0, 5.0f, 2.0, 1.0f, 1.0, 2.0f, 3.0, 4.0f);
    printf("Result delCalc (SSE): %lf\n\n", dRes);

    // --- 2.  Замер времени addCalc ---
    printf("Benchmarking addCalc (%d iterations)...\n", N_ITER);
    volatile int64_t dummy_i;
    clock_t start_i = clock();
    for (int i = 0; i < N_ITER; i++) {
        dummy_i = addCalc(10, &v2, 3, 40LL, 'A', 100ULL, &v7, 5, 2);
    }
    clock_t end_i = clock();
    double time_add = (double)(end_i - start_i) / CLOCKS_PER_SEC;
    printf("Execution time addCalc: %.4f seconds\n\n", time_add);

    // --- 3 Замер времени delCalc ---
    printf("Benchmarking delCalc (%d iterations)...\n", N_ITER);
    volatile double dummy_d;
    clock_t start_d = clock();
    for (int i = 0; i < N_ITER; i++) {
        dummy_d = delCalc(4.0f, 2.0, 1.0f, 3.0, 5.0f, 2.0, 1.0f, 1.0, 2.0f, 3.0, 4.0f);
    }
    clock_t end_d = clock();
    double time_del = (double)(end_d - start_d) / CLOCKS_PER_SEC;
    printf("Execution time delCalc: %.4f seconds\n", time_del);

    return 0;
}