-- Cialo Z_p dla dowolnego P (liczby pierwszej)
-- Wszystkie operacje sa modulo P, przechowywane jako Long_Long_Integer

generic
   P : Positive;
package Ring is

   type Element is private;

   Zero : constant Element;
   One  : constant Element;

   function Make (V : Long_Long_Integer) return Element;
   function Val  (E : Element) return Long_Long_Integer;

   function "+" (L, R : Element) return Element;
   function "-" (L, R : Element) return Element;
   function "*" (L, R : Element) return Element;
   function "/" (L, R : Element) return Element;
   function "-" (E : Element)    return Element;
   function "=" (L, R : Element) return Boolean;

   Not_Invertible : exception;

private

   type Element is record
      V : Long_Long_Integer := 0;
   end record;

   Zero : constant Element := (V => 0);
   One  : constant Element := (V => 1);

end Ring;