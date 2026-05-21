package Dining_System is

   type State_Type is (THINKING, WANTS_TO_EAT, EATS, DONE);
   type Message_Kind is (GIVE_FORK, REQUEST_FORK);

   type Fork is record
      Is_Dirty : Boolean := True;
   end record;
   type Fork_Access is access Fork;

   subtype Philosopher_Id is Integer;

   type Message is record
      Kind   : Message_Kind;
      Source : Philosopher_Id;
      Fk     : Fork_Access;
   end record;

   Max_Queue_Size : constant := 50;
   type Message_Array is array (1 .. Max_Queue_Size) of Message;

   protected type Mailbox is
      entry Put (Item : Message);
      entry Get (Item : out Message);
   private
      Head   : Integer := 1;
      Tail   : Integer := 1;
      Count  : Integer := 0;
      Buffer : Message_Array;
   end Mailbox;

   type Mailbox_Access is access all Mailbox;

   type Side is record
      Neighbour_Id      : Philosopher_Id;
      Neighbour_Mailbox : Mailbox_Access;
      Fk                : Fork_Access := null;
      Pending_Request   : Boolean := False;
   end record;

   task type Philosopher_Task is
      entry Initialize
        (My_Id           : Philosopher_Id;
         Number_Of_Tries : Integer;
         Left_Id         : Philosopher_Id;
         Right_Id        : Philosopher_Id;
         Left_Box        : Mailbox_Access;
         Right_Box       : Mailbox_Access;
         Left_Fork       : Fork_Access;
         Right_Fork      : Fork_Access;
         My_Box          : Mailbox_Access);
   end Philosopher_Task;

end Dining_System;