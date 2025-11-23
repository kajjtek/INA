using Test
#KAJETAN PLEWA

function newton(f, pf, x0::Float64, delta::Float64, epsilon::Float64, maxit::Int)
    v = f(x0)
    if abs(v) < epsilon
        return (0, x0, v, 1)
    end
    for k in 1:maxit 
        pv = pf(x0)
        if abs(pv) < 1e-15
            return (x0, v, k, 2)
        end
        x1 = x0 - (v/pv)
        v = f(x1)
        if(abs(x1-x0) < delta || abs(v)<epsilon )
            return (x1, v, k, 0)
        end
        x0 = x1
    end
    return (x0, v, maxit, 1)
    error("out of iteration loop") 
end 

@testset "Zad. 4.2: Metoda Newtona" begin
    delta = 0.5 * 1e-5
    epsilon = 0.5 * 1e-5
    maxit = 100
    (r, v, it, err) = newton(x -> sin(x) - (0.5 * x)^2, x -> cos(x) - x/2.0, 1.5, delta, epsilon, maxit)

    @info "Newton: r=$r, it=$it, v=$v, error=$err"

    @test err == 0

    
    @test it < 10 
end

@testset "Zad. 6.1: Metoda Newtona" begin
    delta = 1e-5
    epsilon = 1e-5
    maxit = 100
    (r, v, it, err) = newton(x -> exp(1-x)-1, x -> -exp(1-x), 0.0, delta, epsilon, maxit)

    @info "Newton: r=$r, it=$it, v=$v, error=$err"

    @test err == 0
end

@testset "Zad. 6.2: Metoda Newtona" begin
    delta = 1e-5
    epsilon = 1e-5
    maxit = 100
    (r, v, it, err) = newton(x -> x*exp(-x), x -> -(x-1)*exp(-x), 0.5, delta, epsilon, maxit)

    @info "Newton: r=$r, it=$it, v=$v, error=$err"

    @test err == 0
    
end

@testset "Przypadki graniczne - f1" begin
    delta = 1e-5
    epsilon = 1e-5
    maxit = 100
    (r, v, it, err) = newton(x -> x*exp(-x), x -> -(x-1)*exp(-x), 2.0, delta, epsilon, maxit)

    @info "Newton: r=$r, it=$it, v=$v, error=$err"

    
end


@testset "Przypadki graniczne - f2" begin
    delta = 1e-5
    epsilon = 1e-5
    maxit = 100
    (r, v, it, err) = newton(x -> x*exp(-x), x -> -(x-1)*exp(-x), 2.0, delta, epsilon, maxit)

    @info "Newton: r=$r, it=$it, v=$v, error=$err"

    
end

@testset "Przypadki graniczne - f2 x0 = 1" begin
    delta = 1e-5
    epsilon = 1e-5
    maxit = 100
    (r, v, it, err) = newton(x -> x*exp(-x), x -> -(x-1)*exp(-x), 1.0, delta, epsilon, maxit)

    @info "Newton: r=$r, it=$it, v=$v, error=$err"

    
end