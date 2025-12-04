using Plots
include("ilorazyRoznicowe.jl")
include("normalna.jl")
include("newton.jl")

function select_nodes(a::Float64, b::Float64, n::Int; nodes_type::Symbol)
    
    x_nodes = zeros(Float64, n + 1)
    
    if nodes_type == :equidistant
        h = (b - a) / n
        for k in 0:n
            x_nodes[k + 1] = a + k * h 
        end
        
    elseif nodes_type == :chebyshev
        for k in 0:n
            cos_arg = (2.0 * k + 1.0) * pi / (2.0 * (n + 1.0)) 
            
            xi = cos(cos_arg) 
            
            x_nodes[k + 1] = (a + b) / 2.0 + (b - a) / 2.0 * xi
        end
        
    else
        error("Unknown node type: $nodes_type. Use :equidistant or :chebyshev.")
    end
    
    return x_nodes
end

function plotNewtonInterp(f, a::Float64, b::Float64, n::Int; nodes_type::Symbol = :equidistant)
    
    x_nodes = select_nodes(a, b, n, nodes_type)
    y_nodes = f.(x_nodes) 

    c_coeffs = ilorazyRoznicowe(x_nodes, y_nodes) 
    
    Pn(x) = warNewton(x_nodes, c_coeffs, x)
    
    num_points = 200 
    X_plot = range(a, b, length = num_points)
    
    Y_f = f.(X_plot)
    Y_Pn = Pn.(X_plot)
    
    plot_title = "Newton Interpolation (n=$n, Nodes: $nodes_type)"
    
    p = plot(X_plot, Y_f, 
             label = "Function f(x)", 
             linecolor = :blue, 
             linewidth = 2,
             title = plot_title,
             xlabel = "x",
             ylabel = "y")
             
    plot!(p, X_plot, Y_Pn, 
          label = "Polynomial P_n(x)", 
          linecolor = :red, 
          linestyle = :dash, 
          linewidth = 2)
          
    scatter!(p, x_nodes, y_nodes, 
             label = "Interpolation Nodes", 
             marker = :circle, 
             markersize = 4, 
             markercolor = :black)
             
    return p 
end