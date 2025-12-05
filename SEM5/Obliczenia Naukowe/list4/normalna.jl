function naturalna(x, fx)
    n = length(fx) - 1;
    
    a = fx; 
    
    for i = n : -1 : 1
        
        w = x[i]; 
        for j = n + 1 : -1 : 2 
            a[j-1] = a[j-1] - a[j] * w;
        end
        println(a[n])
    end
    
    return a;
end