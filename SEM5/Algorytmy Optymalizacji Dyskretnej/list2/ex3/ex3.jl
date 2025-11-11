using JuMP
import HiGHS

using DelimitedFiles

function parse_inventory_data(filename::String)
    # Wczytanie wszystkich linii z pliku jako tekst
    data_lines = readlines(filename)
    
    # Funkcja pomocnicza do parsowania linii Int
    parse_line(line) = parse.(Int, filter!(!isempty, split(line, ' ')))

    # 1. Stałe i stan początkowy (Linia 1)
    constants = parse_line(data_lines[1])
    n_intervals = constants[1] # K = 4
    capacity = constants[2]    # 70 jednostek
    I0 = constants[3]          # 15 jednostek

    # 2. Jednostkowy koszt produkcji normalnej c_j (Linia 2)
    production_cost = parse_line(data_lines[2])
    
    # 3. Maksymalna ilość produkcji nadwymiarowej a_j (Linia 3)
    max_overflow = parse_line(data_lines[3])
    
    # 4. Jednostkowy koszt produkcji nadwymiarowej o_j (Linia 4)
    cost_overflow = parse_line(data_lines[4])

    # 5. Zapotrzebowanie d_j (Linia 5)
    demand = parse_line(data_lines[5])

    println("--- Wczytane Dane ---")
    println("Liczba okresów (K): ", n_intervals)
    println("Pojemność magazynu: ", capacity)
    println("Stan początkowy (I0): ", I0)
    println("Koszt prod. normalnej (c): ", production_cost)
    println("Max nadprodukcja (a): ", max_overflow)
    println("Koszt nadprodukcji (o): ", cost_overflow)
    println("Zapotrzebowanie (d): ", demand)
    println("-----------------------")

    return n_intervals, production_cost, max_overflow, cost_overflow, demand, capacity, I0
end



function createModel(n_intervals::Int, production_cost::Array, max_overflow::Array, cost_overflow::Array, demand::Array, capacity::Int, I0::Int)
    model = Model(HiGHS.Optimizer)

    @variable(model, 0<= x[i in 1:n_intervals] <= 100)
    @variable(model, 0<= y[i in 1:n_intervals] <= max_overflow[i])
    @variable(model, 0<= I[i in 1:n_intervals] <=capacity)

    @constraint(model, bilans_init, I[1] == I0 + x[1] + y[1] - demand[1])

    @constraint(model, bilans[i in 2:n_intervals], I[i] == I[i-1] + x[i] + y[i] - demand[i])

    @objective(model, Min, sum(x[i]*production_cost[i] + y[i]*cost_overflow[i] + 1500*I[i] for i in 1:n_intervals))


    optimize!(model)

    println("\n--- Wyniki Modelu Magazynowego ---")
    status = termination_status(model)
    if status == MOI.OPTIMAL
        println("STATUS: OPTYMALNY")
        println("Wartość celu (całkowity koszt):", objective_value(model))

        x_val = value.(x)
        y_val = value.(y)
        I_val = value.(I)

        println("\nPer-okresowe wyniki:")
        println(rpad("okres",6), rpad("x (prod)",12), rpad("y (over)",12), rpad("I (inv)",10), rpad("d (demand)",10))
        for t in 1:n_intervals
            println(rpad(string(t),6), rpad(string(round(x_val[t]; digits=3)),12), rpad(string(round(y_val[t]; digits=3)),12), rpad(string(round(I_val[t]; digits=3)),10), rpad(string(demand[t]),10))
        end

        total_x = sum(x_val)
        total_y = sum(y_val)
        println("\nSuma produkcji normalnej (x): ", round(total_x; digits=3))
        println("Suma nadprodukcji (y): ", round(total_y; digits=3))
        println("Stan końcowy magazynu I[K]: ", round(I_val[end]; digits=3))
    else
        println("STATUS: NIEOPTYMALNY (", status, ")")
    end
end 

const DATA_FILE = "inventory_data.txt"

# Wczytanie danych z pliku
try
    # Odczyt danych
    n_intervals, production_cost, max_overflow, cost_overflow, demand, capacity, I0 = parse_inventory_data(DATA_FILE)
    
    # Uruchomienie rozwiazania
    createModel(n_intervals, production_cost, max_overflow, cost_overflow, demand, capacity, I0)

catch e
    println("\nBLAD PRZY URUCHAMIANIU MODELU:")
    println(e)
    println("\nUpewnij sie, ze plik $DATA_FILE istnieje i zawiera poprawne dane.")
end

# n_intervals = 4

# production_cost = [6000, 4000, 8000, 9000]
# max_overflow = [60, 65, 70, 60]
# cost_overflow = [8000, 6000, 10000, 11000]
# demand = [130, 80, 125, 195]
# capacity = 70
# I0 = 15

