with Ring;

generic
   P : Positive;
package DH is

   package Field is new Ring (P => P);
   subtype Elem is Field.Element;

   type Setup_Type is private;
   type User_Type  is private;

   function  Create_Setup    return Setup_Type;
   function  Get_Generator   (S : Setup_Type) return Elem;
   function  Power           (Base : Elem;
                              Exp  : Long_Long_Integer) return Elem;

   function  Create_User     (S : Setup_Type) return User_Type;
   function  Get_Public_Key  (U : User_Type)  return Elem;
   function  Get_Shared_Key  (U : User_Type)  return Elem;
   procedure Set_Key         (U : in out User_Type; A : Elem);
   function  Encrypt         (U : User_Type; M : Elem) return Elem;
   function  Decrypt         (U : User_Type; C : Elem) return Elem;

   Key_Not_Set : exception;

private

   type Setup_Type is record
      Generator : Elem;
   end record;

   type User_Type is record
      Setup         : Setup_Type;
      Secret        : Long_Long_Integer;
      Shared_Secret : Elem;
      Has_Key       : Boolean;
   end record;

end DH;