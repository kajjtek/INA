# KAJETAN PLEWA

function machine_epsilon_finder(T::Type)

    epsilon = T(1.0)

    while T(1.0)<T(1.0+epsilon)
        epsilon = epsilon/2
    end
    
    return 2*epsilon
end

function eta_finder(T::Type)

    eta = T(1.0)

    while eta>T(0.0)
        eta = eta/2
    end
    
    return 2*eta
end

function max_finder(T::Type)
    x = T(1.0)
    while isfinite(2 * x)
        x *= T(2.0)
    end
   
    return prevfloat(2 * x)
end


function main()
    types = [Float16, Float32, Float64]

    for T in types
        computed_m = machine_epsilon_finder(T)
        builtin_m = eps(T)

        computed_e = eta_finder(T)
        builtin_e = nextfloat(T(0.0))

        computed_max = eta_finder(T)
        builtin_max = nextfloat(T(0.0))
    

        println("Type: ", T, " Computed macheps: ", computed_m, " Built-in macheps: ", builtin_m)
        println("Are they equal? : ", computed_m==builtin_m)

        println("Type: ", T, " Computed eta: ", computed_e, " Built-in eta: ", builtin_e)
        println("Are they equal? : ", computed_e==builtin_e)

        println("Type: ", T, " Computed max: ", computed_max, " Built-in max: ", builtin_max)
        println("Are they equal? : ", computed_max==builtin_max)
    end

    println("Funkcja floatmin dla Float32: ", floatmin(Float32))
    println("Funkcja floatmin dla Float64: ", floatmin(Float64))
end


main()