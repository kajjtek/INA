with Ada.Command_Line; use Ada.Command_Line;
with Dining_System;    use Dining_System;

procedure Ex1 is
   Num_Of_Philosophers : Integer := 5;
   Num_Of_Dishes       : Integer := 10;
begin
   if Argument_Count >= 1 then
      Num_Of_Philosophers := Integer'Value (Argument (1));
   end if;
   if Argument_Count >= 2 then
      Num_Of_Dishes := Integer'Value (Argument (2));
   end if;

   declare
      subtype Phil_Range is Integer range 0 .. Num_Of_Philosophers - 1;

      Mailboxes    : array (Phil_Range) of aliased Mailbox;
      Forks        : array (Phil_Range) of Fork_Access;
      Philosophers : array (Phil_Range) of Philosopher_Task;
   begin
      for I in Phil_Range loop
         Forks (I) := new Fork;
      end loop;

      for I in Phil_Range loop
         declare
            Left_Idx : constant Integer :=
              (I - 1 + Num_Of_Philosophers) mod Num_Of_Philosophers;
            Right_Idx : constant Integer :=
              (I + 1) mod Num_Of_Philosophers;

            Init_Left_Fork  : Fork_Access := null;
            Init_Right_Fork : Fork_Access := null;
         begin
            if Left_Idx < I then
               Init_Left_Fork := null;
            else
               Init_Left_Fork := Forks (Left_Idx);
            end if;

            if I < Right_Idx then
               Init_Right_Fork := Forks (I);
            else
               Init_Right_Fork := null;
            end if;

            Philosophers (I).Initialize
              (My_Id              => I,
               Number_Of_Tries    => Num_Of_Dishes,
               Left_Id            => Left_Idx,
               Right_Id           => Right_Idx,
               Left_Box           => Mailboxes (Left_Idx)'Unchecked_Access,
               Right_Box          => Mailboxes (Right_Idx)'Unchecked_Access,
               Left_Fork          => Init_Left_Fork,
               Right_Fork         => Init_Right_Fork,
               My_Box             => Mailboxes (I)'Unchecked_Access);
         end;
      end loop;
   end;
end Ex1;