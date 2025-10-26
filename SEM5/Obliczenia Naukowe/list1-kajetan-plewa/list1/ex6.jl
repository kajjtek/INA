# Author: KAJETAN PLEWA

"""
    f(x::Float64)::Float64

Calculates the value of the function $f(x) = \\sqrt{x^2 + 1.0} - 1.0$.

This function is prone to **catastrophic cancellation** when $x$ is close to $0$, 
because $\\sqrt{x^2 + 1.0}$ is very close to $1.0$, and subtracting these two 
nearly equal numbers results in a significant loss of precision.

# Formal Parameters
- `x::Float64`: The input argument for the function.

# Returns
- `Float64`: The computed value of $f(x)$.
"""
function f(x::Float64)::Float64
    return sqrt(x^2 + 1.0) - 1.0
end

---

"""
    g(x::Float64)::Float64

Calculates the value of the function $g(x) = \\frac{x^2}{\\sqrt{x^2 + 1.0} + 1.0}$.

This function is mathematically equivalent to $f(x)$ but is **numerically stable** for small values of $x$. It avoids catastrophic cancellation by multiplying the 
numerator and denominator of $f(x)$ by the conjugate $\\sqrt{x^2 + 1.0} + 1.0$.

# Formal Parameters
- `x::Float64`: The input argument for the function.

# Returns
- `Float64`: The computed value of $g(x)$.
"""
function g(x::Float64)::Float64
    return x^2 / (sqrt(x^2 + 1.0) + 1.0)
end

---

"""
    calculate_and_compare()

Compares the numerical results of the unstable function $f(x)$ and the 
stable function $g(x)$ for decreasing values of $x$ ($x = 8^{-i}$). It calculates 
and prints the **relative difference** between the two results, illustrating 
the point at which cancellation error in $f(x)$ becomes significant.

# Formal Parameters
- This function takes no parameters.
"""
function calculate_and_compare()

    # Loop iterates from i = 1 to 15.
    for i in 1:15
        # x: The input value for the current iteration, calculated as $8^{-i}$.
        x = 8.0^(-i)
        # fx: The result of the numerically unstable function f(x).
        fx = f(x)
        # gx: The result of the numerically stable function g(x).
        gx = g(x)

        # relative_diff: The measure of error in f(x) relative to the more accurate result g(x).
        # Calculated as $\\frac{|f(x) - g(x)|}{g(x)}$.
        relative_diff = abs(fx - gx) / gx

        # Printing the results in a formatted table layout.
        println("| ", lpad(i, 1), " | ",
                  rpad(string(x), 17), " | ",
                  rpad(string(fx), 19), " | ",
                  rpad(string(gx), 17), " | ",
                  rpad(string(relative_diff), 17), " |")

        # Break condition: If f(x) underflows to 0.0 (and it's not the very first iteration), stop the loop.
        if fx == 0.0 && i > 1
              break
        end
    end
end

calculate_and_compare()