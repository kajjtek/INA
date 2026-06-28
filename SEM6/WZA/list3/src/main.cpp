#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "../include/Monomial.hpp"
#include "../include/Polynomial.hpp"
#include "../include/Orders.hpp"

// Indeks 282212: a=2, b=8, c=2, d=2, e=1, f=2
static const int A=2, B=8, C=2, D=2, E=1, F=2;

// =========================================================
// Pomocnicze: wypisywanie wielomianow trojzmiennych (x,y,z)
// =========================================================
template<long long N, typename Comp>
std::string polyStr(const Polynomial<N, Comp>& p, const std::vector<std::string>& vars) {
    if (p.is_zero()) return "0";
    std::string out;
    for (auto it = p.monomials.rbegin(); it != p.monomials.rend(); ++it) {
        double c = it->coefficient;
        if (std::abs(c) < 1e-9) continue;
        if (!out.empty() && c > 0) out += " + ";
        else if (c < 0) { out += (out.empty() ? "-" : " - "); c = -c; }
        bool hasVars = false;
        for (int i = 0; i < N; i++) if (it->powers[i] > 0) { hasVars = true; break; }
        if (std::abs(c - 1.0) > 1e-9 || !hasVars) {
            // print as integer if close
            if (std::abs(c - std::round(c)) < 1e-6) out += std::to_string((long long)std::round(c));
            else out += std::to_string(c);
        }
        for (int i = 0; i < N; i++) {
            if (it->powers[i] == 1) out += vars[i];
            else if (it->powers[i] > 1) out += vars[i] + "^" + std::to_string(it->powers[i]);
        }
    }
    return out.empty() ? "0" : out;
}

// Skrot dla 3 zmiennych xyz
using P3 = Polynomial<3, OrderLex>;
using M3 = Monomial<3>;
// vars: [0]=x  [1]=y  [2]=z  — ale LT wg OrderLex musi byc "najwiekszy"
// Uwaga: OrderLex porownuje od indeksu N-1 w dol (indeks 2 = z jest najwazniejszy dla eliminacji)
// Zdefiniujemy: indeks 0=z, 1=y, 2=x  aby lex eliminowal z jako pierwsze
// Konwencja w tym pliku: powers[0]=z, powers[1]=y, powers[2]=x
// Dzieki temu LT (back po sortowaniu rosnaco) ma najwyzszy stopien w z, potem y, potem x
static const std::vector<std::string> VARS3 = {"z","y","x"};
// powers: [0]=z, [1]=y, [2]=x

static M3 m3(int pz, int py, int px, double c) {
    return M3({pz, py, px}, c);
}

void section(const std::string& s) {
    std::cout << "\n" << std::string(60,'=') << "\n" << s << "\n" << std::string(60,'=') << "\n";
}

void printBasis(const std::vector<P3>& G) {
    std::cout << "Baza (" << G.size() << " elementow):\n";
    for (int i = 0; i < (int)G.size(); i++)
        std::cout << "  g[" << i << "] = " << polyStr(G[i], VARS3) << "\n";
}

// Filtruj elementy bazy bez zmiennej z (powers[0]==0 dla wszystkich jednomianow)
std::vector<P3> eliminateZ(const std::vector<P3>& G) {
    std::vector<P3> res;
    for (const auto& p : G) {
        bool hasZ = false;
        for (const auto& m : p.monomials)
            if (m.powers[0] != 0) { hasZ = true; break; }
        if (!hasZ) res.push_back(p);
    }
    return res;
}

// =========================================================
// LAB 6b — demonstracja S-wielomianu i kroku Buchbergera
// =========================================================
void lab6b() {
    section("LAB 6b: S-wielomian i algorytm Buchbergera — przyklad");

    // Klasyczny przyklad: I = <x^2 - y, x^3 - z> w R[z,y,x] (GrLex)
    using PG = Polynomial<3, OrderGrLex>;
    using MG = Monomial<3>;

    // powers: [0]=z, [1]=y, [2]=x (jak wyzej)
    // x^2 - y  (px=2, py=1, pz=0) → powers[2]=2, powers[1]=1, powers[0]=0
    PG f1({MG({0,0,2}, 1.0), MG({0,1,0}, -1.0)});
    // x^3 - z  (px=3, pz=1) → powers[2]=3, powers[0]=1
    PG f2({MG({0,0,3}, 1.0), MG({1,0,0}, -1.0)});

    std::cout << "f1 = " << polyStr(f1, VARS3) << "\n";
    std::cout << "f2 = " << polyStr(f2, VARS3) << "\n";

    PG s12 = PG::S(f1, f2);
    std::cout << "S(f1,f2) = " << polyStr(s12, VARS3) << "\n";

    auto G = PG::buchberger({f1, f2});
    std::cout << "Baza Groebnera:\n";
    for (int i = 0; i < (int)G.size(); i++)
        std::cout << "  g[" << i << "] = " << polyStr(G[i], VARS3) << "\n";
}

// =========================================================
// LAB 6c — uklad x^2+y^2-z^2=0, f(x,y,z)=0, 5 przypadkow
// Konwencja: powers = [z, y, x]
// =========================================================
void lab6c() {
    section("LAB 6c: Uklad x^2+y^2=z^2, f(x,y,z)=0 (indeks 282212: a=2,b=8,c=2,d=2,e=1,f=2)");

    // x^2 + y^2 - z^2 = 0
    // powers[0]=z, powers[1]=y, powers[2]=x
    P3 pythagorean({
        m3(2, 0, 0, -1.0),  // -z^2
        m3(0, 2, 0,  1.0),  //  y^2
        m3(0, 0, 2,  1.0)   //  x^2
    });

    struct Case { std::string name; P3 f; };
    std::vector<Case> cases = {
        // f1 = (c+1)z = 3z
        {"f = (c+1)z = 3z", P3({m3(1,0,0, (double)(C+1))})},
        // f2 = z + d + 1 = z + 3
        {"f = z+d+1 = z+3", P3({m3(1,0,0, 1.0), m3(0,0,0, (double)(D+1))})},
        // f3 = x + z - e - 1 = x + z - 2
        {"f = x+z-e-1 = x+z-2", P3({m3(0,0,1, 1.0), m3(1,0,0, 1.0), m3(0,0,0, (double)(-E-1))})},
        // f4 = x + y + z + f + 1 = x + y + z + 3
        {"f = x+y+z+f+1 = x+y+z+3", P3({m3(0,0,1,1.0), m3(0,1,0,1.0), m3(1,0,0,1.0), m3(0,0,0,(double)(F+1))})},
        // f5 = y/a + z + 1 = y/2 + z + 1
        {"f = y/a+z+1 = y/2+z+1", P3({m3(0,1,0, 1.0/(double)A), m3(1,0,0, 1.0), m3(0,0,0, 1.0)})},
    };

    for (auto& cas : cases) {
        std::cout << "\n--- " << cas.name << " ---\n";
        std::cout << "  rownanie f: " << polyStr(cas.f, VARS3) << " = 0\n";

        auto G = P3::reducedGroebner({pythagorean, cas.f});
        printBasis(G);

        auto Ify = eliminateZ(G);
        std::cout << "Po eliminacji z (elementy bez z):\n";
        if (Ify.empty()) std::cout << "  (brak — ideał trywialny lub wymaga dalszej eliminacji)\n";
        for (auto& p : Ify)
            std::cout << "  " << polyStr(p, VARS3) << " = 0\n";
    }
}

// =========================================================
// LAB 6d — Spirala Pascala / kardioda
// (x^2+y^2-ax)^2 = z^2(x^2+y^2)  +  x + 2y + 3z = 0
// Konwencja: powers = [x, y, z]  — eliminujemy x, wiec x musi byc na indeksie 2
// Uzywamy osobnego typu z x na pozycji 2 (taka sama kolejnosc jak VARS3 ale zmienne to x,y,z)
// Tak sie sklada ze VARS3=[z,y,x] i powers=[pz,py,px] — dla zadania d)
// chcemy eliminowac x wiec x=powers[2] jest juz najwazniejszy w OrderLex.
// Ale semantycznie zmienimy etykiety: teraz powers[0]=x, powers[1]=y, powers[2]=z
// Potrzebujemy innego porzadku: x najwazniejszy = powers[0] najwazniejszy
// OrderLex porownuje od indeksu N-1. Wiec aby x byl najwazniejszy: powers[2]=x
// Czyli: powers[0]=z, powers[1]=y, powers[2]=x (to samo co wyzej!)
// W takim razie wystarczy ze dla zadania d) labelling: VARS3d = {"z","y","x"}
// a do rownan podstawiamy x <- pozycja [2], y<-[1], z<-[0]
// =========================================================
void lab6d() {
    section("LAB 6d: Spirala Pascala (x^2+y^2-ax)^2=z^2(x^2+y^2), x+2y+3z=0 (a=2)");
    // powers: [0]=z, [1]=y, [2]=x — eliminujemy x (indeks 2, LT przy OrderLex)

    // (x^2+y^2-2x)^2 - z^2(x^2+y^2) = 0
    // Rozwijamy: (x^2+y^2)^2 - 4x(x^2+y^2) + 4x^2 - z^2 x^2 - z^2 y^2 = 0
    // = x^4 + 2x^2y^2 + y^4 - 4x^3 - 4xy^2 + 4x^2 - z^2 x^2 - z^2 y^2 = 0
    P3 spiral({
        m3(0,0,4,  1.0),   // x^4
        m3(0,2,2,  2.0),   // 2x^2y^2
        m3(0,4,0,  1.0),   // y^4
        m3(0,0,3, -4.0),   // -4x^3
        m3(0,2,1, -4.0),   // -4xy^2
        m3(0,0,2,  4.0),   // 4x^2
        m3(2,0,2, -1.0),   // -z^2x^2
        m3(2,2,0, -1.0),   // -z^2y^2
    });

    // x + 2y + 3z = 0
    P3 plane({
        m3(0,0,1, 1.0),   // x
        m3(0,1,0, 2.0),   // 2y
        m3(1,0,0, 3.0),   // 3z
    });

    std::cout << "Rownanie spirali: " << polyStr(spiral, VARS3) << " = 0\n";
    std::cout << "Plaszczyzna:      " << polyStr(plane,  VARS3) << " = 0\n";
    std::cout << "\nLicze baze Groebnera (moze chwile potrwac)...\n";
    std::cout.flush();

    auto G = P3::reducedGroebner({spiral, plane});
    printBasis(G);

    // Eliminacja x: elementy bez x (powers[2]==0)
    std::vector<P3> noX;
    for (const auto& p : G) {
        bool hasX = false;
        for (const auto& m : p.monomials) if (m.powers[2] != 0) { hasX = true; break; }
        if (!hasX) noX.push_back(p);
    }
    std::cout << "\nPo eliminacji x (wielomiany bez x):\n";
    if (noX.empty()) std::cout << "  (brak)\n";
    for (auto& p : noX) std::cout << "  " << polyStr(p, VARS3) << " = 0\n";

    std::cout << "\nPierwsze rownanie z=a=2: (x^2+y^2-2x)^2 = 4(x^2+y^2)\n";
    std::cout << "To jest rownoanie kardiody w ukladzie kartezjanskim.\n";
    std::cout << "Wykres: narysowac przy uzyciu pliku plots/lab6d.py\n";
}

// =========================================================
// LAB 6e — Trysektrisa
// r = b(4cos(theta) + 1/cos(theta)), b=8
// Wyprowadzenie analityczne:
//   r = b*4*cos(theta) + b/cos(theta)
//   r*cos(theta) = 4b*cos^2(theta) + b
//   Podstaw x=r*cos(theta), r^2=x^2+y^2, cos(theta)=x/r:
//   x = 4b*(x/r)^2*r/1 + b => upraszczamy:
//   r*x = 4b*x^2 + b*r^2  => (x^2+y^2)*x = 4b*x^2 + b*(x^2+y^2)
//   (x^2+y^2)*x - b*(x^2+y^2) = 4b*x^2
//   (x^2+y^2)*(x-b) = 4b*x^2
//   Dla b=8: (x^2+y^2)*(x-8) - 32*x^2 = 0
// Weryfikacja przez Buchbergera z eliminacja pomocniczych zmiennych
// =========================================================
void lab6e() {
    section("LAB 6e: Trysektrisa r=b*(4cos(t)+1/cos(t)), b=8");

    // Analityczny wynik: (x^2+y^2)(x-8) = 32x^2
    // Rownowaznie: x^3 + xy^2 - 8x^2 - 8y^2 - 32x^2 = 0
    // = x^3 + xy^2 - 40x^2 - 8y^2 = 0

    // Konwencja: powers=[z,y,x] (tu nie ma z, wiec pz=0 zawsze)
    // Uzyjemy Polynomial<2, OrderLex> dla 2 zmiennych (y,x): powers[0]=y, powers[1]=x
    using P2  = Polynomial<2, OrderLex>;
    using M2  = Monomial<2>;
    auto m2 = [](int py, int px, double c) { return M2({py,px},c); };
    const std::vector<std::string> vars2 = {"y","x"};

    // (x^2+y^2)(x-8) - 32x^2 = x^3 + xy^2 - 8x^2 - 8y^2 - 32x^2
    //                         = x^3 + xy^2 - 40x^2 - 8y^2
    P2 trisectrix({
        m2(0,3,  1.0),   // x^3
        m2(2,1,  1.0),   // xy^2
        m2(0,2,-40.0),   // -40x^2
        m2(2,0, -8.0),   // -8y^2
    });

    std::cout << "Rownanie trysektrisy (analityczne):\n";
    std::cout << "  (x^2+y^2)(x-8) - 32x^2 = 0\n";
    std::cout << "  Czyli: " << polyStr(trisectrix, vars2) << " = 0\n\n";

    // Weryfikacja Buchbergerem:
    // Zmienne pomocnicze: wprowadzamy 4 zmienne [c, s, r, x, y] i relacje
    // To skomplikowane algebraicznie — zamiast tego pokazujemy ze nasz wielomian
    // jest poprawny przez podstawienie biegunowe numeryczne
    std::cout << "Weryfikacja numeryczna (theta = 0, pi/6, pi/4):\n";
    double bval = B;
    auto verify = [&](double theta) {
        double cosT = std::cos(theta);
        double r = bval * (4.0*cosT + 1.0/cosT);
        double x = r * cosT;
        double y = r * std::sin(theta);
        double lhs = (x*x + y*y)*(x - bval) - 4.0*bval*x*x;
        std::cout << "  theta=" << theta << "  x=" << x << "  y=" << y
                  << "  rownanie=" << lhs << "  (powinno byc ~0)\n";
    };
    verify(0.1); verify(0.5); verify(1.0);

    std::cout << "\nKrzywa trysektrisy: x^3 + xy^2 - 40x^2 - 8y^2 = 0\n";
    std::cout << "Do czego sluzy: trysektrisa Maclaurina pozwala trisektowac dowolny kat\n";
    std::cout << "  geometrycznie — trojdzielenie kata jest niemozliwe cyrklem i linijka,\n";
    std::cout << "  ale mozliwe przy pomocy tej krzywej.\n";
    std::cout << "\nWykres: patrz plots/lab6e.py\n";
}

int main() {
    std::cout << "WZA Sprawozdanie 3 — Kajetan Plewa 282212\n";
    std::cout << "Indeks: a=" << A << " b=" << B << " c=" << C
              << " d=" << D << " e=" << E << " f=" << F << "\n";

    lab6b();
    lab6c();
    lab6d();
    lab6e();

    return 0;
}
