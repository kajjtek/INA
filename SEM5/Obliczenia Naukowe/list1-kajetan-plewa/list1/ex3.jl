# Author: KAJETAN PLEWA

"""
    analyse_gap(T::Type, flag::Bool)

Analyzes the spacing between consecutive floating-point numbers in the interval 
$[1.0, 2.0]$ for the specified type `T`. It verifies whether adding the machine 
epsilon (`eps(T)`) to a number correctly yields the next representable floating-point number 
in that range. This is a common test to show that the absolute gap between numbers is $\\text{eps}(T)$ 
in the range $[1.0, 2.0)$.

# Formal Parameters
- `T::Type`: The floating-point type (e.g., Float64) to be analyzed.
- `flag::Bool`: A boolean flag that controls the function's mode:
    - If `true`, it prints the results of each step and returns `true` at the end.
    - If `false`, it stops and returns `false` immediately upon the first mismatch 
      (where `current` is not equal to `nextfloat(prev)`).

# Returns
- `Bool`: Returns `true` if `flag` is true, or if `flag` is false and all steps 
          matched. Returns `false` if `flag` is false and a mismatch occurred.
"""
function analyse_gap(T::Type, flag)
    # macheps: The machine epsilon for type T, representing the smallest number that can be added to 1.0 to change its value.
    macheps = eps(T)

    # prev: Stores the previous floating-point number, initialized to 1.0.
    prev = T(1.0)
    # current: Stores the current floating-point number, initialized to 1.0.
    current = T(1.0)

    # Loop continues as long as 'current' is less than 2.0.
    while(current < T(2.0))
        # prev: Stores the value of 'current' before it's incremented.
        prev = current
        # current: The number after adding 'macheps' to the previous number.
        current += macheps

        # Conditional block based on the 'flag' parameter.
        if flag
            # nextfloat(prev): The next exactly representable floating-point number after 'prev'.
            println("Next number: ", current, " Is it equal to expected?: ", nextfloat(prev) == current)
        else
            # Check for a mismatch immediately.
            if nextfloat(prev) != current
                return false
            end
        end
    end 

    # If the loop finishes (either mode) and flag is true, return true.
    if flag
        return true
    end

end

---

"""
    second_part()

Analyzes the absolute spacing between consecutive Float64 numbers (`nextfloat(x) - x`) 
at different intervals ($[0.5, 1.0]$ and $[2.0, 4.0]$). It also displays the binary 
(bitstring) representation of the numbers to illustrate how the exponent and mantissa 
change across the power-of-two boundaries.

# Formal Parameters
- This function takes no parameters.
"""
function second_part()
    # intervals: A list of intervals (tuples) to analyze.
    intervals = [[0.5, 1.0], [2.0, 4.0]]

    # Loop that iterates through each interval.
    for interval in intervals
        println("For interval: ", interval)
        # x1: The starting point of the interval, cast to Float64.
        x1 = Float64(interval[1])
        # x2: The end point of the interval, cast to Float64.
        x2 = Float64(interval[2])
        # med: The difference between the endpoints (x2 - x1).
        med = Float64(x2 - x1)


        # tab: A list of specific values within or near the interval to analyze their spacing.
        tab = [x1, x2, med]

        # Loop that iterates through the specific values in 'tab'.
        for x in tab
            # next: The next representable Float64 number after x.
            next = nextfloat(x)
            
            println("The difference between ", x, " and ", next, " equals: ", Float64(next - x))
            
            println("Bit version of ", x)
            # bitstring(x): Displays the 64-bit binary representation of x.
            println(bitstring(x))
            
            println("Bit version of nextfloat", next)
            # bitstring(next): Displays the 64-bit binary representation of 'next'.
            println(bitstring(next))
        end
    end
end

---

"""
    main()

The main function that sets up the environment and calls the analysis functions.

# Formal Parameters
- This function takes no parameters.
"""
function main()
    # types: An array of floating-point types to test with analyse_gap.
    types = [Float64]
    # flag: The mode flag passed to analyse_gap.
    flag = true

    # Loop that iterates through each type.
    for T in types
        println("For type: ", T)
        # x: The result of the analyse_gap function.
        x = analyse_gap(T, flag)

        # Print the final result only if the 'flag' was set to false (non-printing mode).
        if !flag
            println("Have all been equal?: ", x)
        end 
    end

end

second_part()
main()