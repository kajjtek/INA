1. BIBLIOTEKI (ZADANIA 1-3)

C (Zadanie 1)

cd library_c/src
gcc -c -fPIC libC.c -o libC.o
gcc -shared -o libmath_c.so libC.o


Ada (Zadanie 2)

cd library_ada/src/libmath_ada
alr build
# Biblioteka i pliki .o znajdą się w folderze: src/


Rust (Zadanie 3)

cd library_rust
cargo build --release
# Biblioteka znajdzie się w: target/release/libmath_rust.dylib


2. PROGRAMY GŁÓWNE (ZADANIA 4-6)

C-host (Zadanie 4) - POPRAWIONE


cd ex4
gcc ex4/main.c library_c/src/libC.o library_ada/src/libmath_ada/src/math_library.o library_ada/src/libmath_ada/src/b~math_library.o library_rust/target/release/libmath_rust.a /Users/kajtek/.local/share/alire/toolchains/gnat_native_15.1.2_60748c54/lib/gcc/aarch64-apple-darwin23.6.0/15.0.1/adalib/libgnat.a -lpthread -ldl -o ex4/program_zad4


Ada-host (Zadanie 5)

cd ex5
gnatmake main_test.adb \
  -I../library_ada/src/libmath_ada/src \
  -largs \
  ../library_c/src/libC.o \
  ../library_rust/target/release/libmath_rust.a \
  -lpthread -ldl


Rust-host (Zadanie 6) 

Kompilacja z jawnym linkowaniem bindera Ady i ustawieniem ścieżki przeszukiwania bibliotek (RPATH):

cd ex6
rustc main.rs \
  -C link-arg=../library_ada/src/libmath_ada/src/b~math_library.o \
  -L . -l math_c \
  -L . -l math_ada \
  -L /Users/kajtek/.local/share/alire/toolchains/gnat_native_15.1.2_60748c54/lib/gcc/aarch64-apple-darwin23.6.0/15.0.1/adalib \
  -C link-arg=-Wl,-rpath,/Users/kajtek/.local/share/alire/toolchains/gnat_native_15.1.2_60748c54/lib/gcc/aarch64-apple-darwin23.6.0/15.0.1/adalib \
  -l gnat \
  -o program_zad6


3. PYTHON (ZADANIE 7)

Przygotowanie plików w folderze ex7:

cd ex7
cp ../library_c/src/libmath_c.so .
cp ../library_ada/src/libmath_ada/src/libmath_ada.dylib .
cp ../ex6/libgnat-15.dylib .
cp ../library_rust/target/release/libmath_rust.dylib .


Uruchomienie skryptu:

Wymaga ustawienia ścieżki do bibliotek uruchomieniowych Ady:

export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:/Users/kajtek/.local/share/alire/toolchains/gnat_native_15.1.2_60748c54/lib/gcc/aarch64-apple-darwin23.6.0/15.0.1/adalib
python3 test_all.py
