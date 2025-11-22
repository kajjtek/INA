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