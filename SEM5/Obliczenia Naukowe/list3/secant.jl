using Test

#KAJETAN PLEWA

function secant(f, x0::Float64, x1::Float64, delta::Float64, epsilon::Float64, maxit::Int)
    v0 = f(x0)
    v1 = f(x1) 
    for k in 1:maxit
        if(abs(v0) > abs(v1))
            (x0, x1) = (x1, x0)
            (v0, v1) = (v1, v0)
        end
        if(abs(v1-v0)<1e-15)
             return (x1, v1, k, 0)
        end
        s = (x1-x0)/(v1-v0)
        x1 = x0
        v1 = v0
        x0 = x0 - v0 * s
        v0 = f(x0)
        if(abs(x1-x0) < delta || abs(v0)<epsilon )
            return (x0, v0, k, 0)
        end
    end
    return (NaN, NaN, maxit , 1)
end

@testset "Zad. 4.3: Metoda Siecznych" begin

    delta = 0.5 * 1e-5
    epsilon = 0.5 * 1e-5
    maxit = 100

    (r, v, it, err) = secant(x -> sin(x) - (0.5 * x)^2, 1.5, 2.5, delta, epsilon, maxit)

    @info "Siecznych: r=$r, it=$it, v=$v, error=$err"
    
    @test err == 0
    
end

@testset "Zad. 6.1: Metoda Siecznych - 1" begin

    delta = 0.5 * 1e-5
    epsilon = 0.5 * 1e-5
    maxit = 1000000000

    (r, v, it, err) = secant(x -> exp(1-x)-1, 0.5, 0.75, delta, epsilon, maxit)

    @info "Siecznych: r=$r, it=$it, v=$v, error=$err"
    
    @test err == 0
end

@testset "Zad. 6.1: Metoda Siecznych" begin

    delta = 0.5 * 1e-5
    epsilon = 0.5 * 1e-5
    maxit = 100000

    (r, v, it, err) = secant(x -> x*exp(-x), -1.0, -0.5, delta, epsilon, maxit)

    @info "Siecznych: r=$r, it=$it, v=$v, error=$err"
    
    @test err == 0

end
