#include <iostream>
#include "User.hpp"

int main() {
    const long long P = 1234567891;

    try {
        std::cout << "Inicjalizacja DH dla P = " << P << "..." << std::endl;
        DHSetup<P> setup;
        std::cout << "Znaleziony generator: " << setup.getGenerator() << std::endl;

        User<P> alice(setup);
        User<P> bob(setup);

        // Wymiana kluczy publicznych
        auto alice_pub = alice.getPublicKey();
        auto bob_pub = bob.getPublicKey();

        std::cout << "Klucz publiczny Alicji: " << alice_pub << std::endl;
        std::cout << "Klucz publiczny Boba: " << bob_pub << std::endl;

        // Ustalenie wspólnego klucza
        alice.setKey(bob_pub);
        bob.setKey(alice_pub);

        // Test szyfrowania
        Ring<P> message(987654321);
        std::cout << "\nOryginalna wiadomosc: " << message << std::endl;

        auto encrypted = alice.encrypt(message);
        std::cout << "Zaszyfrowana (Alicja): " << encrypted << std::endl;

        auto decrypted = bob.decrypt(encrypted);
        std::cout << "Odszyfrowana (Bob): " << decrypted << std::endl;

        if (message == decrypted) {
            std::cout << "\nSukces! Protokol Diffie-Hellman zadzialal poprawnie." << std::endl;
        } else {
            std::cout << "\nBlad! Wiadomosci sie roznia." << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Blad: " << e.what() << std::endl;
    }

    return 0;
}