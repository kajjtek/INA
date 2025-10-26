# Author: KAJETAN PLEWA

using Printf

"""
    f(x::Float64)::Float64

Defines the function $f(x) = \\sin(x) + \\cos(3x)$.

# Formal Parameters
- `x::Float64`: The input argument for the function.

# Returns
- `Float64`: The computed value of $f(x)$.
"""
f(x::Float64)::Float64 = sin(x) + cos(3.0 * x)

---

"""
    f_prime_exact(x::Float64)::Float64

Calculates the exact analytical derivative of the function $f(x)$: 
$f'(x) = \\cos(x) - 3 \\sin(3x)$.

# Formal Parameters
- `x::Float64`: The point at which the exact derivative is calculated.

# Returns
- `Float64`: The exact value of $f'(x)$.
"""
f_prime_exact(x::Float64)::Float64 = cos(x) - 3.0 * sin(3.0 * x)

---

"""
    f_prime_approx(x0::Float64, h::Float64)::Float64

Calculates the approximate derivative of $f(x)$ at $x_0$ using the **forward finite difference** formula:
$f'(x_0) \\approx \\frac{f(x_0 + h) - f(x_0)}{h}$.

This method introduces two types of error: **truncation error** (due to the approximation) 
and **cancellation error** (for small $h$ when $f(x_0 + h) \\approx f(x_0)$).

# Formal Parameters
- `x0::Float64`: The point around which the derivative is approximated.
- `h::Float64`: The step size (or perturbation) used in the finite difference formula.

# Returns
- `Float64`: The approximate value of $f'(x_0)$.
"""
f_prime_approx(x0::Float64, h::Float64)::Float64 = (f(x0 + h) - f(x0)) / h

---

"""
    numerical_differentiation_experiment()

Performs an experiment to study the error behavior of the forward finite 
difference approximation of the derivative $f'(x_0)$ as the step size $h$ 
is progressively decreased ($h = 2^{-n}$). 
It demonstrates the trade-off between **truncation error** (which decreases with $h$) 
and **cancellation error** (which increases as $h$ approaches machine epsilon).

# Formal Parameters
- This function takes no parameters.
"""
function numerical_differentiation_experiment()
    # x0: The point at which the derivative is calculated.
    x0 = 1.0 
    # n_max: The maximum exponent for the step size $h = 2^{-n}$. $n_{max} = 54$ 
    # ensures $h$ decreases below the machine epsilon for Float64 (approx $2^{-52}$).
    n_max = 54 

    # exact_derivative: The known, precise value of the derivative at x0, calculated analytically.
    exact_derivative = f_prime_exact(x0)
    
    println("------------------------------------------------------------------------------------------------------")
    println("Eksperyment: Przybliżenie pochodnej f(x) = sin(x) + cos(3x) w x0 = 1.0")
    println("Exact value of  f'(1.0): ", exact_derivative)
    
    # Loop iterates n from 0 to n_max, progressively making the step size h smaller.
    for n in 0:n_max
        # h: The current step size, calculated as $2^{-n}$.
        h = 2.0^(-n)
        # approx_derivative: The derivative approximated using the finite difference method with step h.
        approx_derivative = f_prime_approx(x0, h)
        # absolute_error: The absolute difference between the exact and approximate derivative.
        absolute_error = abs(exact_derivative - approx_derivative)
        # percentage: The absolute error expressed as a percentage of the exact value.
        percentage = abs(absolute_error / exact_derivative * 100)
        
        # x0_plus_h: The perturbed point $x_0 + h$. Observing this value helps show 
        # when $x_0 + h$ stops changing (i.e., when $h$ is smaller than $\\text{eps}(x_0)$).
        x0_plus_h = x0 + h

        @printf("| %2d | %1.16e | %1.16e | %1.16e | %1.16e | %1.16e |\n",
                n, h, approx_derivative, absolute_error, x0_plus_h, percentage)

    end
end

numerical_differentiation_experiment()