#KAJETAN PLEWA
using Plots
using Printf

function quadratic_map(p0::Float64, c::Float64)
    p = p0
    history = [p]

    for i in 1:40
        p = p*p + c

        if(p>1e10 || p<-1e10)
            println("Divergence detected at iteration ", i)
            break
        end

        push!(history, p)
    end 
    return (p, history)
end


function stworz_diagram_pajeczynowy(c::Float64, x0::Float64, n_iteracji::Int; plik_wyjsciowy::Union{String, Nothing}=nothing)
    
    x_seq = zeros(Float64, n_iteracji + 1)
    x_seq[1] = x0
    for i in 1:n_iteracji
        x_seq[i+1] = x_seq[i]*x_seq[i] + c
    end

    println(x_seq);

    path_x = [x0]
    path_y = [0.0]
    for i in 1:n_iteracji
        push!(path_x, x_seq[i])
        push!(path_y, x_seq[i+1])
        push!(path_x, x_seq[i+1])
        push!(path_y, x_seq[i+1])
    end

    min_val = min(minimum(x_seq), 0.0)
    max_val = max(maximum(x_seq), 0.0)
    margin = (max_val - min_val) * 0.15
    zakres_x = (min_val - margin, max_val + margin)
    zakres_y = (min_val - margin, max_val + margin)

    
    p = Plots.plot(
        title = @sprintf("Diagram pajęczynowy: c=%.3f, x₀=%.3f", c, x0),
        xlabel = "xₙ",
        ylabel = "xₙ₊₁",
        aspect_ratio = :equal,
        legend = :outertopright,
        xlims = zakres_x,
        ylims = zakres_y
    )

    f(x) = x^2 + c
    Plots.plot!(p, f, zakres_x..., label="y = x² + c", lw=2, color=:blue)
    Plots.plot!(p, x -> x, zakres_x..., label="y = x", lw=2, color=:red, linestyle=:dash)
    Plots.plot!(p, path_x, path_y, label="Iteracje", lw=1.5, color=:green, markershape=:circle, markersize=2)

   
    Plots.scatter!(p, [x0], [0], color=:purple, markersize=5, label="")
    
    
    y_offset = (zakres_y[2] - zakres_y[1]) * 0.04
    
   
    Plots.annotate!(p, [(x0, -y_offset, Plots.text("x₀", 10, :center, :top, :purple))])

  
    if plik_wyjsciowy !== nothing
        Plots.savefig(p, plik_wyjsciowy)
        println("Wykres został zapisany do pliku: $plik_wyjsciowy")
    end
    
    display(p)
end

function main()
    test_cases = [
        (-2.0, 1.0),
        (-2.0, 2.0),
        (-2.0, 1.99999999999999),
        (-1.0, 1.0),
        (-1.0, -1.0),
        (-1.0, 0.75),
        (-1.0, 0.25)
    ]

    for (i, (c, x0)) in enumerate(test_cases)

        stworz_diagram_pajeczynowy(Float64(c), Float64(x0), 40,  plik_wyjsciowy="spider_diagram_c_$(replace(string(c), "." => "_"))_x0_$(replace(string(x0), "." => "_")).png")
        
        println("\n=== Experiment $(i): c = $c, x0 = $x0 ===")
        
    end
end 

main()
