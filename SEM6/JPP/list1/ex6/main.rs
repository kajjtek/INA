use std::fmt;


#[repr(C)]
#[derive(Debug, PartialEq, Clone, Copy)]
pub struct DiofanticResult {
    pub x: i64,
    pub y: i64,
    pub is_solvable: bool,
}


extern "C" {
    
    fn gcd(n: i64, m: i64) -> i64;
    fn lcd(n: i64) -> i64;
    fn euler(n: i64) -> i64;
    fn diofantic(a: i64, b: i64, c: i64) -> DiofanticResult;

    
    fn ada_gcd(n: i64, m: i64) -> i64;
    fn ada_lcd(n: i64) -> i64;
    fn ada_euler(n: i64) -> i64;
    fn ada_diofantic(a: i64, b: i64, c: i64) -> DiofanticResult;

    
    fn adainit();
    fn adafinal();
}

#[path = "../library_rust/src/lib.rs"]
mod rust_lib;


fn main() {
    
    unsafe { adainit(); }

    let (a, b, c_val) = (48, 18, 10);

    println!("--- TESTY PORÓWNAWCZE (RUST HOST) ---");

    unsafe {
        
        let res_c = gcd(a, b);
        let res_ada = ada_gcd(a, b);
        let res_rust = rust_lib::gcd(a, b);
        print_res("GCD(48,18)", res_c, res_ada, res_rust);

        
        print_res("LCD(15)", lcd(15), ada_lcd(15), rust_lib::lcd(15));

        
        print_res("Euler(10)", euler(10), ada_euler(10), rust_lib::euler(10));

        
        let d_c = diofantic(10, 6, 2);
        let d_ada = ada_diofantic(10, 6, 2);
        let d_rust = rust_lib::diofantic(10, 6, 2);
        
        println!("\nDiofantic(10, 6, 2):");
        println!("  C:    x={}, y={}, sol={}", d_c.x, d_c.y, d_c.is_solvable);
        println!("  Ada:  x={}, y={}, sol={}", d_ada.x, d_ada.y, d_ada.is_solvable);
        println!("  Rust: x={}, y={}, sol={}", d_rust.x, d_rust.y, d_rust.is_solvable);
    }

    
    unsafe { adafinal(); }
}

fn print_res(label: &str, c: i64, ada: i64, rust: i64) {
    let status = if c == ada && ada == rust { "OK" } else { "ERROR" };
    println!("{:<12} | C: {:<3} | Ada: {:<3} | Rust: {:<3} | Status: {}", label, c, ada, rust, status);
}