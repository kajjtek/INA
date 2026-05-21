with Message_Pkg; use Message_Pkg;
with Ada.Containers.Doubly_Linked_Lists;

package Inbox_Pkg is

   package Message_Lists is new Ada.Containers.Doubly_Linked_Lists (Message);

   protected type Inbox_Type is
      entry Put (Item : Message);
      entry Take (Item : out Message);
   private
      List      : Message_Lists.List;
      Msg_Count : Natural := 0; -- Licznik usuwa ostrzeżenie o barierze wejścia
   end Inbox_Type;

   type Inbox_Access is access all Inbox_Type;
   type Inbox_Access_Array is array (Integer range <>) of Inbox_Access;
   type Inbox_Access_Array_Ptr is access all Inbox_Access_Array;

end Inbox_Pkg;