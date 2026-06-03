#ifndef MONOMIAL_H
#define MONOMIAL_H

#include <array>

template<long long N>
class Monomial {
    public:
    std::array<int, N> powers;
    float coefficient;
    Monomial(const std::array<int, N>& p, float c) : coefficient(c), powers(p) {}
    Monomial() : powers(), coefficient(0) {}

    Monomial operator/(const Monomial& o) const {
    std::array<int, N> res_powers;
    for(int i = 0; i < N; i++) {
        res_powers[i] = this->powers[i] - o.powers[i];
    }
    return Monomial(res_powers, this->coefficient / o.coefficient);
}

    Monomial &operator+=(const Monomial &o) {  //todo do przepisania
        if(this->powers != o.powers) throw std::runtime_error("Cant add different monomials in temrs of power");
        
        this->coefficient += o.coefficient;
        
        return *this;
    }
    Monomial &operator-=(const Monomial &o) { 
        if(this->powers != o.powers) throw std::runtime_error("Cant add different monomials in temrs of power");
        
        this->coefficient -= o.coefficient;
        
        return *this;
    }
    Monomial &operator*=(const Monomial &o) { 
        for(int i=0; i<N; i++) {
            this->powers[i] += o.powers[i];
        }        
        this->coefficient *= o.coefficient;
        
        return *this;
     }

    Monomial operator+(const Monomial &o) const { Monomial r(*this); r += o; return r; }
    Monomial operator-(const Monomial &o) const { Monomial r(*this); r -= o; return r; }
    Monomial operator*(const Monomial &o) const { Monomial r(*this); r *= o; return r; }

    bool operator==(const Monomial &o) const {
        if(this->coefficient != o.coefficient) return false;
        for(int i=0; i<N; i++) {
            if(this->powers[i]!=o.powers[i]) return false;
        }
        return true;
    }
    bool operator!=(const Monomial &o) const { return !(*this == o); }

    bool divisableBy(const Monomial<N>& other) {
        for(int i=0; i<N; i++) {
            if(powers[i] < other.powers[i]) return false;
        }
        return true;
    }
};

#endif