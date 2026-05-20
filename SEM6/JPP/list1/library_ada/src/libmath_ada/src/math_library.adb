package body Math_Library is

   function Check_Natural (X : Long_Integer) return Boolean is
   begin
      return X > 0;
   end Check_Natural;

   function Gcd (N, M : Long_Integer) return Long_Integer is
      Temp_N : Long_Integer := N;
      Temp_M : Long_Integer := M;
      Temp   : Long_Integer;
   begin
      if not (Check_Natural (Temp_N) and Check_Natural (Temp_M)) then
         return -1;
      end if;

      while Temp_M > 0 loop
         Temp := Temp_M;
         Temp_M := Temp_N mod Temp_M;
         Temp_N := Temp;
      end loop;

      return Temp_N;
   end Gcd;

   function Lcd (N : Long_Integer) return Long_Integer is
      I : Long_Integer := 3;
   begin
      if N < 2 then
         return -1;
      end if;

      if N mod 2 = 0 then
         return 2;
      end if;

      while I * I <= N loop
         if N mod I = 0 then
            return I;
         end if;
         I := I + 2;
      end loop;

      return N;
   end Lcd;

   function Euler (N : Long_Integer) return Long_Integer is
      Result : Long_Integer := N;
      Temp_N : Long_Integer := N;
      I      : Long_Integer := 3;
   begin
      if Temp_N < 1 then
         return -1;
      end if;

      if Temp_N mod 2 = 0 then
         Result := Result - Result / 2;
         while Temp_N mod 2 = 0 loop
            Temp_N := Temp_N / 2;
         end loop;
      end if;

      while I * I <= Temp_N loop
         if Temp_N mod I = 0 then
            Result := Result - Result / I;
            while Temp_N mod I = 0 loop
               Temp_N := Temp_N / I;
            end loop;
         end if;
         I := I + 2;
      end loop;

      if Temp_N > 1 then
         Result := Result - Result / Temp_N;
      end if;

      return Result;
   end Euler;

   procedure Extended_GCD (A, B : Long_Integer; X, Y, G : out Long_Integer) is
      X1, Y1 : Long_Integer;
   begin
      if B = 0 then
         X := 1;
         Y := 0;
         G := A;
      else
         Extended_GCD (B, A mod B, X1, Y1, G);
         X := Y1;
         Y := X1 - (A / B) * Y1;
      end if;
   end Extended_GCD;

   function Diofantic (A, B, C_Val : Long_Integer) return Diofantic_Result is
      X, Y, G : Long_Integer;
      Scale   : Long_Integer;
   begin
      Extended_GCD (A, B, X, Y, G);

      if C_Val mod G /= 0 then
         return (X => 0, Y => 0, Is_Solvable => False);
      end if;

      Scale := C_Val / G;
      return (X => X * Scale, Y => Y * Scale, Is_Solvable => True);
   end Diofantic;

end Math_Library;