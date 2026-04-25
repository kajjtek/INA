generic
   N : Positive;  
package Ring is
   
   type Element is private;

   function To_Element (V : Integer) return Element;
   function To_Integer (E : Element) return Integer;

   function "+" (Left, Right : Element) return Element;
   function "-" (Left, Right : Element) return Element;
   function "*" (Left, Right : Element) return Element;
   function "/" (Left, Right : Element) return Element;
   function "-" (E : Element)           return Element;  

   function "="  (Left, Right : Element) return Boolean;
   
   function "<"  (Left, Right : Element) return Boolean;
   function "<=" (Left, Right : Element) return Boolean;
   function ">"  (Left, Right : Element) return Boolean;
   function ">=" (Left, Right : Element) return Boolean;

   Not_Invertible : exception;

   procedure Put   (E : in Element);
   procedure Get   (E : out Element);
   function  Image (E : in Element) return String;

private

   type Element is record
      Val : Natural range 0 .. N - 1 := 0;
   end record;

end Ring;