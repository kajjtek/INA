with Message_Pkg; use Message_Pkg;
with Inbox_Pkg;   use Inbox_Pkg;

package Server_Pkg is

   task type Server_Task (N : Integer; Clients : Inbox_Access_Array_Ptr) is
      entry Put (Msg : Message);
   end Server_Task;

   type Server_Access is access all Server_Task;

end Server_Pkg;