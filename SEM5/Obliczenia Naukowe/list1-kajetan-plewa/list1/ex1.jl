# Author: KAJETAN PLEWA

"""
    machine_epsilon_finder(T::Type)

Calculates the **machine epsilon** for the specified floating-point type `T`. 
Machine epsilon is defined as the smallest positive number `ε` such that `1.0 + ε > 1.0`.

# Formal Parameters
- `T::Type`: The floating-point type (e.g., Float16, Float32, Float64) for which 
             the machine epsilon should be calculated.
"""
function machine_epsilon_finder(T::Type)

    # epsilon: The current value being tested, initialized to 1.0. It is repeatedly divided by 2.
    epsilon = T(1.0)

    # The loop continues as long as adding epsilon to 1.0 results in a value greater than 1.0 
    # due to the floating-point precision of type T.
    while T(1.0) < T(1.0 + epsilon)
        epsilon = epsilon / 2
    end
    
    # Returns 2 * epsilon, since the loop exits when epsilon is already too small (i.e., 1.0 + epsilon == 1.0).
    return 2 * epsilon
end

"""
    eta_finder(T::Type)

Calculates the smallest **positive normalized** floating-point number (`eta`, often 
`nextfloat(0.0)` in Julia) for the given type `T`. It finds the smallest number 
greater than zero representable before underflow to zero occurs.

# Formal Parameters
- `T::Type`: The floating-point type (e.g., Float16, Float32, Float64).
"""
function eta_finder(T::Type)

    # eta: The current value being tested, initialized to 1.0. It is repeatedly divided by 2.
    eta = T(1.0)
    # last: Stores the last known non-zero value of 'eta' before underflow to zero. 
    # This value is the result of the function.
    last = 0.0

    # The loop continues as long as 'eta' is strictly greater than zero.
    while eta > T(0.0)
        last = eta
        eta = eta / 2
    end
    
    # Returns the last value of 'eta' that was successfully represented as non-zero.
    return last
end

"""
    max_finder(T::Type)

Calculates the **largest finite** floating-point number (`floatmax`) that can 
be represented by the given type `T`.

# Formal Parameters
- `T::Type`: The floating-point type (e.g., Float16, Float32, Float64).
"""
function max_finder(T::Type)
    # x: The current value being tested, initialized to 1.0. It is repeatedly multiplied by 2.
    x = T(1.0)
    
    # The loop continues as long as multiplying x by 2 results in a finite number 
    # (i.e., not Infinity).
    while isfinite(2 * x)
        x *= T(2.0)
    end
    
    # Returns the last finite value of x before overflow to Infinity occurred.
    return x
end


"""
    main()

The main function that orchestrates the calculation of machine epsilon, eta, and 
maximum value for Float16, Float32, and Float64 types. It compares the computed 
results with Julia's built-in functions.

# Formal Parameters
- This function takes no parameters.
"""
function main()
    # types: An array containing the floating-point types to be tested.
    types = [Float16, Float32, Float64]

    # Loop that iterates through each type in the 'types' array.
    for T in types
        # computed_m: The machine epsilon calculated by machine_epsilon_finder.
        computed_m = machine_epsilon_finder(T)
        # builtin_m: The machine epsilon obtained using Julia's built-in 'eps' function.
        builtin_m = eps(T)

        # computed_e: The smallest positive number (eta) calculated by eta_finder.
        computed_e = eta_finder(T)
        # builtin_e: The smallest positive number (eta) obtained using Julia's built-in 'nextfloat(0.0)' function.
        builtin_e = nextfloat(T(0.0))

        # computed_max: The largest finite number calculated by max_finder.
        computed_max = max_finder(T)
        # builtin_max: The largest finite number obtained using Julia's built-in 'floatmax' function.
        builtin_max = floatmax(T(0.0))
    

        println("Type: ", T, " Computed macheps: ", computed_m, " Built-in macheps: ", builtin_m)
        println("Are they equal? : ", computed_m==builtin_m)

        println("Type: ", T, " Computed eta: ", computed_e, " Built-in eta: ", builtin_e)
        println("Are they equal? : ", computed_e==builtin_e)

        println("Type: ", T, " Computed max: ", computed_max, " Built-in max: ", builtin_max)
        println("Are they equal? : ", computed_max==builtin_max)
    end

    # Display built-in floatmin values for comparison.
    println("Funkcja floatmin dla Float32: ", floatmin(Float32))
    println("Funkcja floatmin dla Float64: ", floatmin(Float64))
end


main()