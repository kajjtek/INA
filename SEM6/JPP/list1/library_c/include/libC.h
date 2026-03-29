#ifndef LIBC_H
#define LIBC_H

#include <stdbool.h>

typedef struct {
    long x;
    long y;
    bool is_solvable;
} Diofantic_result;

long gcd(long n, long m);
long lcd(long n);
long euler(long n);
Diofantic_result diofantic(long a, long b, long c);
bool checkNatural(long x);
long extendedGCD(long a, long b, long *x, long *y);

#endif LIBC_H