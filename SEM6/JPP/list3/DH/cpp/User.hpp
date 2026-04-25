#ifndef USER_H
#define USER_H

#include <stdexcept>
#include "./DHSetup.hpp"

template<long long N>
class User {
    using Num = Ring<N>;
public:
    User(DHSetup<N> &d) : dh(d) {
        this->secret = std::rand() % (N - 1) + 1;
    }

    Num getPublicKey() {
        return dh.power(dh.getGenerator(), this->secret);
    }

    void setKey(Num other_user_public_key) {
        this->shared_secret = dh.power(other_user_public_key, this->secret);
        this->keySet = true;
    }

    Num encrypt(Num m) {
        if (!keySet) throw std::logic_error("Key not set!");
        return m * shared_secret;
    }

    Num decrypt(Num c) {
        if (!keySet) throw std::logic_error("Key not set!");
        // Dzielenie w ciele to mnożenie przez odwrotność
        return c / shared_secret;
    }

private:
    DHSetup<N> &dh;
    unsigned long secret;
    Num shared_secret;
    bool keySet = false;
};

#endif