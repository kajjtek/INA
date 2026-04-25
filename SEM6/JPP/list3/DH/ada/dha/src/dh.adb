with Ada.Numerics.Discrete_Random;

package body DH is

   use Field;  

   function Power (Base : Elem; Exp : Long_Long_Integer) return Elem is
      Result : Elem              := One;
      B      : Elem              := Base;
      E      : Long_Long_Integer := Exp;
   begin
      while E > 0 loop
         if E mod 2 = 1 then
            Result := Result * B;
         end if;
         B := B * B;
         E := E / 2;
      end loop;
      return Result;
   end Power;

   function Is_Generator (G : Elem) return Boolean is
      Phi : Long_Long_Integer := Long_Long_Integer (P) - 1;
      Tmp : Long_Long_Integer := Phi;
      F   : Long_Long_Integer := 2;
   begin
      if G = Zero then
         return False;
      end if;
      while F * F <= Tmp loop
         if Tmp mod F = 0 then
            if Power (G, Phi / F) = One then
               return False;
            end if;
            while Tmp mod F = 0 loop
               Tmp := Tmp / F;
            end loop;
         end if;
         F := F + 1;
      end loop;
      if Tmp > 1 then
         if Power (G, Phi / Tmp) = One then
            return False;
         end if;
      end if;
      return True;
   end Is_Generator;

   function Create_Setup return Setup_Type is
      subtype Cand_Range is Integer range 2 .. P - 1;
      package Rnd is new Ada.Numerics.Discrete_Random (Cand_Range);
      Gen  : Rnd.Generator;
      Cand : Elem;
   begin
      Rnd.Reset (Gen);
      loop
         Cand := Field.Make (Long_Long_Integer (Rnd.Random (Gen)));
         exit when Is_Generator (Cand);
      end loop;
      return (Generator => Cand);
   end Create_Setup;

   function Get_Generator (S : Setup_Type) return Elem is
   begin
      return S.Generator;
   end Get_Generator;

   function Create_User (S : Setup_Type) return User_Type is
      subtype Sec_Range is Long_Long_Integer
        range 2 .. Long_Long_Integer (P) - 1;
      package Rnd is new Ada.Numerics.Discrete_Random (Sec_Range);
      Gen : Rnd.Generator;
   begin
      Rnd.Reset (Gen);
      return (Setup         => S,
              Secret        => Rnd.Random (Gen),
              Shared_Secret => Zero,
              Has_Key       => False);
   end Create_User;

   function Get_Public_Key (U : User_Type) return Elem is
   begin
      return Power (U.Setup.Generator, U.Secret);
   end Get_Public_Key;

   function Get_Shared_Key (U : User_Type) return Elem is
   begin
      return U.Shared_Secret;
   end Get_Shared_Key;

   procedure Set_Key (U : in out User_Type; A : Elem) is
   begin
      U.Shared_Secret := Power (A, U.Secret);
      U.Has_Key       := True;
   end Set_Key;

   function Encrypt (U : User_Type; M : Elem) return Elem is
   begin
      if not U.Has_Key then
         raise Key_Not_Set with "Ustaw klucz przed szyfrowaniem!";
      end if;
      return M * U.Shared_Secret;
   end Encrypt;

   function Decrypt (U : User_Type; C : Elem) return Elem is
   begin
      if not U.Has_Key then
         raise Key_Not_Set with "Ustaw klucz przed deszyfrowaniem!";
      end if;
      return C / U.Shared_Secret;
   end Decrypt;

end DH;