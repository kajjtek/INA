# KAJETAN PLEWA

function find_smallest_ex4(T::Type)
    macheps = eps(T)
    current = T(1.0)+T(macheps)

    while current < T(2.0)
        if current * (T(1.0)/current)!=T(1.0)
            return current
        end
        current+=macheps
    end
end

function main()
    types = [Float64]

    for T in types
        println("For type: ", T)

        x = find_smallest_ex4(T)

        println("The smalles number found: ", x)
    end

end

main()