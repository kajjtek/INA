include("structure.jl")

function read_matrix(filepath::String)::BlockMatrix
    open(filepath, "r") do file
        # 1. Czytamy pierwszą linię (n i l)
        first_line = split(readline(file))
        n = parse(Int, first_line[1])
        l = parse(Int, first_line[2])
        v = Int(n / l)

        # 2. Inicjalizujemy BlockMatrix
        # Tworzymy wektory macierzy l x l wypełnionych zerami
        AList = [Block(zeros(Float64, l, l)) for _ in 1:v]
        BList = [Block(zeros(Float64, l, l)) for _ in 1:v]
        CList = [Block(zeros(Float64, l, l)) for _ in 1:v]
        
        bm = BlockMatrix(n, l, v, AList, BList, CList)

        # 3. Czytamy resztę pliku linia po linii
        for line in eachline(file)
            parts = split(line)
            if length(parts) < 3 continue end
            
            i = parse(Int, parts[1])
            j = parse(Int, parts[2])
            val = parse(Float64, parts[3])

            # Wyznaczamy numer bloku i lokalne indeksy
            block_i = ceil(Int, i / l)
            block_j = ceil(Int, j / l)
            
            local_i = ((i - 1) % l) + 1
            local_j = ((j - 1) % l) + 1

            # Przypisujemy do odpowiedniego bloku
            if block_i == block_j
                # Diagonala -> A
                bm.AList[block_i].fields[local_i, local_j] = val
            elseif block_i == block_j + 1
                # Pod diagonalą -> B
                bm.BList[block_i].fields[local_i, local_j] = val
            elseif block_i == block_j - 1
                # Nad diagonalą -> C
                bm.CList[block_i].fields[local_i, local_j] = val
            end
        end
        return bm
    end
end

function read_vector(filepath::String)::Vector{Float64}
    open(filepath, "r") do file
        # Pierwsza linia to n
        n = parse(Int, readline(file))
        b = Vector{Float64}(undef, n)
        
        idx = 1
        for line in eachline(file)
            val = parse(Float64, strip(line))
            b[idx] = val
            idx += 1
        end
        return b
    end
end

function compute_b_ones(M::BlockMatrix)
    n = M.n
    l = M.l
    b = zeros(Float64, n)
    
    for k in 1:M.v
        offset = (k - 1) * l
        
        # 1. Sumujemy elementy z bloku A (diagonalny)
        for i in 1:l
            for j in 1:l
                b[offset + i] += M.AList[k].fields[i, j]
            end
        end
        
        # 2. Sumujemy elementy z bloku C (nad diagonalą)
        if k < M.v
            for i in 1:l
                for j in 1:l
                    b[offset + i] += M.CList[k].fields[i, j]
                end
            end
        end
        
        # 3. Sumujemy elementy z bloku B (pod diagonalą)
        # Uwaga: sprawdzamy czy lista BList ma elementy dla tego bloku
        if k > 1 && k <= length(M.BList)
            for i in 1:l
                for j in 1:l
                    b[offset + i] += M.BList[k].fields[i, j]
                end
            end
        end
    end
    return b
end