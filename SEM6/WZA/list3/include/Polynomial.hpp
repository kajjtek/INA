#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include "Monomial.hpp"

template<long long N, typename Compare>
class Polynomial {
public:
    std::vector<Monomial<N>> monomials; // posortowane rosnaco wg Compare; LT = back()

    Polynomial() {
        monomials.push_back(Monomial<N>{});
    }
    explicit Polynomial(std::vector<Monomial<N>> c) : monomials(std::move(c)) {
        normalise();
    }
    Polynomial(std::initializer_list<Monomial<N>> l) : monomials(l) {
        normalise();
    }

    bool is_zero() const {
        return monomials.size() == 1 && std::abs(monomials[0].coefficient) < 1e-9;
    }

    // Wiodacy jednomian (najwiekszy wg Compare)
    Monomial<N> LT() const { return monomials.back(); }

    void removeLT() {
        if (!monomials.empty()) monomials.pop_back();
        if (monomials.empty()) monomials.push_back(Monomial<N>{});
    }

    void normalise() {
        Compare comp;
        std::sort(monomials.begin(), monomials.end(), comp);

        std::vector<Monomial<N>> result;
        for (int i = 0; i < (int)monomials.size(); i++) {
            if (i + 1 < (int)monomials.size() && monomials[i].powers == monomials[i+1].powers) {
                monomials[i+1].coefficient += monomials[i].coefficient;
            } else if (std::abs(monomials[i].coefficient) > 1e-9) {
                result.push_back(monomials[i]);
            }
        }
        if (result.empty()) result.push_back(Monomial<N>{});
        monomials = std::move(result);
    }

    // Mnozy przez jednomian
    Polynomial operator*(const Monomial<N>& m) const {
        std::vector<Monomial<N>> res;
        for (const auto& mon : monomials) res.push_back(mon * m);
        return Polynomial(std::move(res));
    }

    Polynomial& operator+=(const Polynomial& o) {
        for (const auto& m : o.monomials) monomials.push_back(m);
        normalise();
        return *this;
    }
    Polynomial& operator-=(const Polynomial& o) {
        for (auto m : o.monomials) { m.coefficient = -m.coefficient; monomials.push_back(m); }
        normalise();
        return *this;
    }
    Polynomial& operator*=(const Polynomial& o) {
        std::vector<Monomial<N>> res;
        for (const auto& a : monomials)
            for (const auto& b : o.monomials)
                res.push_back(a * b);
        monomials = std::move(res);
        normalise();
        return *this;
    }

    Polynomial operator+(const Polynomial& o) const { Polynomial r(*this); r += o; return r; }
    Polynomial operator-(const Polynomial& o) const { Polynomial r(*this); r -= o; return r; }
    Polynomial operator*(const Polynomial& o) const { Polynomial r(*this); r *= o; return r; }

    bool operator==(const Polynomial& o) const {
        if (monomials.size() != o.monomials.size()) return false;
        for (size_t i = 0; i < monomials.size(); i++) {
            if (monomials[i].powers != o.monomials[i].powers) return false;
            if (std::abs(monomials[i].coefficient - o.monomials[i].coefficient) >= 1e-9) return false;
        }
        return true;
    }
    bool operator!=(const Polynomial& o) const { return !(*this == o); }

    // Redukcja f przez liste dzielnikow G, zwraca {wspolczynniki, reszta}
    std::pair<std::vector<Polynomial>, Polynomial> reduce(std::vector<Polynomial> G) const {
        int s = G.size();
        Polynomial p(*this);
        std::vector<Polynomial> qs(s);
        Polynomial rest;

        while (!p.is_zero()) {
            bool divided = false;
            Monomial<N> lt_p = p.LT();

            for (int i = 0; i < s; i++) {
                Monomial<N> lt_g = G[i].LT();
                if (lt_p.divisableBy(lt_g)) {
                    Monomial<N> factor = lt_p / lt_g;
                    qs[i] += Polynomial({factor});
                    p -= G[i] * factor;
                    divided = true;
                    break;
                }
            }
            if (!divided) {
                rest += Polynomial({lt_p});
                p.removeLT();
            }
        }
        return {qs, rest};
    }

    // Reszta z redukcji przez G
    Polynomial reduceRest(std::vector<Polynomial> G) const {
        return reduce(std::move(G)).second;
    }

    // S-wielomian (syzygium) S(f, g)
    static Polynomial S(const Polynomial& f, const Polynomial& g) {
        Monomial<N> lt_f = f.LT();
        Monomial<N> lt_g = g.LT();
        Monomial<N> lcm_m = Monomial<N>::lcm(lt_f, lt_g);

        // lcm/LT(f) = jednomian o potegach lcm-lt_f i wspolczynniku 1/lt_f.coeff
        std::array<int, N> pf, pg;
        for (int i = 0; i < N; i++) {
            pf[i] = lcm_m.powers[i] - lt_f.powers[i];
            pg[i] = lcm_m.powers[i] - lt_g.powers[i];
        }
        Monomial<N> mf(pf, 1.0 / lt_f.coefficient);
        Monomial<N> mg(pg, 1.0 / lt_g.coefficient);

        return f * mf - g * mg;
    }

    // Algorytm Buchbergera — zwraca baze Groebnera
    static std::vector<Polynomial> buchberger(std::vector<Polynomial> G) {
        bool changed = true;
        while (changed) {
            changed = false;
            std::vector<Polynomial> newG = G;
            int n = G.size();
            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
                    Polynomial s = S(G[i], G[j]);
                    Polynomial r = s.reduceRest(G);
                    if (!r.is_zero()) {
                        newG.push_back(r);
                        changed = true;
                    }
                }
            }
            G = std::move(newG);
        }
        return G;
    }

    // Zredukowana baza Groebnera (makeMonic + interredukcja)
    static std::vector<Polynomial> reducedGroebner(std::vector<Polynomial> G) {
        G = buchberger(std::move(G));

        // Zrob monicze
        for (auto& p : G) {
            double lc = p.LT().coefficient;
            for (auto& m : p.monomials) m.coefficient /= lc;
        }

        // Interredukcja: usun wielomiany, ktorych LT jest podzielne przez LT innego
        bool removed = true;
        while (removed) {
            removed = false;
            for (int i = 0; i < (int)G.size(); i++) {
                std::vector<Polynomial> rest;
                for (int j = 0; j < (int)G.size(); j++)
                    if (j != i) rest.push_back(G[j]);
                Polynomial r = G[i].reduceRest(rest);
                if (r.is_zero()) {
                    G.erase(G.begin() + i);
                    removed = true;
                    break;
                }
                // Normalize
                double lc = r.LT().coefficient;
                for (auto& m : r.monomials) m.coefficient /= lc;
                G[i] = r;
            }
        }
        return G;
    }
};

#endif
