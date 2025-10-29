function quadratic_map(p0::Float64, c::Float64)
    p = p0
    history = [p]

    for i in 1:40
        p = p*p + c

        if abs(p) > 1e10 
            push!(history, Inf) 
            break 
        end
        push!(history, p)
    end 
    return (p, history)
end

function main()
    test_cases = [
        (-2.0, 1.0),
        (-2.0, 2.0),
        (-2.0, 1.9999999999999999),
        (-1.0, 1.0),
        (-1.0, -1.0),
        (-1.0, 0.75),
        (-1.0, 0.25)
    ]

    for (i, (c, x0)) in enumerate(test_cases)
        (result ,results) = quadratic_map(c, x0)
        
        println("\n=== Experiment $(i): c = $c, x0 = $x0 ===")

        
        println("First five arguments of the series (x0 to x5): ", results[1:min(6, length(results))])
        if length(results) > 10
            println("The end of the series (x35 to x40): ", results[max(1, length(results)-5):end])
        end
        
    end
end 

main()