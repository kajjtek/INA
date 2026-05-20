#ifndef Polynomial_H
#define Polynomial_H

#include <vector>



template<typename T>
class Polynomial {
    using DivisionResult = std::vector<std::pair<T, T>>;
    public:
        struct XGCDResult {
        Polynomial<T> gcd;
        Polynomial<T> s;
        Polynomial<T> t;
    };
    T leading_coefficient() {
        return coefficients.back();
    }
    std::vector<T> coefficients;
    unsigned long long norm() const {return coefficients.size()-1;}
    static std::pair<Polynomial<T>, Polynomial<T>> division(Polynomial<T> f, Polynomial<T> g) {
        Polynomial<T> zero;
        Polynomial<T> i;
        Polynomial<T> remainder = f;
        while(remainder.norm() >= g.norm() && remainder != zero) {
            Polynomial<T> temp(remainder.norm() - g.norm(), remainder.leading_coefficient()/g.leading_coefficient());
            i += temp;
            remainder-=temp*g;
        } 
        return {i, remainder};
    }
    static Polynomial gdc(Polynomial a, Polynomial b) {
        Polynomial<T> zero;
        while(b!=zero) {
            Polynomial<T> temp = division(a, b).second;
            a = b;
            b = temp;
        }
        return a;
    }
    static Polynomial<T> lcd(Polynomial a, Polynomial b) {
        return division(a*b, gdc(a, b)).first;
    }
    Polynomial(std::vector<T> c) : coefficients(c) {}
    Polynomial() {
        coefficients = {0};
    }
    Polynomial(const Polynomial<T> &o) : coefficients(o.coefficients) {}
    Polynomial(long long term, T coeff) {
        std::vector<T> temp;
        for(int i=0; i<term; i++) {
            temp.push_back(0);
        }
        temp.push_back(coeff);
        coefficients = temp;
        normalise();
    }
    Polynomial(std::initializer_list<T> l) : coefficients(l) {
        normalise();
    }

    void normalise() {
        while (coefficients.size() > 1 && std::abs(coefficients.back()) < 1e-9)
            coefficients.pop_back();
        if (coefficients.empty())
            coefficients.push_back(T(0));
    }

    Polynomial &operator+=(const Polynomial &o) { 
        if(o.coefficients.size() > coefficients.size()) {
            coefficients.resize(o.coefficients.size(), T(0));
        }
        for(int i=0; i<o.coefficients.size(); i++) {
            coefficients[i] += o.coefficients[i];
        }

        normalise();
        return *this;
    }
    Polynomial &operator-=(const Polynomial &o) { 
        if(o.coefficients.size() > coefficients.size()) {
            coefficients.resize(o.coefficients.size(), T(0));
        }
        for(int i=0; i<o.coefficients.size(); i++) {
            coefficients[i] -= o.coefficients[i];
        }

        normalise();
        return *this;
    }
    Polynomial &operator*=(const Polynomial &o) { 
        std::vector<T> result_coef;
        result_coef.resize(coefficients.size() + o.coefficients.size(), T(0));
        for(int i=0; i<coefficients.size(); i++) {
            for(int j=0; j<o.coefficients.size(); j++) {
                result_coef[i+j] += coefficients[i] * o.coefficients[j];
            }
        }

        coefficients = result_coef;
        normalise();
        return *this;
     }

    Polynomial operator+(const Polynomial &o) const { Polynomial r(*this); r += o; return r; }
    Polynomial operator-(const Polynomial &o) const { Polynomial r(*this); r -= o; return r; }
    Polynomial operator*(const Polynomial &o) const { Polynomial r(*this); r *= o; return r; }

    bool operator==(const Polynomial &o) const {
        if (coefficients.size() != o.coefficients.size()) return false;
        for (size_t i = 0; i < coefficients.size(); ++i)
            if (std::abs(coefficients[i] - o.coefficients[i]) >= 1e-9) return false;
        return true;
    }
    bool operator!=(const Polynomial &o) const { return !(*this == o); }
    bool operator< (const Polynomial &o) const { return this->norm() <  o.norm(); }
    bool operator<=(const Polynomial &o) const { return this->norm() <=  o.norm(); }
    bool operator> (const Polynomial &o) const { return this->norm() >  o.norm(); }
    bool operator>=(const Polynomial &o) const { return this->norm() >=  o.norm(); }
    bool is_zero() const { return *this == Polynomial<T>(); }

    
    static XGCDResult xgcd(Polynomial<T> a, Polynomial<T> b) {
        Polynomial<T> s = Polynomial<T>({T(0)});
        Polynomial<T> old_s = Polynomial<T>({T(1)});

        Polynomial<T> t = Polynomial<T>({T(1)});
        Polynomial<T> old_t = Polynomial<T>({T(0)});

        Polynomial<T> r = b;
        Polynomial<T> old_r = a;

        while (!r.is_zero()) {     
            auto res = division(old_r, r);
            Polynomial<T> quotient = res.first;
    
            old_r = r;
            r = res.second;

            Polynomial<T> temp_s = old_s - (quotient * s);
            old_s = s;
            s = temp_s;

            Polynomial<T> temp_t = old_t - (quotient * t);
            old_t = t;
            t = temp_t;
        }
        
        if (!old_r.is_zero()) {
            T l_coeff = old_r.leading_coefficient();
        }

        return {old_r, old_s, old_t};
    }

};

#endif