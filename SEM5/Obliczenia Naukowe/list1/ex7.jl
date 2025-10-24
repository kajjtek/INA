# KAJETAN PLEWA

using Printf

f(x::Float64)::Float64 = sin(x) + cos(3.0 * x)

f_prime_exact(x::Float64)::Float64 = cos(x) - 3.0 * sin(3.0 * x)

f_prime_approx(x0::Float64, h::Float64)::Float64 = (f(x0 + h) - f(x0)) / h

function numerical_differentiation_experiment()
    x0 = 1.0 
    n_max = 54 

    exact_derivative = f_prime_exact(x0)
    
    println("------------------------------------------------------------------------------------------------------")
    println("Eksperyment: Przybliżenie pochodnej f(x) = sin(x) + cos(3x) w x0 = 1.0")
    println("Exact value of  f'(1.0): ", exact_derivative)

    for n in 0:n_max
        h = 2.0^(-n)
        approx_derivative = f_prime_approx(x0, h)
        absolute_error = abs(exact_derivative - approx_derivative)
        
        x0_plus_h = x0 + h

        @printf("| %2d | %1.16e | %1.16e | %1.16e | %1.16e |\n",
                n, h, approx_derivative, absolute_error, x0_plus_h)

    end
end

numerical_differentiation_experiment()