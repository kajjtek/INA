with Ada.Text_IO; use Ada.Text_IO;
with DH;

procedure Main is

   P_Val : constant Positive := 1_234_567_891;

   package My_DH is new DH (P => P_Val);

   Setup     : constant My_DH.Setup_Type := My_DH.Create_Setup;
   Alice     : My_DH.User_Type           := My_DH.Create_User (Setup);
   Bob       : My_DH.User_Type           := My_DH.Create_User (Setup);

   Alice_Pub : constant My_DH.Elem := My_DH.Get_Public_Key (Alice);
   Bob_Pub   : constant My_DH.Elem := My_DH.Get_Public_Key (Bob);

   Msg : constant My_DH.Elem := My_DH.Field.Make (987_654_321);
   Enc : My_DH.Elem;
   Dec : My_DH.Elem;

begin
   Put_Line ("=== Protokol Diffie-Hellman (Ada) ===");
   Put_Line ("P         =" & P_Val'Image);
   Put_Line ("Generator =" &
      Long_Long_Integer'Image
        (My_DH.Field.Val (My_DH.Get_Generator (Setup))));
   Put_Line ("Alice pub =" &
      Long_Long_Integer'Image (My_DH.Field.Val (Alice_Pub)));
   Put_Line ("Bob   pub =" &
      Long_Long_Integer'Image (My_DH.Field.Val (Bob_Pub)));

   My_DH.Set_Key (Alice, Bob_Pub);
   My_DH.Set_Key (Bob,   Alice_Pub);

   Put_Line ("Wspolny sekret Alice =" &
      Long_Long_Integer'Image
        (My_DH.Field.Val (My_DH.Get_Shared_Key (Alice))));
   Put_Line ("Wspolny sekret Bob   =" &
      Long_Long_Integer'Image
        (My_DH.Field.Val (My_DH.Get_Shared_Key (Bob))));

   Put_Line ("Wiadomosc    =" &
      Long_Long_Integer'Image (My_DH.Field.Val (Msg)));

   Enc := My_DH.Encrypt (Alice, Msg);
   Put_Line ("Zaszyfrowana =" &
      Long_Long_Integer'Image (My_DH.Field.Val (Enc)));

   Dec := My_DH.Decrypt (Bob, Enc);
   Put_Line ("Odszyfrowana =" &
      Long_Long_Integer'Image (My_DH.Field.Val (Dec)));

   if My_DH.Field.Val (Dec) = My_DH.Field.Val (Msg) then
      Put_Line (">>> SUKCES!");
   else
      Put_Line (">>> BLAD!");
   end if;

exception
   when My_DH.Key_Not_Set =>
      Put_Line ("BLAD: klucz nie ustawiony!");
end Main;