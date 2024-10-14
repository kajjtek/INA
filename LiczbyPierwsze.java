

public class LiczbyPierwsze {
    public int[] primes;

    LiczbyPierwsze(int n){
        int k = 0;
        for(int i = 2; i<=n;i++){
            if(isPrime(i)){
                k++;
            }
        }
        primes = new int [k];
        k = 0;
        for(int i=2; i<=n; i++){
            if(isPrime(i)){
                primes[k] = i;
                k++;
            }
        }

    }
public static boolean isPrime(int num){
    if(num<=1){
        return false;
    }
    for(int i = 2; i<=num/2;i++){
        if(num%i==0){
            return false;
        }
    }
    return true;
}
public int liczba(int m){
    return(primes[m]);
}

}

