with Ada.Text_IO; use Ada.Text_IO;
with RSA_Pkg;

procedure Main is
   -- Charakterystyka: 10007 * 10009 = 100160063
   Modulus : constant Positive := 100160063;
   
   package My_RSA is new RSA_Pkg (N => Modulus);
   use My_RSA;

   RSA : RSA_Instance;
   -- Używamy Twojego typu Element z instancji pierścienia wewnątrz RSA
   Msg : My_RSA.R.Element;
   Enc : My_RSA.R.Element;
   Dec : My_RSA.R.Element;

begin
   Put_Line ("--- RSA Test (Ada) ---");
   
   RSA := Create (10007, 10009);
   
   Put_Line ("Modulo: " & Positive'Image (Get_Modulo));
   Put_Line ("Public Key: " & My_RSA.R.Image (Get_Public_Key (RSA)));

   -- Test wiadomości
   Msg := My_RSA.R.To_Element (987654); -- 
   Put_Line ("Wiadomosc: " & My_RSA.R.Image (Msg));

   Enc := Encrypt (RSA, Msg);
   Put_Line ("Szyfrogram: " & My_RSA.R.Image (Enc));

   Dec := Decrypt (RSA, Enc);
   Put_Line ("Po deszyfrowaniu: " & My_RSA.R.Image (Dec));

   if My_RSA.R.To_Integer (Msg) = My_RSA.R.To_Integer (Dec) then
      Put_Line ("Wynik: POPRAWNY");
   else
      Put_Line ("Wynik: BLAD");
   end if;
end Main;
