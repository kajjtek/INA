with Ring;

generic
   N : Positive; 
package RSA_Pkg is
   
   package R is new Ring (N => N);

   type RSA_Instance is private;

   
   function Create (P, Q : Positive) return RSA_Instance;

   
   function Get_Modulo return Positive;
   function Get_Public_Key (Self : RSA_Instance) return R.Element;
   function Encrypt (Self : RSA_Instance; M : R.Element) return R.Element;
   function Decrypt (Self : RSA_Instance; S : R.Element) return R.Element;

private
   type RSA_Instance is record
      Public_Key  : R.Element;
      Private_Key : Long_Long_Integer; 
      P_Val, Q_Val : Positive;
   end record;
end RSA_Pkg;