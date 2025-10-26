# Author: KAJETAN PLEWA

"""
    find_smallest_ex4(T::Type)

Searches for the **smallest floating-point number $x$** in the range 
$[1.0 + \\text{eps}(T), 2.0)$ such that the expression $x \\times (1.0/x)$ 
does not equal $1.0$ due to **floating-point arithmetic errors** (rounding). 
This demonstrates that $x \\times (1/x) \\neq 1$ even for numbers close to $1.0$. 
The function iterates with a step size of $\\text{eps}(T)$.

# Formal Parameters
- `T::Type`: The floating-point type (e.g., Float64) to be analyzed.

# Returns
- `T`: The smallest floating-point number $x$ in the range that satisfies the 
       condition $x \\times (1.0/x) \\neq 1.0$. Returns `nothing` if no such 
       number is found within the tested range.
"""
function find_smallest_ex4(T::Type)
    # macheps: The machine epsilon for type T, which is used as the step size for iteration.
    macheps = eps(T)
    # current: The floating-point number currently being tested, initialized to 1.0 + macheps.
    current = T(1.0) + T(macheps)

    # Loop continues as long as 'current' is less than 2.0.
    while current < T(2.0)
        # Check if the calculated value of x * (1/x) is not exactly 1.0.
        # This occurs when rounding during the calculation of 1.0/current or 
        # the final multiplication causes a loss of precision.
        if current * (T(1.0) / current) != T(1.0)
            return current
        end
        # Increment the current value by one machine epsilon (which is the spacing 
        # between numbers in the [1.0, 2.0) range).
        current += macheps
    end
end

"""
    main()

The main function that orchestrates the search for the smallest number $x$ 
for which $x \\times (1/x) \\neq 1.0$ for the specified floating-point types.

# Formal Parameters
- This function takes no parameters.
"""
function main()
    # types: An array containing the floating-point types to be tested.
    types = [Float64]

    # Loop that iterates through each type in the 'types' array.
    for T in types
        println("For type: ", T)

        # x: The result returned by find_smallest_ex4.
        x = find_smallest_ex4(T)

        println("The smalles number found: ", x)
    end

end

main()