with Ada.Text_IO;         use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;

package body Ring is

   M : constant Long_Long_Integer := Long_Long_Integer (N);

   function Reduce (V : Long_Long_Integer) return Natural is
      R : Long_Long_Integer := V mod M;
   begin
      if R < 0 then
         R := R + M;
      end if;
      return Natural (R);
   end Reduce;

   --  Bezpieczne mnozenie: Long_Long_Integer pomiesci (N-1)^2 dla N < 2^31
   function Mul_Mod (A, B : Long_Long_Integer) return Natural is
   begin
      return Reduce ((A mod M) * (B mod M));
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

   function Mod_Inv (A : Long_Long_Integer) return Natural is
      G, X, Y : Long_Long_Integer;
   begin
      Ext_GCD (A mod M, M, G, X, Y);
      if G /= 1 then
         raise Not_Invertible with
           "Element " & Long_Long_Integer'Image (A mod M) &
           " nie jest odwracalny modulo" & Long_Long_Integer'Image (M);
      end if;
      return Reduce (X);
   end Mod_Inv;

   function To_Element (V : Integer) return Element is
   begin
      return (Val => Reduce (Long_Long_Integer (V)));
   end To_Element;

   function To_Integer (E : Element) return Integer is
   begin
      return Integer (E.Val);
   end To_Integer;

   function "+" (Left, Right : Element) return Element is
   begin
      return (Val => Reduce (Long_Long_Integer (Left.Val)
                           + Long_Long_Integer (Right.Val)));
   end "+";

   function "-" (Left, Right : Element) return Element is
   begin
      return (Val => Reduce (Long_Long_Integer (Left.Val)
                           - Long_Long_Integer (Right.Val)));
   end "-";

   --  Mnozenie przez Long_Long_Integer: (N-1)^2 ~ 1.5e18 < LLI'Last ~ 9.2e18
   function "*" (Left, Right : Element) return Element is
   begin
      return (Val => Mul_Mod (Long_Long_Integer (Left.Val),
                              Long_Long_Integer (Right.Val)));
   end "*";

   function "/" (Left, Right : Element) return Element is
   begin
      return (Val => Mul_Mod (Long_Long_Integer (Left.Val),
                              Long_Long_Integer (Mod_Inv
                                (Long_Long_Integer (Right.Val)))));
   end "/";

   function "-" (E : Element) return Element is
   begin
      return (Val => Reduce (-Long_Long_Integer (E.Val)));
   end "-";

   function "="  (Left, Right : Element) return Boolean is
     (Left.Val =  Right.Val);
   function "<"  (Left, Right : Element) return Boolean is
     (Left.Val <  Right.Val);
   function "<=" (Left, Right : Element) return Boolean is
     (Left.Val <= Right.Val);
   function ">"  (Left, Right : Element) return Boolean is
     (Left.Val >  Right.Val);
   function ">=" (Left, Right : Element) return Boolean is
     (Left.Val >= Right.Val);

   function Image (E : Element) return String is
   begin
      return Integer'Image (Integer (E.Val));
   end Image;

   procedure Put (E : Element) is
   begin
      Put (Integer (E.Val), Width => 0);
   end Put;

   procedure Get (E : out Element) is
      V : Integer;
   begin
      Get (V);
      E := (Val => Reduce (Long_Long_Integer (V)));
   end Get;

end Ring;