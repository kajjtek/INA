#ifndef LIBC_H
#define LIBC_H

#include <stdbool.h>

typedef struct {
    long x;
    long y;
    bool is_solvable;
} Diofantic_result;

long gcd_C(long n, long m);
long lcd_C(long n);
long euler_C(long n);
Diofantic_result diofantic_C(long a, long b, long c);
bool checkNatural(long x);
long extendedGCD(long a, long b, long *x, long *y);

#endif