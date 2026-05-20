package Math_Library is

   type Diofantic_Result is record
      X           : Long_Integer;
      Y           : Long_Integer;
      Is_Solvable : Boolean;
   end record;

   function Gcd (N, M : Long_Integer) return Long_Integer;
   pragma Export (C, Gcd, "ada_gcd");

   function Lcd (N : Long_Integer) return Long_Integer;
   pragma Export (C, Lcd, "ada_lcd");

   function Euler (N : Long_Integer) return Long_Integer;
   pragma Export (C, Euler, "ada_euler");

   function Diofantic (A, B, C_Val : Long_Integer) return Diofantic_Result;
   pragma Export (C, Diofantic, "ada_diofantic");

end Math_Library;