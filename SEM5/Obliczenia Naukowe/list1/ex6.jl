# KAJETAN PLEWA

function f(x::Float64)::Float64
    return sqrt(x^2 + 1.0) - 1.0
end

function g(x::Float64)::Float64
    return x^2 / (sqrt(x^2 + 1.0) + 1.0)
end

function calculate_and_compare()

    for i in 1:15
        x = 8.0^(-i)
        fx = f(x)
        gx = g(x)

        relative_diff = abs(fx - gx) / gx

        # Formatowanie wyjścia dla lepszej czytelności
        println("| ", lpad(i, 1), " | ",
                rpad(string(x), 17), " | ",
                rpad(string(fx), 19), " | ",
                rpad(string(gx), 17), " | ",
                rpad(string(relative_diff), 17), " |")

        if fx == 0.0 && i > 1
             break
        end
    end
end

calculate_and_compare()