with Ada.Text_IO; use Ada.Text_IO;
with Math_Library; 
with Foreign_Libs; 

procedure Main_Test is
   package LIO is new Ada.Text_IO.Integer_IO (Long_Integer);
   
   A : constant Long_Integer := 48;
   B : constant Long_Integer := 18;
   
   Res_Ada, Res_C, Res_Rust : Long_Integer;
   DC   : Foreign_Libs.Diofantic_Result;
   DAda : Math_Library.Diofantic_Result;
   DRust: Foreign_Libs.Diofantic_Result;
   
   
   procedure Print_Test (Label : String; V_Ada, V_C, V_Rust : Long_Integer) is
   begin
      Put (Label & " | Ada: ");
      LIO.Put (V_Ada, 0);
      Put (" | C: ");
      LIO.Put (V_C, 0);
      Put (" | Rust: ");
      LIO.Put (V_Rust, 0);
      
      if V_Ada = V_C and V_C = V_Rust then
         Put_Line (" | Status: OK");
      else
         Put_Line (" | Status: BŁĄD");
      end if;
   end Print_Test;

begin
   Res_Ada := Math_Library.Gcd (A, B);
   Res_C   := Foreign_Libs.C_Gcd (A, B);
   Res_Rust := Foreign_Libs.Rust_Gcd (A, B);
   Print_Test ("GCD(48,18)", Res_Ada, Res_C, Res_Rust);

   
   Print_Test ("LCD(15)   ", 
               Math_Library.Lcd (15), 
               Foreign_Libs.C_Lcd (15), 
               Foreign_Libs.Rust_Lcd (15));

   
   Print_Test ("Euler(10) ", 
               Math_Library.Euler (10), 
               Foreign_Libs.C_Euler (10), 
               Foreign_Libs.Rust_Euler (10));

    DC   := Foreign_Libs.C_Diofantic (10, 6, 2);
   DAda := Math_Library.Diofantic (10, 6, 2);
   DRust:= Foreign_Libs.Rust_Diofantic (10, 6, 2);

   Put ("Diofantic | C: x="); LIO.Put(DC.X, 0); Put(", y="); LIO.Put(DC.Y, 0);
   Put (" | Ada: x="); LIO.Put(DAda.X, 0); Put(", y="); LIO.Put(DAda.Y, 0);
   Put (" | Rust: x="); LIO.Put(DRust.X, 0); Put(", y="); LIO.Put(DRust.Y, 0);
   New_Line;

end Main_Test;