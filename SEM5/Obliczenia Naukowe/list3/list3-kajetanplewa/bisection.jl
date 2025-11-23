using Test

#KAJETAN PLEWA

M = 100
#! ma zwracac r, v, it, err
#gdzie r - przyblizenie f(x) = 0, v wartosc f(r)
function bisection(f, a::Float64, b::Float64, delta::Float64, epsilon::Float64)
    error = 0;
    u = Float64(f(a))
    v = Float64(f(b))
    distance_between = b - a
    if (sign(u)==sign(v))
        error = 1
        return (NaN, NaN, 0, error)
    end 

    for count in 1:M
        distance_between = distance_between /2 
        mid = a + distance_between
        res_mid = f(mid)
        if(abs(distance_between)<delta || abs(res_mid)<epsilon)
            return (mid, res_mid, count, error)
        end 

        if sign(res_mid) == sign(f(a))
            a = mid
        else
            b = mid
        end 
    end
end 


@testset "Zad. 4.1: Metoda Bisekcji" begin
    delta = 0.5 * 1e-5
    epsilon = 0.5 * 1e-5

    (r, v, it, err) = bisection(x -> sin(x) - (0.5 * x)^2, 1.5, 2.0, delta, epsilon)

    @info "Bisection: r=$r, it=$it, v=$v, error=$err"
    
    @test err == 0 
end


@testset "Zad. 5.1 - root one" begin
        a1, b1 = 0.0, 1.0 
        delta = 1e-4
        epsilon = 1e-4
        
        (r, v, it, err) = bisection(x -> 3x - exp(x), a1, b1, delta, epsilon)
        
       @info "Bisection: r=$r, it=$it, v=$v, error=$err"

        @test err == 0
    end

@testset "Zad. 5.1 - root two" begin
        a1, b1 = 1.0 , 2.0
        delta = 1e-4
        epsilon = 1e-4
        
        (r, v, it, err) = bisection(x -> 3x - exp(x), a1, b1, delta, epsilon)
        
       @info "Bisection: r=$r, it=$it, v=$v, error=$err"

        @test err == 0
    end   

@testset "Zad. 6.1 - Bisection" begin
        a1, b1 = 0.5 , 2.0
        delta = 1e-5
        epsilon = 1e-5
        
        (r, v, it, err) = bisection(x -> exp(1-x)-1, a1, b1, delta, epsilon)
        
       @info "Bisection: r=$r, it=$it, v=$v, error=$err"

        @test err == 0
    end   

@testset "Zad. 6.2 - Bisection" begin
        a1, b1 = -1.0 , 0.5
        delta = 1e-5
        epsilon = 1e-5
        
        (r, v, it, err) = bisection(x -> x*exp(-x), a1, b1, delta, epsilon)
        
       @info "Bisection: r=$r, it=$it, v=$v, error=$err"

        @test err == 0
    end   