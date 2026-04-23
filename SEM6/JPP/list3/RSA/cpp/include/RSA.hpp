#ifndef RSA_H
#define RSA_H

#include "../../../../list2/cpp/ring.cpp"

template <long long N>
class RSA {
    using Num = Ring<N>;
    private:
    Num private_key;
    unsigned long long modulo;
    Num public_key;
    Num randomPrime(Num eulerTotient) {

    }
    Num power(Num base, Num exp) {
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
        Num eulerTotient = (prime1-1)*(prime2-1);
        Num e = randomPrime(eulerTotient);
        this->public_key = e;
        this->private_key = modinv(e, this->modulo);
    }
    unsigned long long getModulo() {return this->modulo;}
    Num getPublicKey() {return this->public_key;}
    Num encrypt(Num m) {
        return power(m, this->public_key);
    }
    Num decrypt(Num s) {
        return power(s, this->private_key);
    }
};

#endif