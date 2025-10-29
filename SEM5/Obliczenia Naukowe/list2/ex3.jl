using LinearAlgebra

function hilb(n::Int)
# Function generates the Hilbert matrix  A of size n,
#  A (i, j) = 1 / (i + j - 1)
# Inputs:
#	n: size of matrix A, n>=1
#
#
# Usage: hilb(10)
#
# Pawel Zielinski
        if n < 1
         error("size n should be >= 1")
        end
        return [1 / (i + j - 1) for i in 1:n, j in 1:n]
end

using LinearAlgebra

function matcond(n::Int, c::Float64)
# Function generates a random square matrix A of size n with
# a given condition number c.
# Inputs:
#	n: size of matrix A, n>1
#	c: condition of matrix A, c>= 1.0
#
# Usage: matcond(10, 100.0)
#
# Pawel Zielinski
        if n < 2
         error("size n should be > 1")
        end
        if c< 1.0
         error("condition number  c of a matrix  should be >= 1.0")
        end
        (U,S,V)=svd(rand(n,n))
        return U*diagm(0 =>[LinRange(1.0,c,n);])*V'
end

function main()
    hilbertMatrixPart()
    randomMatrixPart()
end 

function hilbertMatrixPart()
    println("----------------------------------------------------")
    println("Calculations performed on HILBERT matrix")
    println("----------------------------------------------------")
    for n in 1:20
        A = hilb(n)
        x_expected = ones(n)
        b = A*x_expected

        x_calculated_gaus = A \ b
        x_calculated_snd = inv(A)*b

        error_gaus = norm(x_expected-x_calculated_gaus) / norm(x_expected)
        error_snd = norm(x_expected-x_calculated_snd) / norm(x_expected)
        println("---------------------------")
        println("For n: ",n)
        println("The RANK of the matrix A: ", rank(A))
        println("COND for matrix A: ", cond(A))
        println("---------------------------")
        println("ERROR from GAUSS method: ", error_gaus)
        println("ERROR from INVERSE method: ", error_snd)
        println("---------------------------")
    end 

end

function randomMatrixPart()
    println("----------------------------------------------------")
    println("Calculations performed on RANDOM matrix")
    println("----------------------------------------------------")
    ns = [5, 10 ,20]
    cs = [1e0, 1e1, 1e3, 1e7, 1e12, 1e16]
    for n in ns
        for c in cs 
            A = matcond(n, c)
            x_expected = ones(n)
            b = A*x_expected

            x_calculated_gaus = A \ b
            x_calculated_snd = inv(A)*b

            error_gaus = norm(x_expected-x_calculated_gaus) / norm(x_expected)
            error_snd = norm(x_expected-x_calculated_snd) / norm(x_expected)
            println("---------------------------")
            println("For n: ",n, " and c: ", c)
            println("The RANK of the matrix A: ", rank(A))
            println("COND for matrix A: ", cond(A))
            println("---------------------------")
            println("ERROR from GAUSS method: ", error_gaus)
            println("ERROR from INVERSE method: ", error_snd)
            println("---------------------------")
        end    
    end
    println("----------------------------------------------------")
    println("----------------------------------------------------") 

end

main()