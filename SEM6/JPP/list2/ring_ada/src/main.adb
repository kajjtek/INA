-- main.adb – program testujący typ generyczny Ring

with Ada.Text_IO;         use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Ring;

procedure Main is

   -- ── Konkretyzacje szablonu ────────────────────────────────────────────────

   package R7  is new Ring (7);   -- ciało (7 pierwsze)
   package R13 is new Ring (13);  -- ciało (13 pierwsze)
   package R6  is new Ring (6);   -- pierścień NIE będący ciałem

   -- use potrzebne, żeby operatory na prywatnym typie Element były widoczne
   use R7; use R13; use R6;

   -- ── Pomocnik ──────────────────────────────────────────────────────────────

   procedure Sep (Title : String) is
   begin
      New_Line;
      Put_Line ("-- " & Title & " --");
   end Sep;

   procedure Expect (Label : String; Got, Expected : Integer) is
   begin
      Put (Label & ": ");
      Put (Got, Width => 0);
      Put ("  (oczekiwano " & Integer'Image (Expected) & ")");
      if Got = Expected then
         Put_Line ("OK");
      else
         Put_Line ("  ERR BLAD");
      end if;
   end Expect;

begin

   -- ── 1. Arytmetyka w Z/7Z ──────────────────────────────────────────────────
   Sep ("Arytmetyka w Z/7Z");

   declare
      A : R7.Element := R7.To_Element (3);
      B : R7.Element := R7.To_Element (5);
   begin
      Put_Line ("a = 3,  b = 5  (mod 7)");
      Expect ("a + b", R7.To_Integer (A + B), 1);
      Expect ("a - b", R7.To_Integer (A - B), 5);
      Expect ("a * b", R7.To_Integer (A * B), 1);
      Expect ("a / b", R7.To_Integer (A / B), 2);
      Expect ("  -a ", R7.To_Integer (-A),    4);
   end;

   -- -- 2. Redukcja duzych / ujemnych wartosci ------------------------------
   Sep ("Redukcja wartosci wejsciowych");

   declare
      Big : R7.Element := R7.To_Element (100);   -- 100 mod 7 = 2
      Neg : R7.Element := R7.To_Element (-3);    -- -3  mod 7 = 4
   begin
      Expect ("To_Element(100)", R7.To_Integer (Big), 2);
      Expect ("To_Element(-3) ", R7.To_Integer (Neg), 4);
   end;

   -- ── 3. Porównania ─────────────────────────────────────────────────────────
   Sep ("Porównania w Z/7Z");

   declare
      X : R7.Element := R7.To_Element (2);
      Y : R7.Element := R7.To_Element (5);

      function B (V : Boolean) return Integer is (if V then 1 else 0);
   begin
      Expect ("x=2, y=5 :  x=y ", B (X =  Y), 0);
      Expect ("x=2, y=5 : x/=y ", B (X /= Y), 1);
      Expect ("x=2, y=5 :  x<y ", B (X <  Y), 1);
      Expect ("x=2, y=5 : x<=y ", B (X <= Y), 1);
      Expect ("x=2, y=5 :  x>y ", B (X >  Y), 0);
      Expect ("x=2, y=5 : x>=y ", B (X >= Y), 0);
   end;

   -- -- 4. Male twierdzenie Fermata (a^12 == 1 mod 13) ---------------------
   Sep ("Male twierdzenie Fermata w Z/13Z");

   declare
      Bases : constant array (1 .. 4) of Integer := (2, 5, 7, 11);
   begin
      for I in Bases'Range loop
         declare
            Cur : R13.Element := R13.To_Element (1);
            Elem : constant R13.Element := R13.To_Element (Bases (I));
         begin
            for J in 1 .. 12 loop
               Cur := Cur * Elem;
            end loop;
            Put (Integer'Image (Bases (I)) & "^12 mod 13 = ");
            R13.Put (Cur);
              Put_Line ("  (oczekiwano 1)" &
                        (if R13.To_Integer (Cur) = 1 then "  OK" else "  ERR"));
         end;
      end loop;
   end;

   -- -- 5. Wyjatek: element nieodwracalny ---------------------------------
   Sep ("Wyjatek: element nieodwracalny w Z/6Z");

   declare
      P : R6.Element := R6.To_Element (4);
      Q : R6.Element := R6.To_Element (3);   -- gcd(3,6)=3 ≠ 1
   begin
   Put_Line ("Proba 4 / 3 w Z/6Z ...");
      declare
         Wynik : R6.Element := P / Q;
         pragma Unreferenced (Wynik);
      begin
         Put_Line ("BLAD: wyjatek nie zostal rzucony!");
      end;
   exception
      when R6.Not_Invertible =>
         Put_Line ("Zlapano oczekiwany wyjatek Ring.Not_Invertible  OK");
   end;

   -- -- 6. Operator Image / Put --------------------------------------------
   Sep ("I/O - Image i Put");

   declare
      E : R7.Element := R7.To_Element (13);   -- 13 mod 7 = 6
   begin
   Put ("To_Element(13) w Z/7Z -> Image: ");
      Put_Line (R7.Image (E) & "  (oczekiwano 6)");
   Put ("To_Element(13) w Z/7Z -> Put:   ");
      R7.Put (E);
      New_Line;
   end;

   New_Line;
   Put_Line ("Wszystkie testy zakonczone.");

end Main;