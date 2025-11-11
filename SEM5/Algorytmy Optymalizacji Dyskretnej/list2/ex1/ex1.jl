using JuMP
import HiGHS
using DelimitedFiles

function parse_data(filename::String)
    # Wczytanie wszystkich linii z pliku jako tekst (readlines)
    data_lines = readlines(filename)
    
    # Funkcja pomocnicza do parsowania linii i ignorowania pustych elementów
    parse_line(line) = parse.(Int, filter!(!isempty, split(line, ' ')))

    # 1. Liczba firm i lotnisk (Linia 1)
    dimensions = parse_line(data_lines[1])
    n_companies = dimensions[1]
    n_airports = dimensions[2]

    # 2. Limity maksymalne firm (Linia 2)
    max_limits = parse_line(data_lines[2])
    
    # 3. Limity minimalne lotnisk (Linia 3)
    min_limits = parse_line(data_lines[3])
    
    # 4. Macierz kosztów (Linie 4 do końca)
    c_matrix_data = data_lines[4:end]
    c_matrix = Matrix{Int}(undef, n_airports, n_companies)

    for i in 1:n_airports
        row_data = parse_line(c_matrix_data[i])
        c_matrix[i, :] = row_data
    end

    println("--- Wczytane Dane ---")
    println("Liczba firm (n_companies): ", n_companies)
    println("Liczba lotnisk (n_airports): ", n_airports)
    println("Max Limity Firm: ", max_limits)
    println("Min Limity Lotnisk: ", min_limits)
    println("Macierz Kosztów (c):\n", c_matrix)
    println("-----------------------")

    return c_matrix, max_limits, min_limits, n_companies, n_airports
end

function createModel(c::Array, max_limits::Array, min_limits::Array, n_companies::Int, n_airports::Int)
    model = Model(HiGHS.Optimizer)

    @variable(model, x[i in 1:n_airports, j in 1:n_companies] >= 0)

    @constraint(model, comp_limit[i in 1:n_companies], sum(x[j,i] for j in 1:n_airports) <= max_limits[i])

    @constraint(model, air_limit[i in 1:n_airports],sum(x[i,j] for j in 1:n_companies) >= min_limits[i])

    @objective(model, Min, sum(c[i,j]*x[i,j] for i in 1:n_airports, j in 1:n_companies))

    optimize!(model)

    println("\n--- Wyniki Optymalizacji Zadanie 1 ---")
    if termination_status(model) == MOI.OPTIMAL
        println("STATUS: OPTYMALNY")
        
        # (a) Minimalny koszt
        min_cost = objective_value(model)
        println("a) Minimalny łączny koszt dostaw: ", min_cost)

        X_val = value.(x)
        
        # Całkowita dostawa wykorzystana przez każdą firmę (j)
        supply_used = [sum(X_val[i, j] for i in 1:n_airports) for j in 1:n_companies]
        
        # (b) Sprawdzenie, czy wszystkie firmy dostarczają paliwo
        all_deliver = all(supply_used .> 1.0) # Sprawdzamy czy przepływ > 0
        println("\nb) Czy wszystkie firmy dostarczają paliwo? ", all_deliver ? "TAK" : "NIE")
        
        # (c) Sprawdzenie, czy możliwości dostaw są wyczerpane
        println("\nc) Czy możliwości dostaw paliwa przez firmy są wyczerpane?")
        for j in 1:n_companies
            limit_used = supply_used[j]
            limit_max = max_limits[j]
            # Używamy tolerancji 1e-5 ze względu na błędy numeryczne
            exhausted = isapprox(limit_used, limit_max, atol=1e-5) 
            
            println("   Firma ", j, ": Wykorzystano ", round(Int, limit_used), "/", limit_max, " galonów. ", exhausted ? "(Wyczerpano)" : "(Nie wyczerpano)")
        end
        
        # Optymalny plan dostaw (x[lotnisko, firma])
        println("\nOptymalny plan dostaw (x[lotnisko, firma]):")
        display(round.(Int, X_val))

    else
        println("STATUS: NIEOPTYMALNY (", termination_status(model), ")")
    end
end 

# c = [
#     10 7 8;
#     10 11 14;
#     9 12 4;
#     11 13 9
# ]

# min_limits = [1.1e5, 2.2e5, 3.3e5, 4.4e5]
# max_limits = [2.75e5, 5.5e5, 6.6e5]



# n_companies = 3
# n_airports = 4

const DATA_FILE = "./assignment_data.txt"


try
    C, Max_L, Min_L, N_C, N_A = parse_data(DATA_FILE)
    
    createModel(C, Max_L, Min_L, N_C, N_A)

catch e
    println("\nBLAD PRZY URUCHAMIANIU MODELU:")
    println(e)
    println("\nUpewnij sie, ze plik $DATA_FILE istnieje i zawiera poprawne dane.")
end