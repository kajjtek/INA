# KAJETAN PLEWA

function calculate_hypothesis(T::Type)
    return T(3.0) * (T(4.0)/T(3.0)-T(1.0)) - T(1.0)
end

function main()
    types = [Float16, Float32, Float64]

    for T in types
        println("For type: ", T)

        x = calculate_hypothesis(T)
        println("Calculated hypothesis equals: ", x)
        print("Is it equal to actual?: ", x==eps(T), "\n")
    end

end

main()