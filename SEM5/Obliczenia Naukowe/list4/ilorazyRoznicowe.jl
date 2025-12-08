#KAJETAN PLEWA
function ilorazyRekurencja(a::Int,b::Int,f::Vector{Float64},x::Vector{Float64})
    if a==b
        return f[a]
    end
    return (ilorazyRekurencja(a+1,b,f,x)-ilorazyRekurencja(a,b-1,f,x))/(x[b]-x[a])
end

function ilorazyRoznicowe(x::Vector{Float64}, f::Vector{Float64})
    n = length(x) - 1
    fx = zeros(Float64,n+1)
    
    for i in 1:n+1
        fx[i]=ilorazyRekurencja(1,i,f,x)
    end

    return fx
end

