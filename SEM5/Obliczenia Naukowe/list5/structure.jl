mutable struct Block
    fields::Matrix{Float64}
end

mutable struct BlockMatrix
    n::Int
    l::Int
    v::Int
    AList::Vector{Block}
    BList::Vector{Block}
    CList::Vector{Block}
end

function createBlockMatrix(nn, ll)
    vv = Int(nn / ll)
    return BlockMatrix(nn, ll, vv, Block[], Block[], Block[])
end

function getPivot(block_matrix::BlockMatrix, pivotOn::Bool, block_n::Int, k::Int)
    if pivotOn #do poprawy
        return minInCol(block_matrix, block_n, k)
    else
        return block_matrix.AList[block_n].fields[k]
    end
end

function minInCol(block_matrix::BlockMatrix, block_n::Int, k::Int)
    #TODO
end 

function gaussianElimination(block_matrix::BlockMatrix, b_matrix::Vector{Float64}, pivotOn::Bool)
    for k in 1:block_matrix.v
        c = block_matrix.CList[k]
        a = block_matrix.AList[k]
        b = nothing
        nexta = nothing
        if k<block_matrix.v
            b = block_matrix.BList[k+1]
            nexta = block_matrix.AList[k+1]
        end
        eliminate!(c, a, b, block_matrix.l, b_matrix, nexta,pivotOn, k)
    end 
end


function eliminate!(C, A, B, l::Int, b_matrix::Vector{Float64}, nextA, pivotOn::Bool, n::Int)
    for k in 1:l-1
        #USTAWIANIE W BLOKU A - najpierw zmieniam sobie A i potem C
        for i in k+1:l
            I = A.fields[i,k] / A.fields[k,k] #TODO tu funkcja getPivot ale zmodyfikowana
            for j in k:l #to ustawia prawidlowe w bloku a
                A.fields[i,j] -= I * A.fields[k,j] 
            end #podczas gdy usuwamy w a musimy tez usuwac w c i usunac jeden z b co wplynie na kolejne A
            for j in 1:k
                C.fields[i,j] -= I * C.fields[k,j]
            end
            b_matrix[(n-1)*l+i] -= I * b_matrix[(n-1)*l+k]
        end
        if B!==nothing
            Ib = B.fields[1,k] / A.fields[k,k]
            for j in k:l
                B.fields[1,j] -= Ib * A.fields[k,j]
            end 
            for j in 1:k
                nextA.fields[1,j] -= Ib * C.fields[k,j]
            end
            b_matrix[(n-1)*l + l + 1] -= Ib * b_matrix[(n-1)*l + k]
        end
    end
    #teraz l ty na przekatnej glownej - musimy pousuwac b ktore pod tym stoja
    if(nextA!==nothing)
        for i in 1:l 
            I = B.fields[i,l] / A.fields[l,l]
            B.fields[i,l] -= I * A.fields[l,l]
            for j in 1:l
                nextA.fields[i,j] -= I * C.fields[l,j]
            end 
            b_matrix[n*l + i] -= I * b_matrix[n*l] 
        end
    end 
end

function back_substitution(M::BlockMatrix, b::Vector{Float64})
    n = M.n
    l = M.l
    x = zeros(Float64, n)
    
    for i in n:-1:1
        s = 0.0
        block_n = ceil(Int, i / l)
        diag_n = ((i - 1) % l) + 1
        
        # Globalny start aktualnego bloku (dla zmiennych x)
        offset_block = (block_n - 1) * l
        
        # 1. Elementy z bloku A (na prawo od przekątnej)
        A = M.AList[block_n]
        for j_local in (diag_n + 1):l
            global_j = offset_block + j_local
            s += A.fields[diag_n, j_local] * x[global_j]
        end
        
        # 2. Elementy z bloku C (następny blok zmiennych x)
        if block_n < M.v
            C = M.CList[block_n]
            offset_next_block = block_n * l
            # Wiersz diag_n w bloku C ma elementy tylko do kolumny diag_n
            for j_local in 1:diag_n
                global_j = offset_next_block + j_local
                if global_j <= n
                    s += C.fields[diag_n, j_local] * x[global_j]
                end
            end
        end

        x[i] = (b[i] - s) / A.fields[diag_n, diag_n]
    end
    return x
end