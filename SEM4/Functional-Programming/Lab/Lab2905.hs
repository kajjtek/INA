
module LAB2905 where

charAppend::Char -> [Char]->[Char]
charAppend x ls = x:ls

greyMapper::Char->[String]->[String]
greyMapper n = map (n:)

greyCode::Int->[String]
greyCode 1 = ["0","1"]
greyCode n = map ('0':) prevCode ++ map ('1':) revPrevCode
    where
        prevCode = greyCode (n-1)
        revPrevCode = reverse prevCode
