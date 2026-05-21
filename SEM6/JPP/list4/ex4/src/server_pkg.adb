with Ada.Text_IO; use Ada.Text_IO;

package body Server_Pkg is

   task body Server_Task is
      Sending_Clients : Integer := N;
      Current_Msg     : Message;
      Target_Inbox    : Inbox_Access;
   begin
      while Sending_Clients > 0 loop
         accept Put (Msg : Message) do
            Current_Msg := Msg;
         end Put;

         Put_Line ("[SERVER] RECEIVED MESSAGE: " &
                   Integer'Image (Current_Msg.Message_Id) &
                   " FROM: " & Integer'Image (Current_Msg.Source_Id) &
                   " TO: " & Integer'Image (Current_Msg.Receiver_Id));

         if Current_Msg.Message_Id = -1 then
            Sending_Clients := Sending_Clients - 1;
         else
            Target_Inbox := Clients.all (Current_Msg.Receiver_Id);
            Target_Inbox.Put (Current_Msg);
         end if;
      end loop;

      for I in Clients.all'Range loop
         Clients.all (I).Put
           (Message'(Source_Id   => -1,
                     Message_Id  => -999,
                     Receiver_Id => I));
      end loop;

      Put_Line ("[SERVER] shutting down");
   end Server_Task;

end Server_Pkg;