package Message_Pkg is

   type Message is record
      Source_Id   : Integer;
      Message_Id  : Integer;
      Receiver_Id : Integer;
   end record;

end Message_Pkg;