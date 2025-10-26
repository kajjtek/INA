# KAJETAN PLEWA

function first(T::Type, x, y)
    x = T.(x)
    y = T.(y)
    sum = T(0.0)
    for i in 1:5
        sum+=T(x[i])*T(y[i])
    end
    return sum
end

function sum_forward(terms, T::Type)
    S = T(0.0)
    for i in 1:length(terms)
        S = S + terms[i]
    end
    return S
end

function second(T::Type, x, y)
    x = T.(x)
    y = T.(y)
    sum = T(0.0)
    for i in 5:-1:1
        sum+=T(x[i])*T(y[i])
    end
    return sum
end

function third(T::Type, x, y)
    x = T.(x)
    y = T.(y)

    products = T.(x) .* T.(y)

    positive = products[products .> T(0.0)]
    negative = products[products .< T(0.0)]

    sort!(positive, by=abs,rev=true)
    sort!(negative, by=abs,rev=true)

    spos = sum_forward(positive, T)
    sneg = sum_forward(negative, T)

    println("Sum of positive: ",spos, " Sum of negative: ", sneg)

    return spos + sneg
end

function fourth(T::Type, x, y)
    x = T.(x)
    y = T.(y)

    products = x .* y

    positive = products[products .> T(0.0)]
    negative = products[products .< T(0.0)]

    sort!(positive, by=abs,rev=false)
    sort!(negative, by=abs,rev=false)

    spos = sum_forward(positive, T)
    sneg = sum_forward(negative, T)

    return spos + sneg
end

function main()
    x = [2.718281828, −3.141592654, 1.414213562, 0.5772156649, 0.3010299957]
    y = [1486.2497, 878366.9879, −22.37492, 4773714.647, 0.000185049]

    functions = [first, second, third, fourth]

    types = [Float32, Float64]

    for T in types
        println("Type: ", T)
        for f in functions
            result = f(T, x, y)
            println("Result for function ", f," is ", result)
        end
    end

end

main()