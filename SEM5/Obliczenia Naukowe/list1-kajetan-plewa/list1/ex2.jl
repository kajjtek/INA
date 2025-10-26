# Author: KAJETAN PLEWA

"""
    calculate_hypothesis(T::Type)

Calculates the expression $3 \\times (4/3 - 1) - 1$ using the specified floating-point type `T`.
This operation is designed to illustrate the effect of **machine epsilon** and **floating-point cancellation** (loss of significance) due to rounding. In ideal mathematics, the result should be $0$. In finite 
precision arithmetic, the result often equals the machine epsilon.

# Formal Parameters
- `T::Type`: The floating-point type (e.g., Float16, Float32, Float64) used for all calculations within the function.

# Returns
- `T`: The result of the calculation.
"""
function calculate_hypothesis(T::Type)
    # The expression is: 3 * (4/3 - 1) - 1.
    # T(3.0), T(4.0), T(1.0): Values cast to the specified floating-point type T to ensure 
    # the entire calculation is performed with the finite precision of T.
    return T(3.0) * (T(4.0)/T(3.0)-T(1.0)) - T(1.0)
end

"""
    main()

The main function that executes the `calculate_hypothesis` for Float16, Float32, and Float64 types. 
It compares the result of the calculation with Julia's built-in machine epsilon (`eps(T)`).

# Formal Parameters
- This function takes no parameters.
"""
function main()
    # types: An array containing the floating-point types to be tested.
    types = [Float16, Float32, Float64]

    # Loop that iterates through each type in the 'types' array.
    for T in types
        println("For type: ", T)

        # x: The computed result of the hypothesis calculation for the current type T.
        x = calculate_hypothesis(T)
        
        println("Calculated hypothesis equals: ", x)
        # eps(T): Julia's built-in function to retrieve the machine epsilon for type T.
        print("Is it equal to actual?: ", x == eps(T), "\n")
    end

end

main()