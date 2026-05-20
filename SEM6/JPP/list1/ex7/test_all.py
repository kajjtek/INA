import ctypes
import os

path = os.getcwd()

try:
    
    lib_c = ctypes.CDLL(os.path.join(path, "libmath_c.so"))
    lib_ada = ctypes.CDLL(os.path.join(path, "libmath_ada.dylib"))
    lib_rust = ctypes.CDLL(os.path.join(path, "libmath_rust.dylib"))

    
    try:
        lib_ada.adainit()
    except AttributeError:
        
        pass

    
    class DiofanticResult(ctypes.Structure):
        _fields_ = [
            ("x", ctypes.c_longlong),
            ("y", ctypes.c_longlong),
            ("is_solvable", ctypes.c_bool)
        ]


    lib_c.gcd_c.argtypes = [ctypes.c_longlong, ctypes.c_longlong]
    lib_c.gcd_c.restype = ctypes.c_longlong

    lib_c.lcd_c.argtypes = [ctypes.c_longlong]
    lib_c.lcd_c.restype = ctypes.c_longlong

    lib_c.euler_c.argtypes = [ctypes.c_longlong]
    lib_c.euler_c.restype = ctypes.c_longlong

    lib_c.diofantic_c.argtypes = [ctypes.c_longlong, ctypes.c_longlong, ctypes.c_longlong]
    lib_c.diofantic_c.restype = DiofanticResult

    
    
    
    lib_ada.ada_gcd.argtypes = [ctypes.c_longlong, ctypes.c_longlong]
    lib_ada.ada_gcd.restype = ctypes.c_longlong

    lib_ada.ada_lcd.argtypes = [ctypes.c_longlong]
    lib_ada.ada_lcd.restype = ctypes.c_longlong

    lib_ada.ada_euler.argtypes = [ctypes.c_longlong]
    lib_ada.ada_euler.restype = ctypes.c_longlong

    lib_ada.ada_diofantic.argtypes = [ctypes.c_longlong, ctypes.c_longlong, ctypes.c_longlong]
    lib_ada.ada_diofantic.restype = DiofanticResult

    
    lib_rust.rust_gcd.argtypes = [ctypes.c_longlong, ctypes.c_longlong]
    lib_rust.rust_gcd.restype = ctypes.c_longlong

    lib_rust.rust_lcd.argtypes = [ctypes.c_longlong]
    lib_rust.rust_lcd.restype = ctypes.c_longlong

    lib_rust.rust_euler.argtypes = [ctypes.c_longlong]
    lib_rust.rust_euler.restype = ctypes.c_longlong

    lib_rust.rust_diofantic.argtypes = [ctypes.c_longlong, ctypes.c_longlong, ctypes.c_longlong]
    lib_rust.rust_diofantic.restype = DiofanticResult

    
    print("================ WYNIKI PORÓWNAWCZE ================\n")

    
    n, m = 48, 18
    p = 15
    e = 10
    a, b, c_val = 48, 18, 6

    
    print(f"--- Największy wspólny dzielnik ({n}, {m}) ---")
    print(f"C:    {lib_c.gcd_c(n, m)}")
    print(f"Ada:  {lib_ada.ada_gcd(n, m)}")
    print(f"Rust: {lib_rust.rust_gcd(n, m)}\n")

    
    print(f"--- Najmniejszy dzielnik pierwszy ({p}) ---")
    print(f"C:    {lib_c.lcd_c(p)}")
    print(f"Ada:  {lib_ada.ada_lcd(p)}")
    print(f"Rust: {lib_rust.rust_lcd(p)}\n")

    
    print(f"--- Funkcja Eulera ({e}) ---")
    print(f"C:    {lib_c.euler_c(e)}")
    print(f"Ada:  {lib_ada.ada_euler(e)}")
    print(f"Rust: {lib_rust.rust_euler(e)}\n")

    
    print(f"--- Równanie diofantyczne: {a}x - {b}y = {c_val} ---")
    res_c = lib_c.diofantic_c(a, b, c_val)
    res_ada = lib_ada.ada_diofantic(a, b, c_val)
    res_rust = lib_rust.rust_diofantic(a, b, c_val)

    print(f"C:    x={res_c.x}, y={res_c.y}, solvable={res_c.is_solvable}")
    print(f"Ada:  x={res_ada.x}, y={res_ada.y}, solvable={res_ada.is_solvable}")
    print(f"Rust: x={res_rust.x}, y={res_rust.y}, solvable={res_rust.is_solvable}")

except Exception as err:
    print(f"\nWystąpił błąd podczas ładowania lub konfiguracji: {err}")