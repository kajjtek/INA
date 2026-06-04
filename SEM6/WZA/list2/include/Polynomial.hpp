#ifndef Polynomial_H
#define Polynomial_H

#include <vector>
#include <algorithm>
#include <cmath>
#include "./Monomial.hpp"

template<long long N, typename Compare>
class Polynomial {
    public:
    std::vector<Monomial<N>> monomials;
    
    Polynomial(std::vector<Monomial<N>>& c) : monomials(c) {}
    Polynomial() {
        Monomial<N> zero;
        monomials.push_back(zero);
    }
    Polynomial(std::initializer_list<Monomial<N>> l) : monomials(l) {
        normalise();
    }

    void normalise() { //todo do przepisania
        Compare comp;
        std::sort(this->monomials.begin(), this->monomials.end(), comp);
        std::vector<Monomial<N>> result;

        for(int i=0; i<this->monomials.size(); i++) {
            const Monomial<N>& current = this->monomials.at(i);
            if(i==monomials.size()-1) {
                if(std::abs(current.coefficient) > 1e-9) {
                    result.push_back(current);
                }
                break;
            }
            Monomial<N>& next = this->monomials.at(i+1);

            if(current.powers == next.powers) {
                next += current;
            } else {
                if(std::abs(current.coefficient) > 1e-9) {
                    result.push_back(current);
                }
            }
        }

        if (result.empty()) result.push_back(Monomial<N>{});
        this->monomials = std::move(result);
    }

    Polynomial &operator+=(const Polynomial &o) {  //todo do przepisania
        std::vector<Monomial<N>> result;
        result.reserve(this->monomials.size()+o.monomials.size());

        int i = 0;
        int j = 0;
        Compare comp;

        while( i < this->monomials.size() && j < o.monomials.size()) {
            Monomial<N> left = this->monomials.at(i);
            Monomial<N> right = o.monomials.at(j);

            if(comp(left, right)) {
                result.push_back(left);
                i++;
            } else if (comp(right,left)) {
                result.push_back(right);
                j++;
            } else {
                result.push_back(left + right);
                i++;
                j++;
            }
        }

        while(i<this->monomials.size()) { result.push_back(this->monomials.at(i++));}
        while(j<o.monomials.size()) { result.push_back(o.monomials.at(j++));}  // BUG FIX: was this->monomials

        this->monomials = std::move(result);
        normalise();
        return *this;
    }
    Polynomial &operator-=(const Polynomial &o) { 
        std::vector<Monomial<N>> result;
        result.reserve(this->monomials.size()+o.monomials.size());

        int i = 0;
        int j = 0;
        Compare comp;

        while( i < this->monomials.size() && j < o.monomials.size()) {
            Monomial<N> left = this->monomials.at(i);
            Monomial<N> right = o.monomials.at(j);

            if(comp(left, right)) {
                result.push_back(left);
                i++;
            } else if (comp(right,left)) {
                Monomial<N> neg_right = right;
                neg_right.coefficient = -neg_right.coefficient;
                result.push_back(neg_right);
                j++;
            } else {
                result.push_back(left - right);
                i++;
                j++;
            }
        }

        while(i<this->monomials.size()) { result.push_back(this->monomials.at(i++));}
        while(j<o.monomials.size()) {
            Monomial<N> neg = o.monomials.at(j++);
            neg.coefficient = -neg.coefficient;
            result.push_back(neg);
        }

        this->monomials = std::move(result);
        normalise();
        return *this;
    }
    Polynomial &operator*=(const Polynomial &o) { 
        std::vector<Monomial<N>> result;
        result.reserve(this->monomials.size()*o.monomials.size());

        for(Monomial<N> left: this->monomials) {
            for(Monomial<N> right: o.monomials) {
                result.push_back(left*right);
            }
        }

        this->monomials = std::move(result);
        normalise();
        return *this;
     }

    Polynomial operator+(const Polynomial &o) const { Polynomial r(*this); r += o; return r; }
    Polynomial operator-(const Polynomial &o) const { Polynomial r(*this); r -= o; return r; }
    Polynomial operator*(const Polynomial &o) const { Polynomial r(*this); r *= o; return r; }

    bool operator==(const Polynomial &o) const {
        if (monomials.size() != o.monomials.size()) return false;
        for (size_t i = 0; i < monomials.size(); ++i) {
            if (this->monomials[i].powers != o.monomials[i].powers) return false; //todo: popraw bo obecnie porownuje tylko coefficienty a nie powersy rowniez
            if (std::abs(monomials[i].coefficient - o.monomials[i].coefficient) >= 1e-9) return false;
        }
            return true;
    }
    bool operator!=(const Polynomial &o) const { return !(*this == o); }
    bool is_zero() const { return *this == Polynomial<N, Compare>(); }

    std::pair<std::vector<Polynomial>, Polynomial> polynomialReduce(std::vector<Polynomial> dividers) {
        int s = dividers.size();

        Polynomial p = *this;
        std::vector<Polynomial> qs(s, Polynomial<N, Compare>());
        Polynomial rest;

        while (!p.is_zero()) {
            int i = 0;
            bool divisionOccured = false;
            
            while (i < s) {
                Monomial<N> leading_term_p = p.LT();
                Polynomial& d = dividers[i];
                Monomial<N> leading_term_d = d.LT();
                
                if (leading_term_p.divisableBy(leading_term_d)) {
                    Monomial<N> res = leading_term_p / leading_term_d;
                    
                    qs[i] += Polynomial({res});
                    p -= Polynomial({res}) * d;
                    
                    divisionOccured = true;
                    break;
                } else {
                    i++;
                }
            }

            if (!divisionOccured) {
                rest += Polynomial({p.LT()});
                p.removeLT();
            }
        }
        return {qs, rest};
    }
    Monomial<N> LT() {
        return monomials.back();
    }

    void removeLT() {
        if (!monomials.empty()) {
            monomials.pop_back();
        }

        // Maintain invariant: polynomial should always have at least one monomial
        // (the zero polynomial is represented as a single zero monomial).
        if (monomials.empty()) {
            monomials.push_back(Monomial<N>{});
        }
    }


};

#endif