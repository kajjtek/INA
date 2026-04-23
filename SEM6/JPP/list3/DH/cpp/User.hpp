#ifndef USER_H
#define USER_H

#include "../../../list2/cpp/ring.cpp"
#include "./DHSetup.hpp"

template<long long N>
class User {
    using Num = Ring<N>;
    public:
    Num getPublicKey() {
        return dh.power(dh.getGenerator(), this->secret);
    }
    void setKey(Num bob_key) {
        this->s = dh.power(bob_key, secret);
        this->keySet = true;
    }
    Num encrypt(Num m) {
        if (!keySet) throw std::logic_error("Key not set!");
        return m * key;
    }

    Num decrypt(Num c) {
        if (!keySet) throw std::logic_error("Key not set!");
        return c / key;
    }
    User(DHSetup<N> &d) : dh(d) {
        this->secret = rand() % (N-1) + 1;
    }
    private:
    DHSetup<N> &dh;
    unsigned long secret;
    Num s;
    bool keySet = false;
};

#endif