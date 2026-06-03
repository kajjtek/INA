#ifndef ORDERS_H
#define ORDERS_H

#include "Monomial.hpp"
#include "Polynomial.hpp"


struct OrderLex {
    template <long long N>
    bool operator()(const Monomial<N>& a, const Monomial<N>& b) const {
        return a.powers < b.powers; // Standardowy leksykograficzny
    }
};

struct OrderGrLex {
    template <long long N>
    bool operator()(const Monomial<N>& a, const Monomial<N>& b) const {
        int sum_a = 0, sum_b = 0;
        for(int i=0; i<N; ++i) { sum_a += a.powers[i]; sum_b += b.powers[i]; }
        
        if (sum_a != sum_b) return sum_a < sum_b;
        return a.powers < b.powers;
    }
};

#endif