#ifndef DHSETUP_H
#define DHSETUP_H

#include "../../../list2/cpp/ring.cpp"
#include <vector>

template<long long N>
class DHSetup {
    using Num = Ring<N>;
    public:
    Num getGenerator() {
        return this->generator;
    }
    Num power(Num base, unsigned long exp) {
        Num result(1);
        
        while (exp > 0) {
            if (exp % 2 == 1)
                result *= base;
            
            base *= base;
            exp /= 2;
        }
        
        return result;
    }
    DHSetup(unsigned long mod): p(mod) {
        unsigned long modulo = this->p-1;
        do {
            this->generator = rand() % modulo + 1;
        } while(!isPrimitive(this->generator));
    }
    private:
    Num generator;
    unsigned long p;
    std::vector<Num> getPrimeDivisors(unsigned long p) {
        std::vector<Num> temp;
        unsigned long n =p-1;
        for(int i = 2; i*i <=n; i++) {
            if((n) % i == 0) {
                temp.push_back(i);
                while(n % i == 0) n /= i;
            }
        }
        if (n > 1) temp.push_back(n);
        return temp;
    }
    bool isPrimitive(Num num) {
        std::vector<Num> divisors = getPrimeDivisors(this->p);

        for(Num x: divisors) {
            if(power(num, (this->p-1)/static_cast<long long>(x)) == 1) return false;
        }
        return true;
    }
};

#endif