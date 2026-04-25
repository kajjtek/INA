use std::time::{SystemTime, UNIX_EPOCH};
use crate::Ring;
use crate::ring::mod_inv;

pub struct RSA<const N: u64> {
    public_key: Ring<N>,
    private_key: u64, 
    modulo: u64,
}

impl<const N: u64> RSA<N> {
    
    pub fn new(p: u64, q: u64) -> Self {
        let n = p * q;
        if n != N {
            panic!("Iloczyn p*q ({}) musi być równy parametrowi N ({})!", n, N);
        }

        let phi = (p - 1) * (q - 1);
        
        let e_val = Self::random_e(phi);
        
    let d = mod_inv(e_val, phi).expect("Błąd przy obliczaniu klucza prywatnego");
        RSA {
            public_key: Ring::new(e_val as i64),
            private_key: d,
            modulo: n,
        }
    }

    
    fn random_e(phi: u64) -> u64 {
        let mut seed = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .unwrap()
            .as_nanos() as u64;

        loop {
            seed = seed.wrapping_mul(6364136223846793005).wrapping_add(1);
            let e = 2 + (seed % (phi - 2));
            
            if Self::gcd(e, phi) == 1 {
                return e;
            }
        }
    }

    fn gcd(mut a: u64, mut b: u64) -> u64 {
        while b != 0 {
            a %= b;
            std::mem::swap(&mut a, &mut b);
        }
        a
    }

    
    fn power(mut base: Ring<N>, mut exp: u64) -> Ring<N> {
        let mut res = Ring::<N>::new(1);
        while exp > 0 {
            if exp % 2 == 1 {
                res *= base;
            }
            base *= base;
            exp /= 2;
        }
        res
    }

    pub fn get_modulo(&self) -> u64 {
        self.modulo
    }

    pub fn get_public_key(&self) -> Ring<N> {
        self.public_key
    }

    pub fn encrypt(&self, m: Ring<N>) -> Ring<N> {
        Self::power(m, self.public_key.value())
    }

    pub fn decrypt(&self, s: Ring<N>) -> Ring<N> {
        Self::power(s, self.private_key)
    }
}