package body Inbox_Pkg is

   protected body Inbox_Type is
      entry Put (Item : Message) when True is
      begin
         List.Append (Item);
         Msg_Count := Msg_Count + 1;
      end Put;

      entry Take (Item : out Message) when Msg_Count > 0 is
      begin
         Item := List.First_Element;
         List.Delete_First;
         Msg_Count := Msg_Count - 1;
      end Take;
   end Inbox_Type;

end Inbox_Pkg;