#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

#include "ex1/include/GaussNum.hpp"
#include "ex2/include/Polynomial.hpp"
#include "ex3/include/ProductOrder.hpp"

const long long a=2, b=8, c=2, d=2, e=1, f=2;

std::string gstr(const GaussNum& g) {
    std::ostringstream o;
    o << g.real;
    if (g.im >= 0) o << '+';
    o << g.im << 'i';
    return o.str();
}

GaussNum gcd_list(std::vector<GaussNum> v) {
    GaussNum r = v[0];
    for (size_t i = 1; i < v.size(); ++i) r = GaussNum::gdc(r, v[i]);
    return r;
}

GaussNum lcm_list(std::vector<GaussNum> v) {
    GaussNum r = v[0];
    for (size_t i = 1; i < v.size(); ++i) r = GaussNum::lcd(r, v[i]);
    return r;
}

std::string pstr(const Polynomial<double>& p) {
    std::ostringstream o;
    bool first = true;
    for (int i = (int)p.coefficients.size()-1; i >= 0; --i) {
        double co = p.coefficients[i];
        if (std::abs(co) < 1e-9) continue;
        if (!first && co > 0.0) o << '+';
        if (i == 0) {
            o << co;
        } else if (i == 1) {
            if (co == 1.0) o << "x";
            else if (co == -1.0) o << "-x";
            else o << co << "x";
        } else {
            if (co == 1.0) o << "x^" << i;
            else if (co == -1.0) o << "-x^" << i;
            else o << co << "x^" << i;
        }
        first = false;
    }
    if (first) o << "0";
    return o.str();
}

template<std::size_t N>
std::string ptstr(const Point<N>& p) {
    std::ostringstream o;
    o << '(';
    for (size_t i = 0; i < N; ++i) { if (i) o << ','; o << p.coords[i]; }
    o << ')';
    return o.str();
}

void section(const std::string& s) {
    std::cout << "\n========================================\n";
    std::cout << s << "\n";
    std::cout << "========================================\n";
}

int main() {
    section("LAB 1: Liczby Gaussa  [indeks 282212: a=2 b=8 c=2 d=2 e=1 f=2]");

    GaussNum g_abi(a, b);
    std::cout << "N(" << gstr(g_abi) << ") = " << g_abi.norm() << "\n";

    GaussNum dividend = GaussNum(c+a, d+b);
    GaussNum divisor(e, f);
    std::cout << "\nDzielenie (" << gstr(dividend) << ")"
              << ") = " << gstr(dividend) << "\nprzez " << gstr(divisor) << ":\n";
    auto divs = GaussNum::division(dividend, divisor);
    for (auto& [q, r] : divs)
        std::cout << "  q=" << gstr(q) << "  r=" << gstr(r)
                  << "  (sprawdzenie: q*d+r = " << gstr(q*divisor+r) << ")\n";
    std::cout << "Liczba roznych wynikow: " << divs.size() << "\n";

    std::vector<GaussNum> triple = {GaussNum(a,b), GaussNum(c,d), GaussNum(e,d)};
    std::cout << "\nTrojka: " << gstr(triple[0]) << ", " << gstr(triple[1]) << ", " << gstr(triple[2]) << "\n";
    std::cout << "NWD = " << gstr(gcd_list(triple)) << "\n";
    std::cout << "NWW = " << gstr(lcm_list(triple)) << "\n";
    std::cout << "\nLista pusta:      NWD=nieokreslone, NWW=nieokreslone\n";
    std::cout << "Lista 1-element.: NWD(" << gstr(triple[0]) << ") = " << gstr(triple[0])
              << ",  NWW = " << gstr(triple[0]) << "\n";

    section("LAB 2: Wielomiany R[x]");

    Polynomial<double> poly_main({(double)b, 0.0, (double)c});
    Polynomial<double> poly_xp1({1.0, 1.0});
    std::cout << "p(x) = " << pstr(poly_main) << "\n";
    std::cout << "norm(p) = " << poly_main.norm() << "\n";
    auto [qp, rp] = Polynomial<double>::division(poly_main, poly_xp1);
    std::cout << "p(x) / (x+1):  q=" << pstr(qp) << "  r=" << pstr(rp) << "\n";

    Polynomial<double> vx({(double)d, (double)c, (double)b, (double)a});
    Polynomial<double> wx({0.0, (double)f, (double)e, (double)d});
    std::cout << "\nv(x) = " << pstr(vx) << "\n";
    std::cout << "w(x) = " << pstr(wx) << "\n";

    section("LAB 2: Testy GCD/LCM dla wielomianow");

auto check_gcd = [&](std::string label, Polynomial<double> p, Polynomial<double> q,
                      std::string expected_gcd) {
    auto g = Polynomial<double>::gdc(p, q);
    auto l = Polynomial<double>::lcd(p, q);
    std::cout << label << "\n";
    std::cout << "  p = " << pstr(p) << ",  q = " << pstr(q) << "\n";
    std::cout << "  NWD = " << pstr(g) << "  (oczekiwane: " << expected_gcd << ")\n";
    std::cout << "  NWW = " << pstr(l) << "\n";
    auto [qa, ra] = Polynomial<double>::division(l, p);
    auto [qb, rb] = Polynomial<double>::division(l, q);
    std::cout << "  Sprawdzenie NWW: NWW/p reszta=" << pstr(ra)
              << "  NWW/q reszta=" << pstr(rb) << "\n";
};

    // x^2-1 = (x-1)(x+1),  x-1  => NWD = x-1
    check_gcd("Test 1: NWD(x^2-1, x-1) = x-1",
        Polynomial<double>({-1.0, 0.0, 1.0}),
        Polynomial<double>({-1.0, 1.0}),
        "x-1");

    // x^2-1,  x^2-2x+1=(x-1)^2  => NWD = x-1
    check_gcd("Test 2: NWD(x^2-1, x^2-2x+1) = x-1",
        Polynomial<double>({-1.0, 0.0, 1.0}),
        Polynomial<double>({1.0, -2.0, 1.0}),
        "x-1");

    // wzajemnie pierwsze: x^2+1, x+1 => NWD = 1
    check_gcd("Test 3: NWD(x^2+1, x+1) = 1",
        Polynomial<double>({1.0, 0.0, 1.0}),
        Polynomial<double>({1.0, 1.0}),
        "1");

    // NWD(p, p) = p (monic)
    check_gcd("Test 4: NWD(x^2-1, x^2-1) = x^2-1",
        Polynomial<double>({-1.0, 0.0, 1.0}),
        Polynomial<double>({-1.0, 0.0, 1.0}),
        "x^2-1");

    auto xr = Polynomial<double>::xgcd(vx, wx);
    std::cout << "XGCD(v,w):\n";
    std::cout << "  gcd = " << pstr(xr.gcd) << "\n";
    std::cout << "    s = " << pstr(xr.s) << "\n";
    std::cout << "    t = " << pstr(xr.t) << "\n";
    std::cout << "NWD(v,w) = " << pstr(Polynomial<double>::gdc(vx, wx)) << "\n";
    std::cout << "NWW(v,w) = " << pstr(Polynomial<double>::lcd(vx, wx)) << "\n";

    auto eval = [](Polynomial<double>& p, double x) {
        double val = 0.0, xn = 1.0;
        for (double c : p.coefficients) { val += c * xn; xn *= x; }
        return val;
    };
    auto evald = [](Polynomial<double>& p, double x) {
        double val = 0.0, xn = 1.0;
        for (size_t i = 1; i < p.coefficients.size(); ++i) { val += i * p.coefficients[i] * xn; xn *= x; }
        return val;
    };

    double alpha = -2.0;
    for (int it = 0; it < 100; ++it) {
        double fa = eval(vx, alpha), dfa = evald(vx, alpha);
        if (std::abs(dfa) < 1e-15) break;
        double next = alpha - fa / dfa;
        if (std::abs(next - alpha) < 1e-12) { alpha = next; break; }
        alpha = next;
    }
    double g = -eval(wx, alpha);

    std::cout << "\nPierwiastek v(x): alfa = " << alpha << "\n";
    std::cout << "g = -w(alfa) = " << g << "\n";

    Polynomial<double> wg = wx + Polynomial<double>({g});
    auto gvw = Polynomial<double>::gdc(vx, wg);
    std::cout << "w(x)+g = " << pstr(wg) << "\n";
    std::cout << "NWD(v, w+g) = " << pstr(gvw) << "\n";
    std::cout << "NWW(v, w+g) = " << pstr(Polynomial<double>::lcd(vx, wg)) << "\n";

    section("LAB 3: Porzadki produktowe");

    auto cmpStr = [](bool le, bool ge) -> std::string {
        if (le && ge) return "=";
        if (le) return "<=";
        if (ge) return ">=";
        return "||";
    };

    std::vector<Point<2>> pairs2 = {{a,b},{c,d},{e,f}};
    std::vector<std::string> n2 = {"(a,b)","(c,d)","(e,f)"};
    std::cout << "Pary w N^2 (porzadek produktowy):\n";
    for (int i = 0; i < 3; ++i)
        for (int j = i+1; j < 3; ++j)
            std::cout << "  " << n2[i] << ptstr(pairs2[i]) << " "
                      << cmpStr(pairs2[i].product_less_equal(pairs2[j]),
                                pairs2[j].product_less_equal(pairs2[i]))
                      << " " << n2[j] << ptstr(pairs2[j]) << "\n";

    Point<3> ace({a,c,e}), bdf({b,d,f});
    std::cout << "\nTrojki w N^3:\n";
    std::cout << "  (a,c,e)" << ptstr(ace) << " "
              << cmpStr(ace.product_less_equal(bdf), bdf.product_less_equal(ace))
              << " (b,d,f)" << ptstr(bdf) << "\n";

    std::cout << "\nZbior A = {(x,y) in N^2 : (x-" << a << ")^2 + (y-" << b << ")^2 < 5}:\n";
    std::vector<Point<2>> setA;
    for (long long x = 0; x <= a+3; ++x)
        for (long long y = 0; y <= b+3; ++y)
            if ((x-a)*(x-a)+(y-b)*(y-b) < 5)
                setA.push_back({x,y});
    std::cout << "Elementy A: ";
    for (auto& p : setA) std::cout << ptstr(p) << " ";
    std::cout << "\nMin(A):     ";
    for (auto& p : findMinimals(setA)) std::cout << ptstr(p) << " ";
    std::cout << "\n";

    std::cout << "\nZbior B = {(x1,x2,x3,x4) in N^4 : (xi-ci)^2 > 224}, c=("
              << c << "," << d << "," << e << "," << f << "):\n";
    std::vector<Point<4>> setB;
    for (long long x1 = 0; x1 <= c+16; ++x1)
        for (long long x2 = 0; x2 <= d+16; ++x2)
            for (long long x3 = 0; x3 <= e+16; ++x3)
                for (long long x4 = 0; x4 <= f+16; ++x4)
                    if ((x1-c)*(x1-c)+(x2-d)*(x2-d)+(x3-e)*(x3-e)+(x4-f)*(x4-f) > 224)
                        setB.push_back({x1,x2,x3,x4});
    std::cout << "Min(B):\n";
    for (auto& p : findMinimals(setB)) std::cout << "  " << ptstr(p) << "\n";

    return 0;
}