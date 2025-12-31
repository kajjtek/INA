mutable struct classA
    a_fields::Matrix{Float64}
end

mutable struct classB
    b_fields::Matrix{Float64}
end

mutable struct classC
    diagonal_fields::Vector{Float64}
end

mutable struct BlockMatrix
    n::Int
    l::Int
    v::Int
    AList::Vector{classA}
    BList::Vector{classB}
    CList::Vector{classC}
end

function createBlockMatrix(nn, ll, vv)
    return BlockMatrix(nn, ll, vv, [], [], []) #TODO
end 

function getPivot(block_matrix::BlockMatrix, pivotOn::Bool, block_n::Int, k::Int)
    if pivotOn #do poprawy
        return minInCol(block_matrix, block_n, k)
    else
        return block_matrix.AList[block_n].a_fields[k]
    end
end

function minInCol(block_matrix::BlockMatrix, block_n::Int, k::Int)
    #TODO
end 

function gaussianElimination(block_matrix::BlockMatrix, b_matrix::Vector{Float64})
    for k in 1:block_matrix.v
        c = block_matrix.CList[k-1]
        a = block_matrix.AList[k]
        b = block_matrix.BList[k+1]
        if k==1
            c = undef
        elseif k==block_matrix.v
            b = undef
        end
        eliminate(c, a, b, l, b_matrix)
    end 
end

function eliminate(C::classC, A::classA, B::classB, l::Int, b_matrix::Vector{Float64})
    for k in 1:l-1
        for i in k+1:l
            I = A.a_fields[i][k] / A.a_fields[k][k] #TODO tu funkcja getPivot ale zmodyfikowana
            for j in k+1:l
                A.a_fields[i][j] -= I * A.a_fields[k][j] 
            end
            B.b_fields[]
        end 
    end
end