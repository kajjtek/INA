# Ustawienia kompilacji
CXX = g++
CXXFLAGS = -std=c++20 -O3 -Wall
# Flaga -lm jest niezbędna do linkowania biblioteki matematycznej (np. dla log, std::bit_width w RadixHeap)
LDFLAGS = -lm 

# Katalogi
BIN_DIR = bin
SRC_DIR = src
INC_DIR = include

# ----------------- Pliki Implementacyjne Algorytmów -----------------
ALGO_SRC = $(SRC_DIR)/Dijkstra.cpp $(SRC_DIR)/Dial.cpp $(SRC_DIR)/RadixDijkstra.cpp
COMMON_SRC = $(SRC_DIR)/Graph.cpp $(SRC_DIR)/Parser.cpp 

# Pliki obiektowe (.o) utworzone ze wszystkich implementacji.
OBJS = $(ALGO_SRC:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o) $(COMMON_SRC:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)

# Lista programów do zbudowania (nazwy plików binarnych)
# WAŻNE: W Windows rozszerzenie .exe jest dodawane automatycznie przez kompilator.
TARGETS = dijkstra dial radixheap comparison

# Pliki z funkcją 'main'
DIJKSTRA_MAIN_SRC = $(SRC_DIR)/mainDijkstra.cpp
DIAL_MAIN_SRC = $(SRC_DIR)/mainDial.cpp
RADIX_MAIN_SRC = $(SRC_DIR)/mainRadix.cpp
COMPARISON_MAIN_SRC = $(SRC_DIR)/mainComparison.cpp

# Dodajemy rozszerzenie .exe do ścieżek plików binarnych
DIJKSTRA_BIN = $(BIN_DIR)/dijkstra.exe
DIAL_BIN = $(BIN_DIR)/dial.exe
RADIX_BIN = $(BIN_DIR)/radixheap.exe
COMPARISON_BIN = $(BIN_DIR)/comparison.exe

# Domyślny cel - buduje wszystko
all: directories $(TARGETS)

# Cel: Tworzenie wymaganego katalogu wyjściowego
# Zmiana: Użycie standardowego 'mkdir' dla Windows i dodanie sprawdzenia warunkowego.
directories:
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)

# Reguła kompilacji wspólnych plików obiektowych (.o)
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Kompilowanie obiektu: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(INC_DIR)

# ----------------- Reguły linkowania celów binarnych -----------------

# 1. Budowanie dijkstra
dijkstra: $(DIJKSTRA_BIN)

$(DIJKSTRA_BIN): $(DIJKSTRA_MAIN_SRC) $(OBJS)
	@echo "Linkowanie: dijkstra"
	$(CXX) $(CXXFLAGS) -o $@ $(DIJKSTRA_MAIN_SRC) $(OBJS) -I$(INC_DIR) $(LDFLAGS)

# 2. Budowanie dial
dial: $(DIAL_BIN)

$(DIAL_BIN): $(DIAL_MAIN_SRC) $(OBJS)
	@echo "Linkowanie: dial"
	$(CXX) $(CXXFLAGS) -o $@ $(DIAL_MAIN_SRC) $(OBJS) -I$(INC_DIR) $(LDFLAGS)

# 3. Budowanie radixheap
radixheap: $(RADIX_BIN)

$(RADIX_BIN): $(RADIX_MAIN_SRC) $(OBJS)
	@echo "Linkowanie: radixheap"
	$(CXX) $(CXXFLAGS) -o $@ $(RADIX_MAIN_SRC) $(OBJS) -I$(INC_DIR) $(LDFLAGS) -pg -g

# 4. Budowanie comparison (porównanie wszystkich algorytmów)
comparison: $(COMPARISON_BIN)

$(COMPARISON_BIN): $(COMPARISON_MAIN_SRC) $(OBJS)
	@echo "Linkowanie: comparison"
	$(CXX) $(CXXFLAGS) -o $@ $(COMPARISON_MAIN_SRC) $(OBJS) -I$(INC_DIR) $(LDFLAGS)

# ----------------- Cel czyszczący -----------------

clean:
	@echo "Usuwanie celow i katalogu $(BIN_DIR)..."
	-del /Q $(BIN_DIR)\*.exe
	-del /Q $(BIN_DIR)\*.o
	-rmdir /S /Q $(BIN_DIR)

.PHONY: all directories clean $(TARGETS)