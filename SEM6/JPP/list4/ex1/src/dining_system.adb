with Ada.Text_IO; use Ada.Text_IO;
with Ada.Numerics.Float_Random; use Ada.Numerics.Float_Random;
with Ada.Real_Time; use Ada.Real_Time;

package body Dining_System is

   protected body Mailbox is
      entry Put (Item : Message) when Count < Max_Queue_Size is
      begin
         Buffer (Tail) := Item;
         Tail := (Tail mod Max_Queue_Size) + 1;
         Count := Count + 1;
      end Put;

      entry Get (Item : out Message) when Count > 0 is
      begin
         Item := Buffer (Head);
         Head := (Head mod Max_Queue_Size) + 1;
         Count := Count - 1;
      end Get;
   end Mailbox;

   task body Philosopher_Task is
      Id          : Philosopher_Id;
      Max_Meals   : Integer;
      State       : State_Type := THINKING;
      Left_Side   : Side;
      Right_Side  : Side;
      Count_Meals : Integer := 0;
      Fail        : Integer := 0;
      Own_Mailbox : Mailbox_Access;
      G           : Generator;

      procedure Send_Request is
      begin
         if Left_Side.Fk = null or else Right_Side.Fk = null then
            Fail := Fail + 1;
         end if;

         if Left_Side.Fk = null then
            declare
               Msg : constant Message :=
                 (Kind => REQUEST_FORK, Source => Id, Fk => null);
            begin
               select
                  Left_Side.Neighbour_Mailbox.Put (Msg);
               or
                  delay 1.0;
               end select;
            end;
         end if;

         if Right_Side.Fk = null then
            declare
               Msg : constant Message :=
                 (Kind => REQUEST_FORK, Source => Id, Fk => null);
            begin
               select
                  Right_Side.Neighbour_Mailbox.Put (Msg);
               or
                  delay 1.0;
               end select;
            end;
         end if;
      end Send_Request;

      procedure Handle_Message (Msg : Message) is
      begin
         case Msg.Kind is
            when REQUEST_FORK =>
               if Msg.Source = Left_Side.Neighbour_Id then
                  if Left_Side.Fk /= null
                    and then Left_Side.Fk.Is_Dirty
                  then
                     Left_Side.Fk.Is_Dirty := False;
                     select
                        Left_Side.Neighbour_Mailbox.Put
                          ((Kind   => GIVE_FORK,
                            Source => Id,
                            Fk     => Left_Side.Fk));
                     or
                        delay 5.0;
                     end select;
                     Left_Side.Fk := null;

                     if State = WANTS_TO_EAT then
                        select
                           Left_Side.Neighbour_Mailbox.Put
                             ((Kind   => REQUEST_FORK,
                               Source => Id,
                               Fk     => null));
                        or
                           delay 1.0;
                        end select;
                     end if;
                  elsif Left_Side.Fk /= null then
                     Left_Side.Pending_Request := True;
                  end if;
               else
                  if Right_Side.Fk /= null
                    and then Right_Side.Fk.Is_Dirty
                  then
                     Right_Side.Fk.Is_Dirty := False;
                     select
                        Right_Side.Neighbour_Mailbox.Put
                          ((Kind   => GIVE_FORK,
                            Source => Id,
                            Fk     => Right_Side.Fk));
                     or
                        delay 5.0;
                     end select;
                     Right_Side.Fk := null;

                     if State = WANTS_TO_EAT then
                        select
                           Right_Side.Neighbour_Mailbox.Put
                             ((Kind   => REQUEST_FORK,
                               Source => Id,
                               Fk     => null));
                        or
                           delay 1.0;
                        end select;
                     end if;
                  elsif Right_Side.Fk /= null then
                     Right_Side.Pending_Request := True;
                  end if;
               end if;

            when GIVE_FORK =>
               if Msg.Source = Left_Side.Neighbour_Id then
                  Left_Side.Fk := Msg.Fk;
               else
                  Right_Side.Fk := Msg.Fk;
               end if;
         end case;
      end Handle_Message;

   begin
      accept Initialize
        (My_Id           : Philosopher_Id;
         Number_Of_Tries : Integer;
         Left_Id         : Philosopher_Id;
         Right_Id        : Philosopher_Id;
         Left_Box        : Mailbox_Access;
         Right_Box       : Mailbox_Access;
         Left_Fork       : Fork_Access;
         Right_Fork      : Fork_Access;
         My_Box          : Mailbox_Access)
      do
         Id        := My_Id;
         Max_Meals := Number_Of_Tries;
         Left_Side :=
           (Neighbour_Id      => Left_Id,
            Neighbour_Mailbox => Left_Box,
            Fk                => Left_Fork,
            Pending_Request   => False);
         Right_Side :=
           (Neighbour_Id      => Right_Id,
            Neighbour_Mailbox => Right_Box,
            Fk                => Right_Fork,
            Pending_Request   => False);
         Own_Mailbox := My_Box;
      end Initialize;

      Reset (G);

      while State /= DONE loop
         case State is
            when THINKING =>
               Put_Line
                 ("Filozof" & Philosopher_Id'Image (Id)
                  & " wszedl w stan THINKING");
               declare
                  Rand_Val : constant Float := Random (G);
                  Think_Ms : constant Integer :=
                    100 + Integer (Rand_Val * 500.0);
                  Think_Time : constant Time_Span :=
                    Milliseconds (Think_Ms);
                  End_Time : constant Time := Clock + Think_Time;
               begin
                  while Clock < End_Time loop
                     declare
                        Time_Left : constant Time_Span := End_Time - Clock;
                        Msg       : Message;
                     begin
                        if Time_Left <= Time_Span_Zero then
                           exit;
                        end if;

                        select
                           Own_Mailbox.Get (Msg);
                           Handle_Message (Msg);
                        or
                           delay To_Duration (Time_Left);
                        end select;
                     end;
                  end loop;
               end;
               State := WANTS_TO_EAT;

            when WANTS_TO_EAT =>
               Put_Line
                 ("Filozof" & Philosopher_Id'Image (Id)
                  & " wszedl w stan WANTS_TO_EAT");
               Send_Request;

               while Left_Side.Fk = null or else Right_Side.Fk = null loop
                  declare
                     Msg : Message;
                  begin
                     Own_Mailbox.Get (Msg);
                     Handle_Message (Msg);
                  end;
               end loop;
               State := EATS;

            when EATS =>
               Put_Line
                 ("Filozof" & Philosopher_Id'Image (Id)
                  & " wszedl w stan EATS");
               declare
                  Rand_Val    : constant Float := Random (G);
                  Eating_Time : constant Duration :=
                    0.2 + Duration (Rand_Val * 0.5);
               begin
                  delay Eating_Time;
               end;

               Left_Side.Fk.Is_Dirty  := True;
               Right_Side.Fk.Is_Dirty := True;
               Count_Meals            := Count_Meals + 1;

               if Left_Side.Pending_Request and then Left_Side.Fk /= null then
                  Left_Side.Fk.Is_Dirty := False;
                  select
                     Left_Side.Neighbour_Mailbox.Put
                       ((Kind   => GIVE_FORK,
                         Source => Id,
                         Fk     => Left_Side.Fk));
                  or
                     delay 5.0;
                  end select;
                  Left_Side.Fk              := null;
                  Left_Side.Pending_Request := False;
               end if;

               if Right_Side.Pending_Request and then Right_Side.Fk /= null
               then
                  Right_Side.Fk.Is_Dirty := False;
                  select
                     Right_Side.Neighbour_Mailbox.Put
                       ((Kind   => GIVE_FORK,
                         Source => Id,
                         Fk     => Right_Side.Fk));
                  or
                     delay 5.0;
                  end select;
                  Right_Side.Fk              := null;
                  Right_Side.Pending_Request := False;
               end if;

               if Count_Meals = Max_Meals then
                  State := DONE;
               else
                  State := THINKING;
               end if;

            when DONE =>
               null;
         end case;
      end loop;

      Put_Line
        (Standard_Error,
         "####### FILOZOF" & Philosopher_Id'Image (Id)
         & " JEST DONE ####### LICZBA FAILOW: "
         & Integer'Image (Fail) & " ##############");

      while Left_Side.Fk /= null or else Right_Side.Fk /= null loop
         declare
            Msg     : Message;
            Got_Msg : Boolean := False;
         begin
            select
               Own_Mailbox.Get (Msg);
               Got_Msg := True;
            or
               delay 1.0;
            end select;

            if Got_Msg then
               Handle_Message (Msg);
            else
               exit;
            end if;
         end;
      end loop;

   end Philosopher_Task;

end Dining_System;