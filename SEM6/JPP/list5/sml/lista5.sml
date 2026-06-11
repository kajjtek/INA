fun binomial n k =
    if k = 0 orelse k = n then 1
    else binomial (n-1) k + binomial (n-1) (k-1)

fun binomial2 n k =
    let
        fun next_row [] = [1]
          | next_row xs =
            let
                fun pairs [] = []
                  | pairs [_] = []
                  | pairs (x::y::ys) = (x+y) :: pairs (y::ys)
            in
                1 :: pairs xs @ [1]
            end

        fun get_row 0 row = row
          | get_row m row = get_row (m-1) (next_row row)
    in
        List.nth (get_row n [1], k)
    end

fun mergesort [] = []
  | mergesort [x] = [x]
  | mergesort xs =
    let
        fun split [] = ([], [])
          | split [x] = ([x], [])
          | split (x::y::zs) =
            let 
                val (left, right) = split zs 
            in 
                (x::left, y::right) 
            end

        fun merge ([], ys) = ys
          | merge (xs, []) = xs
          | merge (x::xs, y::ys) =
            if x <= y then x :: merge (xs, y::ys)
            else y :: merge (x::xs, ys)

        val (left, right) = split xs
    in
        merge (mergesort left, mergesort right)
    end

fun de a 0 = (1, 0, a)
  | de a b =
    let
        val q = a div b
        val r = a mod b
        val (x', y', z) = de b r
    in
        (y', x' - q * y', z)
    end

fun prime_factors n =
    let
        fun factors d m =
            if d * d > m then if m > 1 then [m] else []
            else if m mod d = 0 then d :: factors d (m div d)
            else factors (if d = 2 then 3 else d + 2) m
    in
        if n < 2 then [] else factors 2 n
    end

fun totient n =
    let
        fun gcd a 0 = a
          | gcd a b = gcd b (a mod b)

        fun count k acc =
            if k > n then acc
            else if gcd n k = 1 then count (k + 1) (acc + 1)
            else count (k + 1) acc
    in
        if n <= 0 then 0 else count 1 0
    end

fun totient2 n =
    if n <= 0 then 0
    else
        let
            fun unique [] = []
              | unique [x] = [x]
              | unique (x::y::ys) = if x = y then unique (y::ys) else x :: unique (y::ys)

            val factors = unique (prime_factors n)
            fun calc [] acc = acc
              | calc (p::ps) acc = calc ps (acc div p * (p - 1))
        in
            calc factors n
        end

fun primes n =
    let
        fun sieve [] = []
          | sieve (p::xs) = p :: sieve (List.filter (fn x => x mod p <> 0) xs)
          
        fun range i j = if i > j then [] else i :: range (i+1) j
    in
        sieve (range 2 n)
    end


val t1_binomial  = binomial 5 2;

val t2_binomial2 = binomial2 5 2;

val t3_mergesort = mergesort [4, 1, 8, 3, 2, 9, 0, 5];
(* Oczekiwany wynik: [0, 1, 2, 3, 4, 5, 8, 9] *)

val t4_diofantos = de 102 38;
(* Oczekiwany wynik: (3, ~8, 2) -> Uwaga: w SML minus dla liczb to tylda `~` *)

val t5_factors   = prime_factors 60;
(* Oczekiwany wynik: [2, 2, 3, 5] *)

val t5_factors2 = prime_factors 315

val t6_totient   = totient 9;
(* Oczekiwany wynik: 6 *)

val t7_totient2  = totient2 9;
(* Oczekiwany wynik: 6 *)

val t7_totient3 = totient2 12345678

val t8_primes    = primes 20;
(* Oczekiwany wynik: [2, 3, 5, 7, 11, 13, 17, 19] *)