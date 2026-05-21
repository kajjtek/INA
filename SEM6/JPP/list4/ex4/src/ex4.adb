with Ada.Command_Line; use Ada.Command_Line;
with Inbox_Pkg;        use Inbox_Pkg;
with Server_Pkg;       use Server_Pkg;
with Client_Pkg;       use Client_Pkg;

procedure Ex4 is
   N : Integer := 5;
   M : Integer := 10;

   Inboxes : Inbox_Access_Array_Ptr;
   Server  : Server_Access;

   type Client_Access_Array is array (Integer range <>) of Client_Access;
   type Client_Access_Array_Ptr is access all Client_Access_Array;
   Clients : Client_Access_Array_Ptr;
begin
   if Argument_Count >= 1 then
      N := Integer'Value (Argument (1));
   end if;
   if Argument_Count >= 2 then
      M := Integer'Value (Argument (2));
   end if;

   Inboxes := new Inbox_Access_Array (0 .. N - 1);
   for I in 0 .. N - 1 loop
      Inboxes (I) := new Inbox_Type;
   end loop;

   Server := new Server_Task (N, Inboxes);

   Clients := new Client_Access_Array (0 .. N - 1);
   for I in 0 .. N - 1 loop
      Clients (I) := new Client_Task
        (Id                 => I,
         Number_Of_Clients  => N,
         Number_Of_Messages => M,
         Server             => Server,
         Inbox              => Inboxes (I));
   end loop;

end Ex4;