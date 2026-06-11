mergesort([], []) :- !.
mergesort([X], [X]) :- !.
mergesort(List, Sorted) :-
    split(List, L1, L2),
    mergesort(L1, S1),
    mergesort(L2, S2),
    merge(S1, S2, Sorted).

split([], [], []).
split([X], [X], []).
split([X, Y | Rest], [X | L1], [Y | L2]) :-
    split(Rest, L1, L2).

merge([], L, L) :- !.
merge(L, [], L) :- !.
merge([X | Tx], [Y | Ty], [X | M]) :-
    X =< Y, !,
    merge(Tx, [Y | Ty], M).
merge([X | Tx], [Y | Ty], [Y | M]) :-
    merge([X | Tx], Ty, M).


de(A, 0, 1, 0, A) :- !.
de(A, B, X, Y, Z) :-
    B > 0,
    Q is A // B,
    R is A mod B,
    de(B, R, X1, Y1, Z),
    X is Y1,
    Y is X1 - Q * Y1.


prime_factors(N, X) :-
    N > 1,
    prime_factors(N, 2, X).

prime_factors(1, _, []) :- !.
prime_factors(N, F, [F | Fact]) :-
    N mod F =:= 0, !,
    N1 is N // F,
    prime_factors(N1, F, Fact).
prime_factors(N, F, Fact) :-
    F * F < N, !,
    next_factor(F, F1),
    prime_factors(N, F1, Fact).
prime_factors(N, _, [N]).


totient(1, 1) :- !.
totient(N, T) :-
    N > 1,
    totient_helper(N, 2, N, T).

totient_helper(1, _, Acc, Acc) :- !.
totient_helper(N, F, Acc, T) :-
    N mod F =:= 0, !,
    Acc1 is Acc * (F - 1) // F,
    remove_factor(N, F, N1),
    next_factor(F, F1),
    totient_helper(N1, F1, Acc1, T).
totient_helper(N, F, Acc, T) :-
    F * F < N, !,
    next_factor(F, F1),
    totient_helper(N, F1, Acc, T).
totient_helper(N, _, Acc, T) :-
    N > 1, !,
    T is Acc * (N - 1) // N.
totient_helper(_, _, Acc, Acc).

% Usuwa wszystkie powtórzenia czynnika pierwszego z liczby
remove_factor(N, F, Result) :-
    N mod F =:= 0, !,
    N1 is N // F,
    remove_factor(N1, F, Result).
remove_factor(N, _, N).


primes(N, []) :- N < 2, !.
primes(N, X) :-
    gen_list(2, N, Range),
    sieve(Range, X).

% Generuje listę liczb od L do H
gen_list(L, H, []) :- L > H, !.
gen_list(L, H, [L | T]) :-
    L =< H,
    L1 is L + 1,
    gen_list(L1, H, T).

% Sianie Sita Eratostenesa
sieve([], []).
sieve([P | Rest], [P | Primes]) :-
    filter_multiples(P, Rest, Filtered),
    sieve(Filtered, Primes).

% Usuwa wielokrotności liczby P z listy
filter_multiples(_, [], []).
filter_multiples(P, [X | Src], Dst) :-
    X mod P =:= 0, !,
    filter_multiples(P, Src, Dst).
filter_multiples(P, [X | Src], [X | Dst]) :-
    filter_multiples(P, Src, Dst).


% Zwraca kolejny potencjalny dzielnik (omijamy liczby parzyste większe od 2)
next_factor(2, 3) :- !.
next_factor(F, F1) :- F1 is F + 2.