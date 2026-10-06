#include <stdio.h>
#include <stdint.h>
#include <math.h>

// Целочисленная функция
int64_t addCalc(int a1, int *a2, short a3, long long a4, char a5,
               uint64_t a6, int *a7, int16_t a8, int a9) {
    int val_a2 = a2 ? *a2 : 1;
    int val_a7 = a7 ? *a7 : 1;

    int64_t part_regs = ((int64_t)a1 * val_a2) - (a3 + a4) + ((int64_t)a5 ^ a6);
    int64_t part_stack = (int64_t)val_a7 + a8 - a9;

    return part_regs * part_stack;
}

// Дробная функция
double delCalc(float f1, double d2, float f3, double d4, float f5,
              double d6, float f7, double d8, float f9, double d10, float f11) {

    double num = ((double)f1 * d2) - ((double)f3 * d4) + ((double)f5 * d6) - ((double)f7 * d8);
    if (num < 0) num = -num;

    double sqrt_num = sqrt(num);
    double den = (double)f9 + d10 + (double)f11 + 1.0;

    return sqrt_num / den;
}