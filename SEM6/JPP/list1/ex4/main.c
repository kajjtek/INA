#include <stdio.h>
#include <stdbool.h>
#include "../library_c/include/libC.h" 
extern long rust_gcd(long n, long m);
extern long rust_lcd(long n);
extern long rust_euler(long n);
extern Diofantic_result rust_diofantic(long a, long b, long c);

extern long ada_gcd(long n, long m);
extern long ada_lcd(long n);
extern long ada_euler(long n);
extern Diofantic_result ada_diofantic(long a, long b, long c);

void test_results(const char* label, long resC, long resAda, long resRust) {
    printf("%-10s | C: %ld | Ada: %ld | Rust: %ld | Status: %s\n", 
           label, resC, resAda, resRust, 
           (resC == resAda && resAda == resRust) ? "OK" : "BŁĄD");
}

int main() {
    extern void adainit(void);
    extern void adafinal(void);
    adainit();

    long a = 48, b = 18;

    test_results("GCD(48,18)", gcd_C(a, b), ada_gcd(a, b), rust_gcd(a, b));

    test_results("LCD(15)", lcd_C(15), ada_lcd(15), rust_lcd(15));

    test_results("Euler(10)", euler_C(10), ada_euler(10), rust_euler(10));

    Diofantic_result dC = diofantic_C(10, 6, 2);
    Diofantic_result dAda = ada_diofantic(10, 6, 2);
    Diofantic_result dRust = rust_diofantic(10, 6, 2);
    
    printf("Diofantic | C: x=%ld,y=%ld | Ada: x=%ld,y=%ld | Rust: x=%ld,y=%ld\n",
           dC.x, dC.y, dAda.x, dAda.y, dRust.x, dRust.y);

    adafinal();
    return 0;
}