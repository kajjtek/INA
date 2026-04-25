
.PHONY: all run clean \
        cpp cpp-run \
        ada ada-run \
        rust rust-run

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

# Alire (alr) detection: prefer using alire-managed toolchain if available.
ALR := $(shell command -v alr 2>/dev/null)
ALIRE_GNAT := $(shell ls $(HOME)/.local/share/alire/toolchains/*/bin/gnatmake 2>/dev/null | head -n1)
SYS_GNAT := $(shell command -v gnatmake 2>/dev/null)

ifeq ($(ALR),)
	ifneq ($(ALIRE_GNAT),)
		GNAT := $(ALIRE_GNAT)
	else ifneq ($(SYS_GNAT),)
		GNAT := $(SYS_GNAT)
	else
		GNAT := gnatmake
	endif
else
	GNAT := alr
endif

GNATFLAGS :=

CARGO    := cargo

all: cpp ada rust

cpp: ring_cpp

ring_cpp: ./cpp/ring.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

cpp-run: ring_cpp
	./ring_cpp

ada: ring_ada

ring_ada:
	@if [ -n "$(ALR)" ]; then \
		cd ring_ada && alr build; \
	else \
		$(GNAT) $(GNATFLAGS) -o ring_ada ./ring_ada/src/main.adb; \
	fi

ada-run:
	@if [ -n "$(ALR)" ]; then \
		cd ring_ada && alr run; \
	else \
		./ring_ada; \
	fi

rust:
	cd ring_rust && $(CARGO) build --release 2>&1

rust-run:
	cd ring_rust && $(CARGO) run --release 2>&1

run: cpp-run ada-run rust-run

clean:
	rm -f ring_cpp ring_ada
	rm -f *.o *.ali b~*.ad[sb]
	cd ring_rust && $(CARGO) clean 2>&1