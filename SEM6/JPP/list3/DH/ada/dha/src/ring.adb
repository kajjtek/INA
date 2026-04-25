package body Ring is

   M : constant Long_Long_Integer := Long_Long_Integer (P);

   function Reduce (V : Long_Long_Integer) return Long_Long_Integer is
      R : Long_Long_Integer := V mod M;
   begin
      if R < 0 then
         R := R + M;
      end if;
      return R;
   end Reduce;

   -- Bezpieczne dodawanie modularne: nigdy nie przekroczy 2*M-2 < 2^32
   function Add_Mod (A, B : Long_Long_Integer) return Long_Long_Integer is
      S : Long_Long_Integer := A + B;
   begin
      if S >= M then
         S := S - M;
      end if;
      return S;
   end Add_Mod;

   -- Mnozenie przez podwajanie: uzywa tylko Add_Mod, zero overflow
   function Mul_Mod (A, B : Long_Long_Integer) return Long_Long_Integer is
      Result : Long_Long_Integer := 0;
      AA     : Long_Long_Integer := A mod M;
      BB     : Long_Long_Integer := B mod M;
   begin
      while BB > 0 loop
         if BB mod 2 = 1 then
            Result := Add_Mod (Result, AA);
         end if;
         AA := Add_Mod (AA, AA);  -- AA*2 mod M, max = 2*(M-1) < 2^32 < LLI'Last
         BB := BB / 2;
      end loop;
      return Result;
   end Mul_Mod;

   procedure Ext_GCD (A, B    :     Long_Long_Integer;
                      G, X, Y : out Long_Long_Integer) is
      X1, Y1 : Long_Long_Integer;
   begin
      if B = 0 then
         G := A; X := 1; Y := 0;
      else
         Ext_GCD (B, A mod B, G, X1, Y1);
         X := Y1;
         Y := X1 - (A / B) * Y1;
      end if;
   end Ext_GCD;

   function Inv_Mod (A : Long_Long_Integer) return Long_Long_Integer is
      G, X, Y : Long_Long_Integer;
   begin
      Ext_GCD (A mod M, M, G, X, Y);
      if G /= 1 then
         raise Not_Invertible with "brak odwrotnosci dla" & A'Image;
      end if;
      return Reduce (X);
   end Inv_Mod;

   function Make (V : Long_Long_Integer) return Element is
   begin
      return (V => Reduce (V));
   end Make;

   function Val (E : Element) return Long_Long_Integer is
   begin
      return E.V;
   end Val;

   function "+" (L, R : Element) return Element is
   begin
      return (V => Add_Mod (L.V, R.V));
   end "+";

   function "-" (L, R : Element) return Element is
   begin
      return (V => Reduce (L.V - R.V));
   end "-";

   function "*" (L, R : Element) return Element is
   begin
      return (V => Mul_Mod (L.V, R.V));
   end "*";

   function "/" (L, R : Element) return Element is
   begin
      return (V => Mul_Mod (L.V, Inv_Mod (R.V)));
   end "/";

   function "-" (E : Element) return Element is
   begin
      return (V => Reduce (-E.V));
   end "-";

   function "=" (L, R : Element) return Boolean is
   begin
      return L.V = R.V;
   end "=";

end Ring;