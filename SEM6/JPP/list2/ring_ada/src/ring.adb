

with Ada.Text_IO;         use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;

package body Ring is

   

   
   function Reduce (V : Integer) return Natural is
      R : Integer := V mod N;
   begin
      if R < 0 then R := R + N; end if;
      return Natural (R);
   end Reduce;

   
   procedure Ext_GCD (A, B : in Integer;
                      G, X, Y : out Integer) is
      X1, Y1 : Integer;
   begin
      if B = 0 then
         G := A; X := 1; Y := 0;
      else
         Ext_GCD (B, A mod B, G, X1, Y1);
         G := G;          
         X := Y1;
         Y := X1 - (A / B) * Y1;
      end if;
   end Ext_GCD;

   
   function Mod_Inv (A : Integer) return Integer is
      G, X, Y : Integer;
      AA : constant Integer := Reduce (A);
   begin
      Ext_GCD (AA, N, G, X, Y);
      if G /= 1 then
         raise Not_Invertible with
           "Element " & Integer'Image (AA) &
           " nie jest odwracalny modulo" & Integer'Image (N);
      end if;
      return Reduce (X);
   end Mod_Inv;

   

   function To_Element (V : Integer) return Element is
   begin
      return (Val => Reduce (V));
   end To_Element;

   function To_Integer (E : Element) return Integer is
   begin
      return Integer (E.Val);
   end To_Integer;

   

   function "+" (Left, Right : Element) return Element is
   begin
      return (Val => Reduce (Integer (Left.Val) + Integer (Right.Val)));
   end "+";

   function "-" (Left, Right : Element) return Element is
   begin
      return (Val => Reduce (Integer (Left.Val) - Integer (Right.Val)));
   end "-";

   function "*" (Left, Right : Element) return Element is
   begin
      return (Val => Reduce (Integer (Left.Val) * Integer (Right.Val)));
   end "*";

   function "/" (Left, Right : Element) return Element is
   begin
      return (Val => Reduce (Integer (Left.Val) * Mod_Inv (Integer (Right.Val))));
   end "/";

   function "-" (E : Element) return Element is
   begin
      return (Val => Reduce (-Integer (E.Val)));
   end "-";

   

   function "="  (Left, Right : Element) return Boolean is (Left.Val =  Right.Val);
   
   function "<"  (Left, Right : Element) return Boolean is (Left.Val <  Right.Val);
   function "<=" (Left, Right : Element) return Boolean is (Left.Val <= Right.Val);
   function ">"  (Left, Right : Element) return Boolean is (Left.Val >  Right.Val);
   function ">=" (Left, Right : Element) return Boolean is (Left.Val >= Right.Val);

   

   function Image (E : in Element) return String is
   begin
      return Integer'Image (Integer (E.Val));
   end Image;

   procedure Put (E : in Element) is
   begin
      Put (Integer (E.Val), Width => 0);
   end Put;

   procedure Get (E : out Element) is
      V : Integer;
   begin
      Get (V);
      E := (Val => Reduce (V));
   end Get;

end Ring;