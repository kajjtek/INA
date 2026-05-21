with Server_Pkg; use Server_Pkg;
with Inbox_Pkg;  use Inbox_Pkg;

package Client_Pkg is

   task type Client_Task (
      Id                 : Integer;
      Number_Of_Clients  : Integer;
      Number_Of_Messages : Integer;
      Server             : Server_Access;
      Inbox              : Inbox_Access
   );

   type Client_Access is access all Client_Task;

end Client_Pkg;