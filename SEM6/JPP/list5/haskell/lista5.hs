{-
 file : LAB6
 date : 09.06.2026
 author : Kajetan Plewa
-}
{-# OPTIONS_GHC -Wno-unrecognised-pragmas #-}
{-# HLINT ignore "Use map" #-}
{-# OPTIONS_GHC -Wno-overlapping-patterns #-}
{-# HLINT ignore "Use camelCase" #-}

module LAB6 where
import Distribution.Simple.Utils (xargs)
import Distribution.Verbosity (normal)
import Control.Arrow (ArrowChoice(right))
import Data.List

-- to jest binomail zwykly --
binomial :: Int -> Int -> Int
binomial n 0 = 1
binomial n k = binomial (n-1) (k-1) + binomial (n-1) k

-- tu jest do trojkata paskala --
binomial2 :: Int -> Int -> Int
binomial2 n k = pascal n !! k

pascal :: Int -> [Int]
pascal 0 = [1]
pascal n = zipWith (+) (0:high) (high ++ [0])
    where
        high = pascal (n-1)

--tu sa do mergesortu--
splitAlternating :: [a] -> ([a], [a])
splitAlternating [] = ([], [])
splitAlternating [x] = ([x], [])
splitAlternating (x:y:rest) = (x:xs, y:ys)
  where
    (xs, ys) = splitAlternating rest

mergeSort :: Ord a => [a] -> [a]
mergeSort []  = []
mergeSort [x] = [x]
mergeSort xs  = mergeCustom (mergeSort lewa) (mergeSort prawa)
  where
    (lewa, prawa) = splitAlternating xs

mergeCustom::Ord a => [a] -> [a] -> [a]
mergeCustom [] xs = xs
mergeCustom xs [] = xs
mergeCustom (x:xs) (y:ys)
    | x <= y = x:mergeCustom xs (y:ys)
    | otherwise = y:mergeCustom (x:xs) ys

de :: Integral a => a -> a -> (a, a, a)
de a 0 = (1, 0, a)
de a b = (y', x' - q * y', z)
  where
    q = a `div` b
    r = a `mod` b
    (x', y', z) = de b r

primeFactors :: Integer -> [Integer]
primeFactors n = factor n 2
  where
    factor 1 _ = []
    factor m d
      | m `mod` d == 0 = d : factor (m `div` d) d
      | otherwise      = factor m (d + 1)

totient :: Integer -> Integer
totient n = sum [1 | k <- [1..n], gcd n k == 1]

totient2 :: Integer -> Integer
totient2 n = foldl (\acc p -> (acc `div` p) * (p - 1)) n uniquePrimes
  where
    uniquePrimes = nub (primeFactors n)

primes :: Integer -> [Integer]
primes n = sieve [2..n]
  where
    sieve [] = []
    sieve (p:xs) = p : sieve [x | x <- xs, x `mod` p /= 0]







assertTest :: String -> Bool -> IO ()
assertTest name True  = putStrLn $ "OKEJ: " ++ name
assertTest name False = error    $ "BLad: " ++ name

main :: IO ()
main = do
    putStrLn "\n--- URUCHAMIAM TESTY ---"
    
    putStrLn "\nZadanie 2: Trójkąt Pascala"
    assertTest "binomial2 5 2 = 10"   (binomial2 5 2 == 10)
    assertTest "binomial2 50 2 = 1225" (binomial2 50 2 == 1225)
    
    putStrLn "\nZadanie 3: MergeSort"
    assertTest "Sortowanie listy" (mergeSort [4, 1, 8, 3, 2, 9, 0, 5] == [0, 1, 2, 3, 4, 5, 8, 9])
    -- Dodajemy typ [Int], aby Haskell wiedział, jakiego typu jest pusta lista
    assertTest "Pusta lista"      (mergeSort ([] :: [Int]) == []) 
    assertTest "Jeden element"    (mergeSort [1] == [1])
    
    putStrLn "\nZadanie 4: Równanie diofantyczne"
    assertTest "de 102 38 = (3, -8, 2)" (de 102 38 == (3, -8, 2))
    
    putStrLn "\nZadanie 5: Czynniki pierwsze"
    assertTest "primeFactors 60"  (primeFactors 60 == [2, 2, 3, 5])
    assertTest "primeFactors 315" (primeFactors 315 == [3, 3, 5, 7])
    
    putStrLn "\nZadanie 6 i 7: Funkcja Eulera (Totient)"
    assertTest "totient 9 = 6"          (totient 9 == 6)
    assertTest "totient 1 = 1"          (totient 1 == 1)
    assertTest "totient2 9 = 6"         (totient2 9 == 6)
    assertTest "totient2 1 = 1"         (totient2 1 == 1)
    assertTest "totient2 12345678 (Szybki test!)" (totient2 12345678 == 4027392)
    
    putStrLn "\nZadanie 8: Sito Eratostenesa"
    assertTest "primes 20" (primes 20 == [2, 3, 5, 7, 11, 13, 17, 19])
    
    putStrLn "\n wsyzastko git\n"   