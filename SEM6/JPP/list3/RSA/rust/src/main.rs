mod ring;
mod rsa;

use crate::ring::Ring;
use crate::rsa::RSA;

fn sep(title: &str) { println!("\n── {title} ──"); }
fn check<T: std::fmt::Display + PartialEq>(name: &str, got: T, expected: T) {
    if got == expected {
        println!("{}: OK", name);
    } else {
        println!("{}: FAIL (got {}, expected {})", name, got, expected);
    }
}

fn main() {
    // Charakterystyka 10007 * 10009 = 100160063
    const P: u64 = 10007;
    const Q: u64 = 10009;
    const MODULUS: u64 = 100160063;

    sep("Test RSA w Rust");

    let rsa = RSA::<MODULUS>::new(P, Q);

    println!("RSA Modulo n: {}", rsa.get_modulo());
    println!("Klucz publiczny e: {}", rsa.get_public_key());

    // Wiadomość jako element pierścienia
    let m = Ring::<MODULUS>::new(1234567);
    println!("\nWiadomość oryginalna: {}", m);

    // Szyfrowanie
    let s = rsa.encrypt(m);
    println!("Szyfrogram: {}", s);

    // Deszyfrowanie
    let d = rsa.decrypt(s);
    println!("Wiadomość odszyfrowana: {}", d);

    if m == d {
        println!("\nSukces: RSA działa poprawnie! ✓");
    } else {
        println!("\nBłąd: Wiadomości się różnią! ✗");
    }

    // Dodatkowy test dla małych wartości
    sep("Testy poprawności dla kilku wartości");
    for val in [2, 42, 99999, 100160062] {
        let msg = Ring::<MODULUS>::new(val as i64);
        let enc = rsa.encrypt(msg);
        let dec = rsa.decrypt(enc);
        check(&format!("Test dla {val}"), dec, msg);
    }
}