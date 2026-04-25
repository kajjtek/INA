.PHONY: all clean dh rsa dh-run rsa-run \
        dh-cpp dh-cpp-run dh-ada dh-ada-run dh-rust dh-rust-run \
        rsa-cpp rsa-cpp-run rsa-ada rsa-ada-run rsa-rust rsa-rust-run

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
CARGO    := cargo

# Wykrywanie Alire na macOS
ALR := $(shell command -v alr 2>/dev/null)

all: dh rsa

# Główny podział
dh: dh-cpp dh-ada dh-rust
rsa: rsa-cpp rsa-ada rsa-rust

run: dh-run rsa-run

dh-run: dh-cpp-run dh-ada-run dh-rust-run
rsa-run: rsa-cpp-run rsa-ada-run rsa-rust-run

# ==========================================
# DIFFIE-HELLMAN (DH)
# ==========================================

dh-cpp: DH_cpp_bin

DH_cpp_bin: ./DH/cpp/main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

dh-cpp-run: dh-cpp
	./DH_cpp_bin

dh-ada:
	@if [ -n "$(ALR)" ]; then \
		cd ./DH/ada/dha && alr build; \
	else \
		echo "Brak polecenia alr. Zainstaluj Alire, aby skompilowac projekt Ada."; \
	fi

dh-ada-run:
	@if [ -n "$(ALR)" ]; then \
		cd ./DH/ada/dha && alr run; \
	else \
		echo "Brak polecenia alr."; \
	fi

dh-rust:
	cd ./DH/rust && $(CARGO) build --release

dh-rust-run:
	cd ./DH/rust && $(CARGO) run --release

# ==========================================
# RSA
# ==========================================

rsa-cpp: RSA_cpp_bin

# Zakładam, że w RSA.cpp znajduje się Twoja funkcja main()
RSA_cpp_bin: ./RSA/cpp/src/RSA.cpp
	$(CXX) $(CXXFLAGS) -I./RSA/cpp/include -o $@ $<

rsa-cpp-run: rsa-cpp
	./RSA_cpp_bin

rsa-ada:
	@if [ -n "$(ALR)" ]; then \
		cd ./RSA/ada/rsa && alr build; \
	else \
		echo "Brak polecenia alr. Zainstaluj Alire, aby skompilowac projekt Ada."; \
	fi

rsa-ada-run:
	@if [ -n "$(ALR)" ]; then \
		cd ./RSA/ada/rsa && alr run; \
	else \
		echo "Brak polecenia alr."; \
	fi

rsa-rust:
	cd ./RSA/rust && $(CARGO) build --release

rsa-rust-run:
	cd ./RSA/rust && $(CARGO) run --release

# ==========================================
# CLEAN (CZYSZCZENIE)
# ==========================================

clean:
	rm -f DH_cpp_bin RSA_cpp_bin
	@if [ -n "$(ALR)" ]; then \
		cd ./DH/ada/dha && alr clean; \
		cd ./RSA/ada/rsa && alr clean; \
	fi
	cd ./DH/rust && $(CARGO) clean
	cd ./RSA/rust && $(CARGO) clean