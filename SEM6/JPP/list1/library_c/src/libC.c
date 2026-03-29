#include "../include/libC.h"

bool checkNatural(long x) {
    return x>0;
}

long gcd(long n, long m){
    if(!(checkNatural(n) && checkNatural(m))) return -1;
    
      while(m>0) {
        long temp = m;
        m = n % m;
        n = temp;
      }
      return n;
}

long lcd(long n) {
  if( n< 2 ) return -1;

  if (n%2==0) return 2;

  for (long i = 3; i * i <= n; i += 2) {
    if (n % i == 0) return i;
  }

  return n;
}

long euler(long n) {
  if (n < 1) return -1;
    long result = n;

    if (n % 2 == 0) {
        result -= result / 2;   
        while (n % 2 == 0) n /= 2;
    }

    for (long i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            result -= result / i;   
            while (n % i == 0) n /= i;
        }
    }

    if (n > 1) result -= result / n;

    return result;
}

Diofantic_result diofantic(long a, long b, long c) {
    long x = 0, y = 0;
    long g = extendedGCD(a, b, &x, &y);
    
    Diofantic_result result;
    
    if (c % g != 0) {
        
        result.x = 0;
        result.y = 0;
        result.is_solvable = false;
        return result;
    }
    
    long scale = c / g;
    result.x = x * scale;
    result.y = y * scale;
    result.is_solvable = true;
    
    return result;
}

long extendedGCD(long a, long b, long *x, long *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }

    long x1, y1;
    long g = extendedGCD(b, a % b, &x1, &y1);

    *x = y1;
    *y = x1 - (a / b) * y1;

    return g;
}