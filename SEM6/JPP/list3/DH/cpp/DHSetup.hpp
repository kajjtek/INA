#ifndef DHSETUP_H
#define DHSETUP_H

#include "../../../list2/cpp/ring.cpp"
#include <vector>
#include <ctime>
#include <cstdlib>

template<long long N>
class DHSetup {
    using Num = Ring<N>;
public:
    DHSetup() {
        static bool seeded = false;
        if (!seeded) { std::srand(std::time(nullptr)); seeded = true; }
        
        unsigned long phi = N - 1; // Dla ciała charakterystyki N
        do {
            this->generator = Num(std::rand() % phi + 1);
        } while(!isPrimitive(this->generator));
    }

    Num getGenerator() { return this->generator; }

    Num power(Num base, unsigned long exp) {
        Num result(1);
        Num b = base;
        while (exp > 0) {
            if (exp % 2 == 1) result *= b;
            b *= b;
            exp /= 2;
        }
        return result;
    }

private:
    Num generator;

    std::vector<unsigned long> getPrimeDivisors(unsigned long n) {
        std::vector<unsigned long> temp;
        for(unsigned long i = 2; i * i <= n; i++) {
            if(n % i == 0) {
                temp.push_back(i);
                while(n % i == 0) n /= i;
            }
        }
        if (n > 1) temp.push_back(n);
        return temp;
    }

    bool isPrimitive(Num num) {
        if (static_cast<long long>(num) == 0) return false;
        unsigned long phi = N - 1;
        std::vector<unsigned long> divisors = getPrimeDivisors(phi);

        for(unsigned long q : divisors) {
            // Jeśli num^(phi/q) == 1, to num nie jest generatorem
            if(power(num, phi / q) == Num(1)) return false;
        }
        return true;
    }
};

#endif