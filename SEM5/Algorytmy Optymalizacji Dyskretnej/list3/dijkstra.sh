#!/bin/bash

# ==============================================================================
# Skrypt do automatycznego uruchamiania programu Dijkstry z rekurencyjnym 
# przeszukiwaniem katalogów i zachowaniem struktury wyjściowej.
# Autor: Gemini (Google)
# ==============================================================================

# Wymagany plik wykonywalny, zgodnie z Makefile
EXECUTABLE="./bin/dijkstra"
INPUTS_DIR="./inputs"
RESULTS_DIR="./results"

# ---------------------------- WERYFIKACJA -------------------------------------

# 1. Sprawdzenie, czy plik wykonywalny istnieje
if [ ! -f "$EXECUTABLE" ]; then
    echo "Blad: Nie znaleziono pliku wykonywalnego: $EXECUTABLE." >&2
    echo "Upewnij sie, ze najpierw uruchomiles komende: make dijkstra" >&2
    exit 1
fi

# 2. Tworzenie głównego katalogu na wyniki
mkdir -p "$RESULTS_DIR"

echo "=== Rozpoczecie testow dla $EXECUTABLE (rekursywnie) ==="
echo "Wyniki beda zapisane w: $RESULTS_DIR (z zachowaniem struktury)"
echo "---------------------------------------------------------"

# ---------------------------- GŁÓWNA PĘTLA ------------------------------------

# Rekurencyjna iteracja przez wszystkie pliki grafów (.gr) w katalogu wejściowym i podkatalogach
# Uzywamy find -type f -name "*.gr", aby ujac Long-C/test.gr, Long/test.gr, itd.
find "$INPUTS_DIR" -type f -name "*.gr" | while read -r FULL_GRAPH_PATH; do
    
    # 1. Określenie ścieżki bazowej (np. Long-C/test)
    # Usuwamy prefix $INPUTS_DIR/ oraz sufiks .gr
    GRAPH_RELATIVE_PATH="${FULL_GRAPH_PATH#$INPUTS_DIR/}"
    BASE_PATH_NO_EXT="${GRAPH_RELATIVE_PATH%.gr}"
    
    echo "Analiza: $FULL_GRAPH_PATH"

    # 2. Definicja ścieżek do plików zapytań
    SS_FILE="$INPUTS_DIR/$BASE_PATH_NO_EXT.ss"
    P2P_FILE="$INPUTS_DIR/$BASE_PATH_NO_EXT.p2p"

    # 3. Definicja ścieżek wyjściowych z zachowaniem struktury katalogów
    # Wynik: np. results/Long-C/test.ss.res
    SS_OUTPUT_RES="$RESULTS_DIR/$BASE_PATH_NO_EXT.ss.res"
    P2P_OUTPUT_RES="$RESULTS_DIR/$BASE_PATH_NO_EXT.p2p.res"

    # Tworzenie katalogu wyjściowego dla bieżącego testu, jeśli nie istnieje (np. results/Long-C)
    OUTPUT_FILE_DIR=$(dirname "$SS_OUTPUT_RES")
    mkdir -p "$OUTPUT_FILE_DIR"

    # ------------------- SSSP TEST (Single-Source Shortest Path) -------------------
    if [ -f "$SS_FILE" ]; then
        
        echo -n "  [SSSP] Test: $BASE_PATH_NO_EXT.ss -> "
        
        # KOMENDA DLA SSSP:
        # ./bin/dijkstra -d <plik.gr> -ss <plik.ss> -oss <wynik.ss.res>
        $EXECUTABLE -d "$FULL_GRAPH_PATH" -ss "$SS_FILE" -oss "$SS_OUTPUT_RES" 2>/dev/null
        
        if [ $? -eq 0 ]; then
            TIME=$(grep "^t " "$SS_OUTPUT_RES" 2>/dev/null | awk '{print $2}')
            if [ -n "$TIME" ]; then
                 echo "OK (Czas: ${TIME}s) | Wynik: $SS_OUTPUT_RES"
            else
                 echo "OK (Brak czasu 't' w pliku) | Wynik: $SS_OUTPUT_RES"
            fi
        else
            echo "BLAD podczas wykonania SSSP."
        fi
    fi

    # ------------------- P2P TEST (Point-to-Point) -------------------
    if [ -f "$P2P_FILE" ]; then
        
        echo -n "  [P2P] Test: $BASE_PATH_NO_EXT.p2p -> "
        
        # KOMENDA DLA P2P:
        # ./bin/dijkstra -d <plik.gr> -p2p <plik.p2p> -op2p <wynik.p2p.res>
        $EXECUTABLE -d "$FULL_GRAPH_PATH" -p2p "$P2P_FILE" -op2p "$P2P_OUTPUT_RES" 2>/dev/null
        
        if [ $? -eq 0 ]; then
            echo "OK | Wynik: $P2P_OUTPUT_RES"
        else
            echo "BLAD podczas wykonania P2P."
        fi
    fi
    
    echo "---"

done

echo "=== Koniec testow ==="