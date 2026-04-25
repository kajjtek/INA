#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>


long long extgcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    long long x1, y1;
    long long g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

long long modinv(long long a, long long n) {
    long long x, y;
    long long g = extgcd(((a % n) + n) % n, n, x, y);
    if (g != 1)
        throw std::domain_error(
            "Element " + std::to_string(a) +
            " is not invertible modulo " + std::to_string(n));
    return ((x % n) + n) % n;
}


template <long long N>
class Ring {
    static_assert(N >= 2, "Modulus N must be at least 2");

    long long val_;

        static long long reduce(long long v) {
        return ((v % N) + N) % N;
    }

public:
    
    Ring() : val_(0) {}

    Ring(long long v) : val_(reduce(v)) {}

    Ring(const Ring &) = default;
    Ring(Ring &&)      = default;

    
    Ring &operator=(const Ring &o) = default;
    Ring &operator=(Ring &&o)      = default;

    Ring &operator=(long long v) { val_ = reduce(v); return *this; }

    Ring &operator+=(const Ring &o) { val_ = reduce(val_ + o.val_); return *this; }
    Ring &operator-=(const Ring &o) { val_ = reduce(val_ - o.val_); return *this; }
    Ring &operator*=(const Ring &o) { val_ = reduce(val_ * o.val_); return *this; }
    Ring &operator/=(const Ring &o) { val_ = reduce(val_ * modinv(o.val_, N)); return *this; }

    
    Ring operator+(const Ring &o) const { Ring r(*this); r += o; return r; }
    Ring operator-(const Ring &o) const { Ring r(*this); r -= o; return r; }
    Ring operator*(const Ring &o) const { Ring r(*this); r *= o; return r; }
    Ring operator/(const Ring &o) const { Ring r(*this); r /= o; return r; }

    Ring operator-() const { return Ring(reduce(-val_)); }   
    
    bool operator==(const Ring &o) const { return val_ == o.val_; }
    bool operator!=(const Ring &o) const { return val_ != o.val_; }
    bool operator< (const Ring &o) const { return val_ <  o.val_; }
    bool operator<=(const Ring &o) const { return val_ <= o.val_; }
    bool operator> (const Ring &o) const { return val_ >  o.val_; }
    bool operator>=(const Ring &o) const { return val_ >= o.val_; }

    
    explicit operator long long() const { return val_; }
    explicit operator int()       const { return static_cast<int>(val_); }

    
    friend std::ostream &operator<<(std::ostream &os, const Ring &r) {
        return os << r.val_;
    }

    friend std::istream &operator>>(std::istream &is, Ring &r) {
        long long v;
        if (is >> v) r.val_ = reduce(v);
        return is;
    }

    
    static constexpr long long modulus() { return N; }
};


static void separator(const std::string &title) {
    std::cout << "\n── " << title << " ──\n";
}

// int main() {
//         separator("Arithmetic in Z/7Z");

//     using R7 = Ring<7>;
//     R7 a(3), b(5);
//     std::cout << "a = " << a << ",  b = " << b << "  (mod " << R7::modulus() << ")\n";
//     std::cout << "a + b = " << (a + b) << "  (expected 1)\n";
//     std::cout << "a - b = " << (a - b) << "  (expected 5)\n";
//     std::cout << "a * b = " << (a * b) << "  (expected 1)\n";
//     std::cout << "a / b = " << (a / b) << "  (expected 2, because 5*2=10≡3 mod 7)\n";
//     std::cout << "-a    = " << (-a)    << "  (expected 4)\n";

//         separator("Compound assignment");

//     R7 c(6);
//     std::cout << "c = " << c << "\n";
//     c += R7(4);   std::cout << "c += 4  → " << c << "  (expected 3)\n";
//     c -= R7(5);   std::cout << "c -= 5  → " << c << "  (expected 5)\n";
//     c *= R7(3);   std::cout << "c *= 3  → " << c << "  (expected 1)\n";
//     c /= R7(3);   std::cout << "c /= 3  → " << c << "  (expected 5)\n";

//         separator("Comparisons");

//     R7 x(2), y(5);
//     std::cout << "x=" << x << "  y=" << y << "\n";
//     std::cout << "x == y : " << (x == y) << "  (0)\n";
//     std::cout << "x != y : " << (x != y) << "  (1)\n";
//     std::cout << "x <  y : " << (x <  y) << "  (1)\n";
//     std::cout << "x <= y : " << (x <= y) << "  (1)\n";
//     std::cout << "x >  y : " << (x >  y) << "  (0)\n";
//     std::cout << "x >= y : " << (x >= y) << "  (0)\n";
//     std::cout << "x == 2 (via Ring(2)): " << (x == R7(2)) << "  (1)\n";

//         separator("Stream input");

//     R7 fromStream;
//     std::istringstream iss("15");       iss >> fromStream;
//     std::cout << "Read 15 into Ring<7>: " << fromStream << "  (expected 1)\n";

//         separator("Implicit conversion");

//     R7 big = 100;              std::cout << "Ring<7>(100) = " << big << "  (expected 2)\n";
//     R7 neg = -3;               std::cout << "Ring<7>(-3)  = " << neg << "  (expected 4)\n";

//         separator("Explicit cast to long long");

//     long long lv = static_cast<long long>(a);
//     std::cout << "static_cast<long long>(Ring<7>(3)) = " << lv << "  (expected 3)\n";

//         separator("Exception: non-invertible element");

//     using R6 = Ring<6>;
//     R6 p(4), q(3);
//     std::cout << "Working in Z/6Z: p=" << p << "  q=" << q << "\n";
//     try {
//         R6 result = p / q;           std::cout << "p / q = " << result << "\n";
//     } catch (const std::domain_error &e) {
//         std::cout << "Caught expected exception: " << e.what() << "\n";
//     }

//         separator("Fermat's little theorem in Z/13Z (a^12 ≡ 1)");

//     using R13 = Ring<13>;
//     for (int base : {2, 5, 7, 11}) {
//         R13 cur(1), elem(base);
//         for (int i = 0; i < 12; ++i) cur *= elem;
//         std::cout << base << "^12 mod 13 = " << cur << "  (expected 1)\n";
//     }

//         separator("Copy and assignment");

//     R7 orig(6);
//     R7 copy = orig;
//     copy += R7(1);
//     std::cout << "orig=" << orig << "  copy (after +=1)=" << copy
//               << "  (orig unchanged: " << (orig == R7(6) ? "yes" : "no") << ")\n";

//     std::cout << "\nAll tests passed.\n";
//     return 0;
// }