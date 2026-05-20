package Foreign_Libs is

   type Diofantic_Result is record
      X           : Long_Integer;
      Y           : Long_Integer;
      Is_Solvable : Boolean;
   end record;
   pragma Convention (C, Diofantic_Result); 

   function C_Gcd (N, M : Long_Integer) return Long_Integer;
   pragma Import (C, C_Gcd, "gcd_C");

   function C_Lcd (N : Long_Integer) return Long_Integer;
   pragma Import (C, C_Lcd, "lcd_C");

   function C_Euler (N : Long_Integer) return Long_Integer;
   pragma Import (C, C_Euler, "euler_C");

   function C_Diofantic (A, B, C_Val : Long_Integer) return Diofantic_Result;
   pragma Import (C, C_Diofantic, "diofantic_C");

   function Rust_Gcd (N, M : Long_Integer) return Long_Integer;
   pragma Import (C, Rust_Gcd, "rust_gcd"); 

   function Rust_Lcd (N : Long_Integer) return Long_Integer;
   pragma Import (C, Rust_Lcd, "rust_lcd");

   function Rust_Euler (N : Long_Integer) return Long_Integer;
   pragma Import (C, Rust_Euler, "rust_euler");

   function Rust_Diofantic (A, B, C_Val : Long_Integer) return Diofantic_Result;
   pragma Import (C, Rust_Diofantic, "rust_diofantic");

end Foreign_Libs;