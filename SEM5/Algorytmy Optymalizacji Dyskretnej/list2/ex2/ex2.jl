using JuMP
import HiGHS

using DelimitedFiles

function parse_production_data(filename::String)
    # Wczytanie wszystkich linii z pliku jako tekst
    data_lines = readlines(filename)
    
    # Funkcja pomocnicza do parsowania linii i ignorowania pustych elementów
    parse_line(line) = parse.(Int, filter!(!isempty, split(line, ' ')))

    # 1. Liczba produktów i maszyn
    dimensions = parse_line(data_lines[1])
    n_product = dimensions[1]
    n_machines = dimensions[2]

    # 2. Maksymalny popyt (Linia 2)
    max_product_demand = parse_line(data_lines[2])
    
    # 3. Zysk jednostkowy (Linia 3 - Używamy obliczonego zysku)
    # Uwaga: Zysk (profit) to Cena - Koszt Materiałowy.
    # Używamy wstępnie obliczonych zysków [5, 6, 5, 4]
    product_profit = [5, 6, 5, 4] # Hardcoded na podstawie obliczeń

    # 4. Koszty pracy maszyn (Linia 4)
    # 2$ dla M1, 2$ dla M2, 3$ dla M3
    # Niestety, w pliku masz podane tylko dwie wartości "2 3", co jest nieścisłe.
    # Zakładam, że mają być 3 wartości: [2, 2, 3] 
    machine_hour_cost = [2, 2, 3] 

    # 5. Macierz czasów (Wiersze 5 do końca)
    time_per_kg = Matrix{Int}(undef, n_product, n_machines)

    for i in 1:n_product
        # Wiersze z danymi czasowymi zaczynają się od data_lines[5]
        row_data = parse_line(data_lines[4+i])
        time_per_kg[i, :] = row_data
    end

    println("--- Wczytane Dane ---")
    println("Liczba produktów (n_product): ", n_product)
    println("Liczba maszyn (n_machines): ", n_machines)
    println("Max Popyt (kg): ", max_product_demand)
    println("Zysk Jednostkowy (\$/kg): ", product_profit)
    println("Koszt Maszyn (\$/h): ", machine_hour_cost)
    println("Macierz Czasów (min/kg):\n", time_per_kg)
    println("-----------------------")

    return n_machines, n_product, max_product_demand, product_profit, machine_hour_cost, time_per_kg
end

function createModel(n_machines::Int, n_product::Int, max_product_demand::Array, product_profit::Array, machine_hour_cost::Array, time_per_kg::Array)
    model = Model(HiGHS.Optimizer)

    @variable(model, p[i in 1:n_product, j in 1:n_machines] >= 0) #w minutach

    @constraint(model, max_time[i in 1:n_machines], sum(time_per_kg[j, i]*p[j,i] for j in 1:n_product) <= 3600)

    @constraint(model, max_kgs[i in 1:n_product], sum(p[i, j] for j in 1:n_machines) <=max_product_demand[i])

    @objective(model, Max, sum(product_profit[i]*sum(p[i, j] for j in 1:n_machines) for i in 1:n_product)-sum(machine_hour_cost[i]*sum(p[j, i] * time_per_kg[j, i] / 60 for j in 1:n_product) for i in 1:n_machines))

    optimize!(model)

    println("\n--- Wyniki Optymalizacji Zadanie 2 ---")
    if termination_status(model) == MOI.OPTIMAL
        println("STATUS: OPTYMALNY")
        println("Wartość funkcji celu (zysk netto): ", objective_value(model))

        # Pobranie wartości decyzji: wyprodukowane minuty p[i,j]
        P_val = value.(p)

        println("\nWyprodukowane minuty na produkt i maszynę (p[i,j]):")
        for i in 1:size(P_val, 1)
            for j in 1:size(P_val, 2)
                print(rpad(string(round(P_val[i,j]; digits=3)), 10))
            end
            println()
        end

        # Podsumowanie: ilość kg wyprodukowane dla każdego produktu
        println("\nIlość wyprodukowanego produktu (kg) per produkt:")
        for i in 1:n_product
            total_minutes = sum(P_val[i, j] for j in 1:n_machines)
            # Jeśli p zapisane są w minutach na kg (zgodnie z komentarzem), to zamień na kg
            kg = total_minutes / (sum(time_per_kg[i, :]) / n_machines)  # przybliżone
            println(" Produkt ", i, ": ~", round(kg; digits=3), " kg (min: ", round(total_minutes; digits=3), ")")
        end
    else
        println("STATUS: NIEOPTYMALNY (", termination_status(model), ")")
    end
end


# n_machines = 3
# n_product = 4

# # minuty na 1kg wyrobu i - produkt, j -maszyma
# time_per_kg = [
#     5 10 6;
#     3 6 4;
#     4 5 3;
#     4 2 1
# ]

# max_product_demand = [400, 100, 150, 500]

# product_profit = [5, 6, 5, 4]

# machine_hour_cost = [2, 2, 3]

const DATA_FILE = "dane_produkcja.txt"

# Wczytanie danych z pliku
try
    # Odczyt danych
    n_machines, n_product, max_product_demand, product_profit, machine_hour_cost, time_per_kg = parse_production_data(DATA_FILE)
    
    # Uruchomienie rozwiazania
    createModel(n_machines, n_product, max_product_demand, product_profit, machine_hour_cost, time_per_kg)

catch e
    println("\nBLAD PRZY URUCHAMIANIU MODELU:")
    println(e)
    println("\nUpewnij sie, ze plik $DATA_FILE istnieje i zawiera poprawne dane.")
end