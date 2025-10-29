function logistic_model(p0::T, r::T, shouldStop::Bool) where T <: AbstractFloat
    p = p0

    for i in 1:40
        p = p + r*p*(1-p)

        if i==10 && shouldStop 
            p = floor(p * 1000.0f0) / 1000.0f0
        end 
    end 
    return p
end

function main()
    println("------------------------------")
    println("ITERATING WITH STOP - FLOAT32")
    println("------------------------------")
    T = Float32
    res_stop_32 = logistic_model(T(0.01), T(3.0), true)
    println("RESULT WITH STOP -> ", res_stop_32)
    println("------------------------------")
    println("ITERATING WITHOUT STOP - FLOAT32")
    println("------------------------------")
    T = Float32
    res_32 = logistic_model(T(0.01), T(3.0), false)
    println("RESULT WITH STOP -> ", res_32)
    println("------------------------------")
    println("ITERATING WITHOUT STOP - FLOAT64")
    println("------------------------------")
    T = Float64
    res_64 = logistic_model(T(0.01), T(3.0), false)
    println("RESULT WITH STOP -> ", res_64)
end 

main()