using Plots
using CSV
using DataFrames

# Wczytanie danych z pliku wygenerowanego przez tester.jl
function generate_plots(csv_path="testfiles/czasy.csv")
    if !isfile(csv_path)
        println("Błąd: Nie znaleziono pliku $csv_path. Uruchom najpierw tester.jl")
        return
    end

    # Wczytanie danych do ramki danych
    df = CSV.read(csv_path, DataFrame)
    
    # Konwersja kolumny Plik na typ numeryczny (wyciąganie n)
    df.n = [parse(Int, split(string(name), "_")[1]) for name in df.Plik]
    
    # Unikalne scenariusze (z_pliku / wyliczone)
    scenarios = unique(df.Scenariusz)
    
    # Konfiguracja serii danych (Metoda, Pivot, Kolor, Styl)
    configs = [
        ("Gauss", false, :blue, :solid), 
        ("Gauss", true, :blue, :dash), 
        ("LU", false, :red, :solid), 
        ("LU", true, :red, :dash)
    ]
    
    for scen in scenarios
        sub_df = df[df.Scenariusz .== scen, :]
        
        # --- WYKRES CZASU ---
        p_time = plot(
            title = "Czas wykonania - Scenariusz: $scen",
            xlabel = "Rozmiar macierzy (n)",
            ylabel = "Czas (s)",
            xscale = :log10,
            yscale = :log10,
            legend = :topleft,
            grid = true,
            marker = :circle,
            dpi = 300
        )
        
        # --- WYKRES BŁĘDU ---
        p_error = plot(
            title = "Błąd względny - Scenariusz: $scen",
            xlabel = "Rozmiar macierzy (n)",
            ylabel = "Błąd ||x - x*|| / ||x*||",
            xscale = :log10,
            yscale = :log10,
            legend = :outerright,
            grid = true,
            marker = :square,
            dpi = 300
        )
        
        for (meth, piv, col, style) in configs
            data = sub_df[(sub_df.Metoda .== meth) .& (sub_df.Pivot .== piv), :]
            sort!(data, :n)
            
            if !isempty(data)
                label = "$meth (pivot=$piv)"
                
                # Dodaj do wykresu czasu
                plot!(p_time, data.n, data.Czas_s, 
                    label = label, color = col, linestyle = style,
                    linewidth = 2, markersize = 4)
                
                # Dodaj do wykresu błędu (zastępujemy 0 małą wartością do skali log)
                clean_err = [max(e, 1e-18) for e in data.Blad]
                plot!(p_error, data.n, clean_err, 
                    label = label, color = col, linestyle = style,
                    linewidth = 2, markersize = 4)
            end
        end
        
        # Zapisywanie wykresów
        savefig(p_time, "testfiles/wykres_czas_$(scen).png")
        savefig(p_error, "testfiles/wykres_blad_$(scen).png")
        
        println("Wygenerowano wykresy dla scenariusza: $scen")
    end
end

# Uruchomienie generowania
generate_plots()