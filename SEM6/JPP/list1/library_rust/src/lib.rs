#[repr(C)]
pub struct DiofanticResult {
    pub x: i64,
    pub y: i64,
    pub is_solvable: bool,
}

#[no_mangle]
pub extern "C" fn gcd(mut n: i64, mut m: i64) -> i64 {
    if n <= 0 || m <= 0 { return -1; }
    
    while m > 0 {
        let temp = m;
        m = n % m;
        n = temp;
    }
    n
}

#[no_mangle]
pub extern "C" fn lcd(n: i64) -> i64 {
    if n < 2 { return -1; }
    if n % 2 == 0 { return 2; }
    
    let mut i = 3;
    while i * i <= n {
        if n % i == 0 { return i; }
        i += 2;
    }
    n
}

#[no_mangle]
pub extern "C" fn euler(mut n: i64) -> i64 {
    if n < 1 { return -1; }
    let mut result = n;

    if n % 2 == 0 {
        result -= result / 2;
        while n % 2 == 0 { n /= 2; }
    }

    let mut i = 3;
    while i * i <= n {
        if n % i == 0 {
            result -= result / i;
            while n % i == 0 { n /= i; }
        }
        i += 2;
    }

    if n > 1 { result -= result / n; }

    result
}

fn extended_gcd(a: i64, b: i64, x: &mut i64, y: &mut i64) -> i64 {
    if b == 0 {
        *x = 1;
        *y = 0;
        return a;
    }

    let mut x1 = 0;
    let mut y1 = 0;
    let g = extended_gcd(b, a % b, &mut x1, &mut y1);

    *x = y1;
    *y = x1 - (a / b) * y1;

    g
}

#[no_mangle]
pub extern "C" fn diofantic(a: i64, b: i64, c: i64) -> DiofanticResult {
    let mut x = 0;
    let mut y = 0;
    let g = extended_gcd(a, b, &mut x, &mut y);

    if c % g != 0 {
        return DiofanticResult { x: 0, y: 0, is_solvable: false };
    }

    let scale = c / g;
    DiofanticResult {
        x: x * scale,
        y: y * scale,
        is_solvable: true,
    }
}