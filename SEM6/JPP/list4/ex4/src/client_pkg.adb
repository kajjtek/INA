with Ada.Text_IO; use Ada.Text_IO;
with Ada.Numerics.Discrete_Random;
with Message_Pkg; use Message_Pkg;

package body Client_Pkg is

   task body Client_Task is
      Received_Count : Integer := 0;
      Msg_Left       : Integer := Number_Of_Messages;

      task Sender_Task;

      task body Sender_Task is
         subtype Client_Range is Integer range 0 .. (Number_Of_Clients - 1);
         package Random_Client is new Ada.Numerics.Discrete_Random
           (Client_Range);
         G      : Random_Client.Generator;
         Chosen : Integer;
         Count  : Integer := 0;
      begin
         Random_Client.Reset (G);

         while Msg_Left > 0 loop
            Chosen := Random_Client.Random (G);

            Put_Line ("[CLIENT " & Integer'Image (Id) &
                      "] #### SENT ### msg id " & Integer'Image (Count));

            Server.Put
              (Message'(Source_Id   => Id,
                        Message_Id  => Count,
                        Receiver_Id => Chosen));

            Count    := Count + 1;
            Msg_Left := Msg_Left - 1;
         end loop;

         Server.Put
           (Message'(Source_Id   => Id,
                     Message_Id  => -1,
                     Receiver_Id => -1));
         Put_Line ("[CLIENT " & Integer'Image (Id) & "] #### NOT SENDING");
      end Sender_Task;

      Msg : Message;
   begin
      loop
         Inbox.Take (Msg);

         if Msg.Message_Id = -999 then
            Put_Line ("[CLIENT " & Integer'Image (Id) &
                      "] #### SHUTDOWN RECEIVED");
            exit;
         else
            Put_Line ("[CLIENT " & Integer'Image (Id) &
                      "] #### RECEIVE ### msg id " &
                      Integer'Image (Msg.Message_Id) &
                      " # FROM " & Integer'Image (Msg.Source_Id));
            Received_Count := Received_Count + 1;
         end if;
      end loop;

      Put_Line ("[CLIENT " & Integer'Image (Id) &
                "] #### RECEIVED " & Integer'Image (Received_Count) &
                " SIGNALS");
   end Client_Task;

end Client_Pkg;