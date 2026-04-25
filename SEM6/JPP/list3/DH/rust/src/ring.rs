use std::fmt;
use std::ops::{Add, Sub, Mul, Div, Neg, AddAssign, SubAssign, MulAssign, DivAssign};
use std::str::FromStr;


#[derive(Debug, Clone, PartialEq)]
pub enum RingError {
    NotInvertible { val: u64, modulus: u64 },
    ParseError(String),
}

impl fmt::Display for RingError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            RingError::NotInvertible { val, modulus } =>
                write!(f, "Element {val} nie jest odwracalny modulo {modulus}"),
            RingError::ParseError(s) =>
                write!(f, "Błąd parsowania: {s}"),
        }
    }
}

impl std::error::Error for RingError {}


fn ext_gcd(a: i64, b: i64) -> (i64, i64, i64) {
    if b == 0 {
        (a, 1, 0)
    } else {
        let (g, x1, y1) = ext_gcd(b, a % b);
        (g, y1, x1 - (a / b) * y1)
    }
}

fn mod_inv(a: u64, n: u64) -> Result<u64, RingError> {
    let a_signed = (a % n) as i64;
    let (g, x, _) = ext_gcd(a_signed, n as i64);
    if g != 1 {
        Err(RingError::NotInvertible { val: a, modulus: n })
    } else {
        Ok(((x % n as i64 + n as i64) as u64) % n)
    }
}


#[derive(Clone, Copy, Eq)]
pub struct Ring<const N: u64> {
    val: u64,
}

impl<const N: u64> Ring<N> {
    const _CHECK: () = assert!(N >= 2, "Modulus N musi byc >= 2");

    fn reduce(v: i64) -> u64 {
        ((v % N as i64 + N as i64) as u64) % N
    }

    pub fn new(v: i64) -> Self {
        let _ = Self::_CHECK;
        Ring { val: Self::reduce(v) }
    }

    pub fn value(self) -> u64 { self.val }
    pub fn modulus() -> u64 { N }

    pub fn inv(self) -> Result<Self, RingError> {
        Ok(Ring { val: mod_inv(self.val, N)? })
    }

    pub fn checked_div(self, rhs: Self) -> Result<Self, RingError> {
        Ok(Ring { val: Self::reduce(self.val as i64 * mod_inv(rhs.val, N)? as i64) })
    }
}

// tu sa konwersjed

impl<const N: u64> From<i64> for Ring<N> {
    fn from(v: i64) -> Self { Ring::new(v) }
}
impl<const N: u64> From<i32> for Ring<N> {
    fn from(v: i32) -> Self { Ring::new(v as i64) }
}
impl<const N: u64> From<u64> for Ring<N> {
    fn from(v: u64) -> Self { Ring::new(v as i64) }
}
impl<const N: u64> From<Ring<N>> for u64 {
    fn from(r: Ring<N>) -> u64 { r.val }
}
impl<const N: u64> From<Ring<N>> for i64 {
    fn from(r: Ring<N>) -> i64 { r.val as i64 }
}


impl<const N: u64> FromStr for Ring<N> {
    type Err = RingError;
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let v: i64 = s.trim().parse()
            .map_err(|_| RingError::ParseError(s.to_string()))?;
        Ok(Ring::new(v))
    }
}


impl<const N: u64> fmt::Display for Ring<N> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.val)
    }
}

impl<const N: u64> fmt::Debug for Ring<N> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "Ring<{}>({})", N, self.val)
    }
}


impl<const N: u64> PartialEq for Ring<N> {
    fn eq(&self, other: &Self) -> bool { self.val == other.val }
}

impl<const N: u64> PartialOrd for Ring<N> {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(other))
    }
}

impl<const N: u64> Ord for Ring<N> {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        self.val.cmp(&other.val)
    }
}


impl<const N: u64> Add for Ring<N> {
    type Output = Self;
    fn add(self, rhs: Self) -> Self { Ring { val: (self.val + rhs.val) % N } }
}

impl<const N: u64> Sub for Ring<N> {
    type Output = Self;
    fn sub(self, rhs: Self) -> Self { Ring { val: (self.val + N - rhs.val) % N } }
}

impl<const N: u64> Mul for Ring<N> {
    type Output = Self;
    fn mul(self, rhs: Self) -> Self { Ring { val: (self.val * rhs.val) % N } }
}

impl<const N: u64> Div for Ring<N> {
    type Output = Self;
    fn div(self, rhs: Self) -> Self {
        self.checked_div(rhs).unwrap_or_else(|e| panic!("{e}"))
    }
}

impl<const N: u64> Neg for Ring<N> {
    type Output = Self;
    fn neg(self) -> Self { Ring { val: if self.val == 0 { 0 } else { N - self.val } } }
}


impl<const N: u64> AddAssign for Ring<N> {
    fn add_assign(&mut self, rhs: Self) { *self = *self + rhs; }
}
impl<const N: u64> SubAssign for Ring<N> {
    fn sub_assign(&mut self, rhs: Self) { *self = *self - rhs; }
}
impl<const N: u64> MulAssign for Ring<N> {
    fn mul_assign(&mut self, rhs: Self) { *self = *self * rhs; }
}
impl<const N: u64> DivAssign for Ring<N> {
    fn div_assign(&mut self, rhs: Self) { *self = *self / rhs; }
}