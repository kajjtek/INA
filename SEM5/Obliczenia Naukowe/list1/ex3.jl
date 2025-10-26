# KAJETAN PLEWA

function analyse_gap(T::Type, flag)
    macheps = eps(T)

    prev = T(1.0)
    current=T(1.0)

    while(current<T(2.0))
        prev=current
        current+=macheps

        if flag
            println("Next number: ",current," Is it equal to expected?: ", nextfloat(prev)==current)
        else
            if nextfloat(prev)!=current
                return false
            end
        end
    end 

    if flag
        return true
    end

end

function second_part()
    intervals = [[0.5, 1.0], [2.0, 4.0]]

    for interval in intervals
        println("For interval: ", interval)
        x1 = Float64(interval[1])
        x2 = Float64(interval[2])
        med = Float64(x2-x1)


        tab = [x1, x2, med]

        for x in tab
            next = nextfloat(x)
            println("The difference between ", x, " and ", next, " equals: ", Float64(next-x))
            println("Bit version of ", x)
            println(bitstring(x))
            println("Bit version of nextfloat", next)
            println(bitstring(next))
        end
    end
end

function main()
    types = [Float64]
    flag = true

    for T in types
        println("For type: ", T)
        x = analyse_gap(T, flag)

        if !flag
            println("Have all been equal?: ", x)
        end 
    end

end


second_part()