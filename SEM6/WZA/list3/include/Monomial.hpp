#ifndef MONOMIAL_H
#define MONOMIAL_H

#include <array>
#include <stdexcept>
#include <cmath>

template<long long N>
class Monomial {
public:
    std::array<int, N> powers;
    double coefficient;

    Monomial() : powers{}, coefficient(0.0) { powers.fill(0); }
    Monomial(const std::array<int, N>& p, double c) : powers(p), coefficient(c) {}

    bool divisableBy(const Monomial<N>& other) const {
        for (int i = 0; i < N; i++)
            if (powers[i] < other.powers[i]) return false;
        return true;
    }

    // LCM of two monomials: max of each power, coefficient = 1
    static Monomial<N> lcm(const Monomial<N>& a, const Monomial<N>& b) {
        std::array<int, N> res;
        for (int i = 0; i < N; i++)
            res[i] = std::max(a.powers[i], b.powers[i]);
        return Monomial<N>(res, 1.0);
    }

    Monomial operator/(const Monomial& o) const {
        std::array<int, N> res;
        for (int i = 0; i < N; i++)
            res[i] = powers[i] - o.powers[i];
        return Monomial(res, coefficient / o.coefficient);
    }

    Monomial& operator*=(const Monomial& o) {
        for (int i = 0; i < N; i++) powers[i] += o.powers[i];
        coefficient *= o.coefficient;
        return *this;
    }
    Monomial& operator+=(const Monomial& o) {
        if (powers != o.powers) throw std::runtime_error("Cannot add monomials with different powers");
        coefficient += o.coefficient;
        return *this;
    }
    Monomial& operator-=(const Monomial& o) {
        if (powers != o.powers) throw std::runtime_error("Cannot subtract monomials with different powers");
        coefficient -= o.coefficient;
        return *this;
    }

    Monomial operator*(const Monomial& o) const { Monomial r(*this); r *= o; return r; }
    Monomial operator+(const Monomial& o) const { Monomial r(*this); r += o; return r; }
    Monomial operator-(const Monomial& o) const { Monomial r(*this); r -= o; return r; }

    bool operator==(const Monomial& o) const {
        return powers == o.powers && std::abs(coefficient - o.coefficient) < 1e-9;
    }
    bool operator!=(const Monomial& o) const { return !(*this == o); }
};

#endif
