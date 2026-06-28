#ifndef ORDERS_H
#define ORDERS_H

#include "Monomial.hpp"

// Lex: x1 > x2 > ... > xN  (porownuje od indeksu 0 = najwazniejsza zmienna)
struct OrderLex {
    template<long long N>
    bool operator()(const Monomial<N>& a, const Monomial<N>& b) const {
        for (int i = N - 1; i >= 0; i--) {
            if (a.powers[i] != b.powers[i]) return a.powers[i] < b.powers[i];
        }
        return false;
    }
};

// GrLex: najpierw suma stopni, potem Lex
struct OrderGrLex {
    template<long long N>
    bool operator()(const Monomial<N>& a, const Monomial<N>& b) const {
        int sa = 0, sb = 0;
        for (int i = 0; i < N; i++) { sa += a.powers[i]; sb += b.powers[i]; }
        if (sa != sb) return sa < sb;
        for (int i = N - 1; i >= 0; i--) {
            if (a.powers[i] != b.powers[i]) return a.powers[i] < b.powers[i];
        }
        return false;
    }
};

#endif
