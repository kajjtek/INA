let rec binomial n k =
  if k = 0 || k = n then 1
  else if k < 0 || k > n then 0
  else binomial (n - 1) k + binomial (n - 1) (k - 1)

let next_row row =
  let rec zip = function
    | [] | [_] -> []
    | x :: y :: tl -> (x + y) :: zip (y :: tl)
  in
  1 :: zip row @ [1]

let rec pascal_row current n =
  if n = 0 then current
  else pascal_row (next_row current) (n - 1)

let binomial2 n k =
  if k < 0 || k > n then 0
  else List.nth (pascal_row [1] n) k

let rec split = function
  | [] -> ([], [])
  | [x] -> ([x], [])
  | x :: y :: tl ->
      let l1, l2 = split tl in
      (x :: l1, y :: l2)

let rec merge l1 l2 =
  match l1, l2 with
  | [], l | l, [] -> l
  | x :: tx, y :: ty ->
      if x <= y then x :: merge tx l2
      else y :: merge l1 ty

let rec mergesort = function
  | [] -> []
  | [x] -> [x]
  | l ->
      let l1, l2 = split l in
      merge (mergesort l1) (mergesort l2)

let rec de a b =
  if b = 0 then (1, 0, a)
  else
    let q = a / b in
    let r = a mod b in
    let (x', y', z) = de b r in
    (y', x' - q * y', z)

let prime_factors n =
  let rec factors_from d acc n =
    if n < 2 then acc
    else if d * d > n then n :: acc
    else if n mod d = 0 then factors_from d (d :: acc) (n / d)
    else factors_from (d + 1) acc n
  in
  List.rev (factors_from 2 [] n)

let rec gcd a b = if b = 0 then a else gcd b (a mod b)

let totient n =
  let rec count k acc =
    if k > n then acc
    else if gcd k n = 1 then count (k + 1) (acc + 1)
    else count (k + 1) acc
  in
  if n < 1 then 0 else count 1 0

let rec uniq = function
  | [] -> []
  | [x] -> [x]
  | x :: y :: tl -> if x = y then uniq (y :: tl) else x :: uniq (y :: tl)

let totient2 n =
  if n < 1 then 0
  else
    let factors = uniq (prime_factors n) in
    List.fold_left (fun acc p -> acc * (p - 1) / p) n factors

let primes n =
  let rec range i j = if i > j then [] else i :: range (i + 1) j in
  let rec sieve = function
    | [] -> []
    | p :: xs -> p :: sieve (List.filter (fun x -> x mod p <> 0) xs)
  in
  sieve (range 2 n)



let () =
  assert (binomial 5 2 = 10);
  assert (binomial2 5 2 = 10);
  assert (binomial 50 2 = 1225); (* Szybki test dla Pascala *)
  assert (binomial2 50 2 = 1225) (* Szybki test dla Pascala *)

(* Testy zadania 3 (Mergesort) *)
let () =
  assert (mergesort [4; 1; 8; 3; 2; 9; 0; 5] = [0; 1; 2; 3; 4; 5; 8; 9]);
  assert (mergesort [] = []);
  assert (mergesort [1] = [1])

(* Testy zadania 4 (Równanie diofantyczne) *)
let () =
  assert (de 102 38 = (3, -8, 2))

(* Testy zadania 5 (Czynniki pierwsze) *)
let () =
  assert (prime_factors 60 = [2; 2; 3; 5]);
  assert (prime_factors 315 = [3; 3; 5; 7])

(* Testy zadania 6 i 7 (Funkcja Eulera) *)
let () =
  assert (totient 9 = 6);   (* Liczby względnie pierwsze z 10 to: 1, 3, 7, 9 *)
  assert (totient 1 = 1);
  assert (totient2 9 = 6);
  assert (totient2 1 = 1);
  assert (totient2 12345678 = 4027392) (* Szybki test oparty na rozkładzie *)

(* Testy zadania 8 (Sito Eratostenesa) *)
let () =
  assert (primes 20 = [2; 3; 5; 7; 11; 13; 17; 19])