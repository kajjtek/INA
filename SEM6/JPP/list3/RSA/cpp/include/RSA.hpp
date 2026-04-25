#ifndef RSA_H
#define RSA_H

#include "../../../../list2/cpp/ring.cpp"

static long long gcd_plain(long long a, long long b) {
    while (b) { a %= b; std::swap(a, b); }
    return a;
}

template <long long N>
class RSA {
    using Num = Ring<N>;
    private:
    Num private_key;
    unsigned long long modulo;
    Num public_key;
    Num randomPrime(long long eulerTotient) {
        static bool seeded = false;
        if (!seeded) { std::srand((unsigned)std::time(nullptr)); seeded = true; }

        for (int attempt = 0; attempt < 1000; ++attempt) {
            long long e = 2 + std::rand() % (eulerTotient - 2);
            if (gcd_plain(e, eulerTotient) == 1)
                return Num(e);
        }
        throw std::runtime_error("Could not find e coprime to phi");
    }
    Num power(Num base, long long exp) {
        Num result(1);
        
        while (exp > 0) {
            if (exp % 2 == 1)
                result *= base;
            
            base *= base;
            exp /= 2;
        }
        
        return result;
    }
    public:
    RSA(unsigned long long prime1, unsigned long long prime2) {
        this->modulo = prime1 * prime2;
        long long eulerTotient = static_cast<long long>((prime1 - 1) * (prime2 - 1));
        Num e = randomPrime(eulerTotient);
        this->public_key = e;
        long long e_num = static_cast<long long>(e);
        long long d_num = modinv(e_num, eulerTotient);
        this->private_key = Num(d_num);
    }
    unsigned long long getModulo() {return this->modulo;}
    Num getPublicKey() {return this->public_key;}
    Num encrypt(Num m) {
        return power(m, static_cast<long long>(this->public_key));
    }
    Num decrypt(Num s) {
        return power(s, static_cast<long long>(this->private_key));
    }
};

#endif