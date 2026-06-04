#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include "../include/Monomial.hpp"
#include "../include/Polynomial.hpp"
#include "../include/Orders.hpp"

// --- DODATKOWE PORZĄDKI DLA PODPUNKTU E ---

// Porządek Lex gdzie Y > Z > X
struct OrderLexYZX {
    template <long long N>
    bool operator()(const Monomial<N>& a, const Monomial<N>& b) const {
        if (a.powers[1] != b.powers[1]) return a.powers[1] < b.powers[1];
        if (a.powers[2] != b.powers[2]) return a.powers[2] < b.powers[2];
        return a.powers[0] < b.powers[0];
    }
};

// Porządek Lex gdzie Z > X > Y
struct OrderLexZXY {
    template <long long N>
    bool operator()(const Monomial<N>& a, const Monomial<N>& b) const {
        if (a.powers[2] != b.powers[2]) return a.powers[2] < b.powers[2];
        if (a.powers[0] != b.powers[0]) return a.powers[0] < b.powers[0];
        return a.powers[1] < b.powers[1];
    }
};

// Funkcja pomocnicza do ładnego wypisywania wielomianów (dla N = 3)
template<long long N, typename Compare>
void printPolynomial(const Polynomial<N, Compare>& p) {
    if (p.is_zero()) {
        std::cout << "0";
        return;
    }
    bool first = true;
    for (auto it = p.monomials.rbegin(); it != p.monomials.rend(); ++it) {
        if (std::abs(it->coefficient) < 1e-9) continue;
        
        if (it->coefficient > 0 && !first) std::cout << " + ";
        if (it->coefficient < 0) std::cout << " - ";
        else if (first && it->coefficient < 0) std::cout << "-";
        
        float abs_coeff = std::abs(it->coefficient);
        bool has_vars = (it->powers[0] > 0 || it->powers[1] > 0 || it->powers[2] > 0);
        
        if (abs_coeff != 1.0f || !has_vars) {
            std::cout << abs_coeff;
        }
        
        char vars[3] = {'x', 'y', 'z'};
        for (int i = 0; i < N; i++) {
            if (it->powers[i] > 0) {
                std::cout << vars[i];
                if (it->powers[i] > 1) std::cout << "^" << it->powers[i];
            }
        }
        first = false;
    }
}

int main() {
    std::cout << "=== LAB 4: TESTY REDUKCJI ===\n\n";

    // =================================================================
    // PODPUNKT D) Rozwiązanie Ćwiczenia 37
    // =================================================================
    std::cout << "--- Podpunkt d) Ćwiczenie 37 ---\n";

    // f = x^3 - x^2*y - x^2*z
    Polynomial<3, OrderGrLex> f({
        Monomial<3>({3, 0, 0}, 1.0f),
        Monomial<3>({2, 1, 0}, -1.0f),
        Monomial<3>({2, 0, 1}, -1.0f)
    });

    // g1 = x^2*y - z
    Polynomial<3, OrderGrLex> g1({
        Monomial<3>({2, 1, 0}, 1.0f),
        Monomial<3>({0, 0, 1}, -1.0f)
    });

    // g2 = x*y - 1
    Polynomial<3, OrderGrLex> g2({
        Monomial<3>({1, 1, 0}, 1.0f),
        Monomial<3>({0, 0, 0}, -1.0f)
    });

    // 1. Redukcja przez (g1, g2)
    auto reduction1 = f.polynomialReduce({g1, g2});
    std::cout << "Wynik redukcji f przez (g1, g2):\n";
    std::cout << "  q1: "; printPolynomial(reduction1.first[0]); std::cout << "\n";
    std::cout << "  q2: "; printPolynomial(reduction1.first[1]); std::cout << "\n";
    std::cout << "  Reszta r1: "; printPolynomial(reduction1.second); std::cout << "\n\n";

    // 2. Redukcja przez (g2, g1)
    auto reduction2 = f.polynomialReduce({g2, g1});
    std::cout << "Wynik redukcji f przez (g2, g1):\n";
    std::cout << "  q2: "; printPolynomial(reduction2.first[0]); std::cout << "\n";
    std::cout << "  q1: "; printPolynomial(reduction2.first[1]); std::cout << "\n";
    std::cout << "  Reszta r2: "; printPolynomial(reduction2.second); std::cout << "\n\n";


    // =================================================================
    // PODPUNKT E) Wielomian h(x,y,z) dla Twojego indeksu
    // =================================================================
    std::cout << "--- Podpunkt e) Trzy różne porządki Lex ---\n";

    // !!! TUTAJ WPISZ CYFRY SWOJEGO NUMERU INDEKSU (abcdef) !!!
    int a = 2, b = 8, c = 2, d = 2, e = 1, f_idx = 2; 
    std::cout << "Dla indeksu o cyfrach: a=" << a << ", b=" << b << ", c=" << c 
              << ", d=" << d << ", e=" << e << ", f=" << f_idx << "\n\n";

    // Definiujemy uniwersalny zestaw dzielników G = {x - y, y - z}
    // Gwarantuje on 3 różne wyniki dla każdego niezerowego wielomianu h!
    
    // 1. Porządek: Standardowy Lex (X > Y > Z)
    Polynomial<3, OrderLex> h_lex({
        Monomial<3>({a, b, 0}, 1.0f),
        Monomial<3>({0, c, d}, -1.0f),
        Monomial<3>({e, 0, f_idx}, 1.0f)
    });
    Polynomial<3, OrderLex> g1_lex({ Monomial<3>({1,0,0}, 1.0f), Monomial<3>({0,1,0}, -1.0f) }); // x - y
    Polynomial<3, OrderLex> g2_lex({ Monomial<3>({0,1,0}, 1.0f), Monomial<3>({0,0,1}, -1.0f) }); // y - z
    
    auto red_lex1 = h_lex.polynomialReduce({g1_lex, g2_lex});
    std::cout << "Porządek X > Y > Z | Reszta: "; printPolynomial(red_lex1.second); std::cout << "\n";

    // 2. Porządek: Lex (Y > Z > X)
    Polynomial<3, OrderLexYZX> h_yzx({
        Monomial<3>({a, b, 0}, 1.0f),
        Monomial<3>({0, c, d}, -1.0f),
        Monomial<3>({e, 0, f_idx}, 1.0f)
    });
    Polynomial<3, OrderLexYZX> g1_yzx({ Monomial<3>({1,0,0}, 1.0f), Monomial<3>({0,1,0}, -1.0f) });
    Polynomial<3, OrderLexYZX> g2_yzx({ Monomial<3>({0,1,0}, 1.0f), Monomial<3>({0,0,1}, -1.0f) });

    auto red_lex2 = h_yzx.polynomialReduce({g1_yzx, g2_yzx});
    std::cout << "Porządek Y > Z > X | Reszta: "; printPolynomial(red_lex2.second); std::cout << "\n";

    // 3. Porządek: Lex (Z > X > Y)
    Polynomial<3, OrderLexZXY> h_zxy({
        Monomial<3>({a, b, 0}, 1.0f),
        Monomial<3>({0, c, d}, -1.0f),
        Monomial<3>({e, 0, f_idx}, 1.0f)
    });
    Polynomial<3, OrderLexZXY> g1_zxy({ Monomial<3>({1,0,0}, 1.0f), Monomial<3>({0,1,0}, -1.0f) });
    Polynomial<3, OrderLexZXY> g2_zxy({ Monomial<3>({0,1,0}, 1.0f), Monomial<3>({0,0,1}, -1.0f) });

    auto red_lex3 = h_zxy.polynomialReduce({g1_zxy, g2_zxy});
    std::cout << "Porządek Z > X > Y | Reszta: "; printPolynomial(red_lex3.second); std::cout << "\n";

    return 0;
}