use std::time::{SystemTime, UNIX_EPOCH};
mod ring;
use crate::ring::Ring;


struct Lcg {
    state: u64,
}

impl Lcg {
    fn new() -> Self {
        let state = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .unwrap()
            .as_nanos() as u64;
        Self { state }
    }

    fn next(&mut self) -> u64 {
        self.state = self.state.wrapping_mul(6364136223846793005).wrapping_add(1);
        self.state
    }

    fn next_in_range(&mut self, min: u64, max: u64) -> u64 {
        min + (self.next() % (max - min + 1))
    }
}



pub struct DHSetup<const N: u64> {
    generator: Ring<N>,
}

impl<const N: u64> DHSetup<N> {
    pub fn new() -> Self {
        let mut rng = Lcg::new();
        let phi = N - 1;
        let factors = Self::get_prime_divisors(phi);

        loop {
            
            let candidate = rng.next_in_range(2, N - 1);
            let g = Ring::<N>::new(candidate as i64);

            if Self::is_primitive(g, phi, &factors) {
                return Self { generator: g };
            }
        }
    }

    pub fn get_generator(&self) -> Ring<N> {
        self.generator
    }

    
    pub fn power(mut base: Ring<N>, mut exp: u64) -> Ring<N> {
        let mut result = Ring::<N>::new(1);
        while exp > 0 {
            if exp % 2 == 1 {
                result *= base;
            }
            base *= base;
            exp /= 2;
        }
        result
    }

    fn get_prime_divisors(mut n: u64) -> Vec<u64> {
        let mut factors = Vec::new();
        let mut i = 2;
        while i * i <= n {
            if n % i == 0 {
                factors.push(i);
                while n % i == 0 {
                    n /= i;
                }
            }
            i += 1;
        }
        if n > 1 {
            factors.push(n);
        }
        factors
    }

    fn is_primitive(g: Ring<N>, phi: u64, factors: &[u64]) -> bool {
        if g.value() == 0 {
            return false;
        }
        for &q in factors {
            if Self::power(g, phi / q).value() == 1 {
                return false;
            }
        }
        true
    }
}



pub struct User<'a, const N: u64> {
    
    dh: &'a DHSetup<N>,
    secret: u64,
    
    shared_secret: Option<Ring<N>>, 
}

impl<'a, const N: u64> User<'a, N> {
    pub fn new(dh: &'a DHSetup<N>) -> Self {
        let mut rng = Lcg::new();
        Self {
            dh,
            secret: rng.next_in_range(1, N - 1),
            shared_secret: None,
        }
    }

    pub fn get_public_key(&self) -> Ring<N> {
        DHSetup::<N>::power(self.dh.get_generator(), self.secret)
    }

    pub fn set_key(&mut self, peer_public_key: Ring<N>) {
        self.shared_secret = Some(DHSetup::<N>::power(peer_public_key, self.secret));
    }

    pub fn encrypt(&self, m: Ring<N>) -> Result<Ring<N>, &'static str> {
        match self.shared_secret {
            Some(key) => Ok(m * key),
            None => Err("Klucz szyfrujący nie został ustawiony!"),
        }
    }

    pub fn decrypt(&self, c: Ring<N>) -> Result<Ring<N>, &'static str> {
        match self.shared_secret {
            Some(key) => Ok(c / key), 
            None => Err("Klucz deszyfrujący nie został ustawiony!"),
        }
    }
}



fn main() {
    
    const P: u64 = 1234567891;

    println!("--- Inicjalizacja DH dla P = {} ---", P);
    
    
    let setup = DHSetup::<P>::new();
    println!("Wylosowany generator ciała: {}", setup.get_generator());

    
    let mut alice = User::new(&setup);
    let mut bob = User::new(&setup);

    
    let alice_pub = alice.get_public_key();
    let bob_pub = bob.get_public_key();

    println!("Klucz publiczny Alicji: {}", alice_pub);
    println!("Klucz publiczny Boba: {}", bob_pub);

    
    let dummy_msg = Ring::<P>::new(123);
    match alice.encrypt(dummy_msg) {
        Ok(_) => println!("BŁĄD: Szyfrowanie zadziałało bez klucza!"),
        Err(e) => println!("Poprawnie zablokowano szyfrowanie: {}", e),
    }

    
    alice.set_key(bob_pub);
    bob.set_key(alice_pub);

    
    let message = Ring::<P>::new(987654321);
    println!("\nOryginalna wiadomość: {}", message);

    
    let encrypted = alice.encrypt(message).expect("Błąd szyfrowania");
    println!("Zaszyfrowana (Alicja): {}", encrypted);

    
    let decrypted = bob.decrypt(encrypted).expect("Błąd deszyfrowania");
    println!("Odszyfrowana (Bob): {}", decrypted);

    
    if message == decrypted {
        println!("\nSukces! Protokół Diffie-Hellman działa bez zarzutu.");
    } else {
        println!("\nBłąd! Wiadomości się różnią.");
    }
}