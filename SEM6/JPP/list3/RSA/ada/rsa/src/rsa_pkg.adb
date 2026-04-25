with Ada.Numerics.Discrete_Random;

package body RSA_Pkg is
   use R;
   
   function GCD (A, B : Long_Long_Integer) return Long_Long_Integer is
      X : Long_Long_Integer := A;
      Y : Long_Long_Integer := B;
      T : Long_Long_Integer;
   begin
      while Y /= 0 loop
         T := X mod Y;
         X := Y;
         Y := T;
      end loop;
      return X;
   end GCD;
   
   function Extended_GCD_Inv (A, M : Long_Long_Integer) return Long_Long_Integer is
      T : Long_Long_Integer := 0;  NewT : Long_Long_Integer := 1;
      R : Long_Long_Integer := M;  NewR : Long_Long_Integer := A;
      Q, Temp : Long_Long_Integer;
   begin
      while NewR /= 0 loop
         Q := R / NewR;
         Temp := T; T := NewT; NewT := Temp - Q * NewT;
         Temp := R; R := NewR; NewR := Temp - Q * NewR;
      end loop;
      if T < 0 then T := T + M; end if;
      return T;
   end Extended_GCD_Inv;
   
   function Power (Base : R.Element; Exp : Long_Long_Integer) return R.Element is
      Result : R.Element := R.To_Element (1); 
      B      : R.Element := Base;
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

   function Create (P, Q : Positive) return RSA_Instance is
      Phi : constant Long_Long_Integer := Long_Long_Integer (P - 1) * Long_Long_Integer (Q - 1);
      
      subtype Phi_Range is Long_Long_Integer range 2 .. Phi - 1;
      package Random_Phi is new Ada.Numerics.Discrete_Random (Phi_Range);
      G : Random_Phi.Generator;
      E_Val : Long_Long_Integer;
   begin
      Random_Phi.Reset (G);
      loop
         E_Val := Random_Phi.Random (G);
         exit when GCD (E_Val, Phi) = 1;
      end loop;

      return (Public_Key  => R.To_Element (Integer (E_Val)), 
              Private_Key => Extended_GCD_Inv (E_Val, Phi),
              P_Val => P, Q_Val => Q);
   end Create;

   function Get_Modulo return Positive is (N);
   function Get_Public_Key (Self : RSA_Instance) return R.Element is (Self.Public_Key);

   function Encrypt (Self : RSA_Instance; M : R.Element) return R.Element is
   begin
      return Power (M, Long_Long_Integer (R.To_Integer (Self.Public_Key))); 
   end Encrypt;

   function Decrypt (Self : RSA_Instance; S : R.Element) return R.Element is
   begin
      return Power (S, Self.Private_Key);
   end Decrypt;

end RSA_Pkg;