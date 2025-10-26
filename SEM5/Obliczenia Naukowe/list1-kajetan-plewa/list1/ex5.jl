# Author: KAJETAN PLEWA

"""
    first(T::Type, x::AbstractVector, y::AbstractVector)

Calculates the dot product (scalar product) of two vectors, `x` and `y`, 
by summing the products of their corresponding elements **in forward order** (from index 1 to 5). This is the standard, naive approach.

# Formal Parameters
- `T::Type`: The floating-point type (e.g., Float32, Float64) to be used for the calculation.
- `x::AbstractVector`: The first input vector (expected to have 5 elements).
- `y::AbstractVector`: The second input vector (expected to have 5 elements).

# Returns
- `T`: The computed dot product, $\\sum_{i=1}^{5} x_i y_i$.
"""
function first(T::Type, x, y)
    # x, y: Vectors cast to type T, ensuring all subsequent calculations use the specified precision.
    x = T.(x)
    y = T.(y)
    # sum: The accumulator for the dot product, initialized to 0.0 of type T.
    sum = T(0.0)
    # Loop iterates from the first index (1) to the last (5).
    for i in 1:5
        # The product T(x[i]) * T(y[i]) is added to the sum.
        sum += T(x[i]) * T(y[i])
    end
    return sum
end

---

"""
    sum_forward(terms::AbstractVector, T::Type)

Calculates the sum of a vector of `terms` **in forward order**. This is a helper 
function used by `third` and `fourth` to perform summation in a controlled way.

# Formal Parameters
- `terms::AbstractVector`: The vector of numbers to be summed.
- `T::Type`: The floating-point type used for the accumulator.

# Returns
- `T`: The computed sum, $\\sum_{i=1}^{n} \\text{terms}_i$.
"""
function sum_forward(terms, T::Type)
    # S: The accumulator for the sum, initialized to 0.0 of type T.
    S = T(0.0)
    # Loop iterates through the indices of the 'terms' vector in ascending order.
    for i in 1:length(terms)
        S = S + terms[i]
    end
    return S
end

---

"""
    second(T::Type, x::AbstractVector, y::AbstractVector)

Calculates the dot product of two vectors, `x` and `y`, by summing the products 
of their corresponding elements **in reverse order** (from index 5 down to 1). 
This function tests the effect of the summation order on the result due to 
floating-point rounding.

# Formal Parameters
- `T::Type`: The floating-point type (e.g., Float32, Float64) to be used for the calculation.
- `x::AbstractVector`: The first input vector (expected to have 5 elements).
- `y::AbstractVector`: The second input vector (expected to have 5 elements).

# Returns
- `T`: The computed dot product, $\\sum_{i=5}^{1} x_i y_i$.
"""
function second(T::Type, x, y)
    # x, y: Vectors cast to type T.
    x = T.(x)
    y = T.(y)
    # sum: The accumulator for the dot product, initialized to 0.0 of type T.
    sum = T(0.0)
    # Loop iterates from the last index (5) down to the first (1).
    for i in 5:-1:1
        # The product T(x[i]) * T(y[i]) is added to the sum.
        sum += T(x[i]) * T(y[i])
    end
    return sum
end

---

"""
    third(T::Type, x::AbstractVector, y::AbstractVector)

Calculates the dot product using a refined method to minimize catastrophic 
cancellation. Products are calculated, then separated into **positive** and 
**negative** groups. Both groups are **sorted by magnitude (absolute value) in descending order** (largest terms first), summed separately, and finally combined. Sorting large 
terms first helps maintain precision.

# Formal Parameters
- `T::Type`: The floating-point type (e.g., Float32, Float64) to be used for the calculation.
- `x::AbstractVector`: The first input vector.
- `y::AbstractVector`: The second input vector.

# Returns
- `T`: The computed dot product.
"""
function third(T::Type, x, y)
    # x, y: Vectors cast to type T.
    x = T.(x)
    y = T.(y)

    # products: A vector containing the element-wise product x[i] * y[i].
    products = T.(x) .* T.(y)

    # positive: A sub-array containing all elements from 'products' that are greater than 0.
    positive = products[products .> T(0.0)]
    # negative: A sub-array containing all elements from 'products' that are less than 0.
    negative = products[products .< T(0.0)]

    # Sorting positive terms: Largest magnitude first (descending).
    sort!(positive, by = abs, rev = true)
    # Sorting negative terms: Largest magnitude first (descending).
    sort!(negative, by = abs, rev = true)

    # spos: The sum of the positive terms, calculated using forward summation.
    spos = sum_forward(positive, T)
    # sneg: The sum of the negative terms, calculated using forward summation.
    sneg = sum_forward(negative, T)

    println("Sum of positive: ", spos, " Sum of negative: ", sneg)

    # Final result is the sum of the positive and negative sums.
    return spos + sneg
end

---

"""
    fourth(T::Type, x::AbstractVector, y::AbstractVector)

Calculates the dot product using a method intended to **maximize cancellation error**. 
Products are calculated, then separated into **positive** and **negative** groups. 
Both groups are **sorted by magnitude (absolute value) in ascending order** (smallest terms first), summed separately, and finally combined. Summing small 
terms first can lead to greater loss of significance (precision).

# Formal Parameters
- `T::Type`: The floating-point type (e.g., Float32, Float64) to be used for the calculation.
- `x::AbstractVector`: The first input vector.
- `y::AbstractVector`: The second input vector.

# Returns
- `T`: The computed dot product.
"""
function fourth(T::Type, x, y)
    # x, y: Vectors cast to type T.
    x = T.(x)
    y = T.(y)

    # products: A vector containing the element-wise product x[i] * y[i].
    products = x .* y

    # positive: A sub-array containing all elements from 'products' that are greater than 0.
    positive = products[products .> T(0.0)]
    # negative: A sub-array containing all elements from 'products' that are less than 0.
    negative = products[products .< T(0.0)]

    # Sorting positive terms: Smallest magnitude first (ascending).
    sort!(positive, by = abs, rev = false)
    # Sorting negative terms: Smallest magnitude first (ascending).
    sort!(negative, by = abs, rev = false)

    # spos: The sum of the positive terms, calculated using forward summation.
    spos = sum_forward(positive, T)
    # sneg: The sum of the negative terms, calculated using forward summation.
    sneg = sum_forward(negative, T)

    # Final result is the sum of the positive and negative sums.
    return spos + sneg
end

---

"""
    main()

The main function that sets up the input vectors `x` and `y` and executes the 
four different summation methods (`first`, `second`, `third`, `fourth`) for 
both Float32 and Float64 types, displaying the results to compare accuracy.

# Formal Parameters
- This function takes no parameters.
"""
function main()
    # x: The first input vector of floating-point numbers (Double precision by default in Julia).
    x = [2.718281828, -3.141592654, 1.414213562, 0.5772156649, 0.3010299957]
    # y: The second input vector of floating-point numbers.
    y = [1486.2497, 878366.9879, -22.37492, 4773714.647, 0.000185049]

    # functions: An array containing references to the four summation functions to be tested.
    functions = [first, second, third, fourth]

    # types: An array containing the floating-point precisions to be tested.
    types = [Float32, Float64]

    # Outer loop iterates through each type (Float32, Float64).
    for T in types
        println("Type: ", T)
        # Inner loop iterates through each summation function.
        for f in functions
            # result: The calculated dot product using the current function 'f' and type 'T'.
            result = f(T, x, y)
            println("Result for function ", f, " is ", result)
        end
    end

end

main()