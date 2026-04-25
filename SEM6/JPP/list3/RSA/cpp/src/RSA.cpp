#include <stdio.h>
#include <iostream>
#include "../include/RSA.hpp"

int main() {
    // 1. Definicja parametrów zadania
    const long long p = 10007;
    const long long q = 10009;
    const long long n = p * q; // To jest 100160063

    std::cout << "--- Test Algorytmu RSA ---" << std::endl;
    std::cout << "Liczby pierwsze: p=" << p << ", q=" << q << std::endl;
    std::cout << "Modulo n: " << n << std::endl;

    try {
        // 2. Inicjalizacja RSA z szablonem o charakterystyce n
        // Uzywamy n jako parametru szablonu, bo Ring musi operowac na tym module
        RSA<100160063> rsa(p, q);

        std::cout << "Klucz publiczny (e): " << rsa.getPublicKey() << std::endl;
        std::cout << "Modulo z klasy: " << rsa.getModulo() << std::endl;

        // 3. Testowanie szyfrowania i deszyfrowania
        // Tworzymy wiadomosc m jako element pierscienia
        Ring<100160063> m(1234567); // Przykładowa wiadomość
        std::cout << "\nOryginalna wiadomosc (m): " << m << std::endl;

        // Szyfrowanie
        auto szyfrogram = rsa.encrypt(m);
        std::cout << "Zaszyfrowana wiadomosc (c): " << szyfrogram << std::endl;

        // Deszyfrowanie
        auto odszyfrowana = rsa.decrypt(szyfrogram);
        std::cout << "Odszyfrowana wiadomosc: " << odszyfrowana << std::endl;

        // 4. Weryfikacja poprawności
        if (m == odszyfrowana) {
            std::cout << "\nSukces: Wiadomosc zostala poprawnie odzyskana!" << std::endl;
        } else {
            std::cout << "\nBlad: Wiadomosci sie roznia!" << std::endl;
        }

        // 5. Test dla kilku innych wartosci
        std::cout << "\n--- Szybki test dla innych wartosci ---" << std::endl;
        long long testy[] = {2, 999999, 42, 100160062};
        for(long long t : testy) {
            Ring<100160063> msg(t);
            if (rsa.decrypt(rsa.encrypt(msg)) == msg) {
                std::cout << "Test dla " << t << ": OK" << std::endl;
            } else {
                std::cout << "Test dla " << t << ": FAIL" << std::endl;
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Wystapil blad: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}