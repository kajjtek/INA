pragma Warnings (Off);
pragma Ada_95;
with System;
with System.Parameters;
with System.Secondary_Stack;
package ada_main is

   procedure adainit;
   pragma Export (C, adainit, "adainit");

   procedure adafinal;
   pragma Export (C, adafinal, "adafinal");

   type Version_32 is mod 2 ** 32;
   u00001 : constant Version_32 := 16#c3672c71#;
   pragma Export (C, u00001, "math_libraryB");
   u00002 : constant Version_32 := 16#2afee288#;
   pragma Export (C, u00002, "math_libraryS");
   u00003 : constant Version_32 := 16#b2cfab41#;
   pragma Export (C, u00003, "system__standard_libraryB");
   u00004 : constant Version_32 := 16#6278fccd#;
   pragma Export (C, u00004, "system__standard_libraryS");
   u00005 : constant Version_32 := 16#70765b54#;
   pragma Export (C, u00005, "systemS");
   u00006 : constant Version_32 := 16#0ddbd91f#;
   pragma Export (C, u00006, "system__memoryB");
   u00007 : constant Version_32 := 16#68e2c74e#;
   pragma Export (C, u00007, "system__memoryS");
   u00008 : constant Version_32 := 16#33a162cd#;
   pragma Export (C, u00008, "ada__exceptionsB");
   u00009 : constant Version_32 := 16#00870947#;
   pragma Export (C, u00009, "ada__exceptionsS");
   u00010 : constant Version_32 := 16#76789da1#;
   pragma Export (C, u00010, "adaS");
   u00011 : constant Version_32 := 16#85bf25f7#;
   pragma Export (C, u00011, "ada__exceptions__last_chance_handlerB");
   u00012 : constant Version_32 := 16#a028f72d#;
   pragma Export (C, u00012, "ada__exceptions__last_chance_handlerS");
   u00013 : constant Version_32 := 16#7fa0a598#;
   pragma Export (C, u00013, "system__soft_linksB");
   u00014 : constant Version_32 := 16#a3fdee7d#;
   pragma Export (C, u00014, "system__soft_linksS");
   u00015 : constant Version_32 := 16#d0b087d0#;
   pragma Export (C, u00015, "system__secondary_stackB");
   u00016 : constant Version_32 := 16#debd0a58#;
   pragma Export (C, u00016, "system__secondary_stackS");
   u00017 : constant Version_32 := 16#a43efea2#;
   pragma Export (C, u00017, "system__parametersB");
   u00018 : constant Version_32 := 16#45e1a745#;
   pragma Export (C, u00018, "system__parametersS");
   u00019 : constant Version_32 := 16#bca88fbc#;
   pragma Export (C, u00019, "system__storage_elementsS");
   u00020 : constant Version_32 := 16#0286ce9f#;
   pragma Export (C, u00020, "system__soft_links__initializeB");
   u00021 : constant Version_32 := 16#ac2e8b53#;
   pragma Export (C, u00021, "system__soft_links__initializeS");
   u00022 : constant Version_32 := 16#8599b27b#;
   pragma Export (C, u00022, "system__stack_checkingB");
   u00023 : constant Version_32 := 16#b7294e42#;
   pragma Export (C, u00023, "system__stack_checkingS");
   u00024 : constant Version_32 := 16#45e1965e#;
   pragma Export (C, u00024, "system__exception_tableB");
   u00025 : constant Version_32 := 16#fd5d2d4d#;
   pragma Export (C, u00025, "system__exception_tableS");
   u00026 : constant Version_32 := 16#42d3e466#;
   pragma Export (C, u00026, "system__exceptionsS");
   u00027 : constant Version_32 := 16#c367aa24#;
   pragma Export (C, u00027, "system__exceptions__machineB");
   u00028 : constant Version_32 := 16#ec13924a#;
   pragma Export (C, u00028, "system__exceptions__machineS");
   u00029 : constant Version_32 := 16#7706238d#;
   pragma Export (C, u00029, "system__exceptions_debugB");
   u00030 : constant Version_32 := 16#40780307#;
   pragma Export (C, u00030, "system__exceptions_debugS");
   u00031 : constant Version_32 := 16#52e91815#;
   pragma Export (C, u00031, "system__img_intS");
   u00032 : constant Version_32 := 16#f2c63a02#;
   pragma Export (C, u00032, "ada__numericsS");
   u00033 : constant Version_32 := 16#174f5472#;
   pragma Export (C, u00033, "ada__numerics__big_numbersS");
   u00034 : constant Version_32 := 16#8a5c240d#;
   pragma Export (C, u00034, "system__unsigned_typesS");
   u00035 : constant Version_32 := 16#5c7d9c20#;
   pragma Export (C, u00035, "system__tracebackB");
   u00036 : constant Version_32 := 16#f6ecafe9#;
   pragma Export (C, u00036, "system__tracebackS");
   u00037 : constant Version_32 := 16#5f6b6486#;
   pragma Export (C, u00037, "system__traceback_entriesB");
   u00038 : constant Version_32 := 16#b86ae4d8#;
   pragma Export (C, u00038, "system__traceback_entriesS");
   u00039 : constant Version_32 := 16#65d5266b#;
   pragma Export (C, u00039, "system__traceback__symbolicB");
   u00040 : constant Version_32 := 16#140ceb78#;
   pragma Export (C, u00040, "system__traceback__symbolicS");
   u00041 : constant Version_32 := 16#701f9d88#;
   pragma Export (C, u00041, "ada__exceptions__tracebackB");
   u00042 : constant Version_32 := 16#26ed0985#;
   pragma Export (C, u00042, "ada__exceptions__tracebackS");
   u00043 : constant Version_32 := 16#f9910acc#;
   pragma Export (C, u00043, "system__address_imageB");
   u00044 : constant Version_32 := 16#d19ac66e#;
   pragma Export (C, u00044, "system__address_imageS");
   u00045 : constant Version_32 := 16#45c8b1f1#;
   pragma Export (C, u00045, "system__img_address_32S");
   u00046 : constant Version_32 := 16#9111f9c1#;
   pragma Export (C, u00046, "interfacesS");
   u00047 : constant Version_32 := 16#68e81073#;
   pragma Export (C, u00047, "system__img_address_64S");
   u00048 : constant Version_32 := 16#fd158a37#;
   pragma Export (C, u00048, "system__wch_conB");
   u00049 : constant Version_32 := 16#a9757837#;
   pragma Export (C, u00049, "system__wch_conS");
   u00050 : constant Version_32 := 16#5c289972#;
   pragma Export (C, u00050, "system__wch_stwB");
   u00051 : constant Version_32 := 16#84645436#;
   pragma Export (C, u00051, "system__wch_stwS");
   u00052 : constant Version_32 := 16#7cd63de5#;
   pragma Export (C, u00052, "system__wch_cnvB");
   u00053 : constant Version_32 := 16#afb5b247#;
   pragma Export (C, u00053, "system__wch_cnvS");
   u00054 : constant Version_32 := 16#e538de43#;
   pragma Export (C, u00054, "system__wch_jisB");
   u00055 : constant Version_32 := 16#1a02d06d#;
   pragma Export (C, u00055, "system__wch_jisS");
   u00056 : constant Version_32 := 16#fb523cdb#;
   pragma Export (C, u00056, "system__crtlS");

   --  BEGIN ELABORATION ORDER
   --  ada%s
   --  interfaces%s
   --  system%s
   --  system.parameters%s
   --  system.parameters%b
   --  system.crtl%s
   --  system.storage_elements%s
   --  system.img_address_32%s
   --  system.img_address_64%s
   --  system.stack_checking%s
   --  system.stack_checking%b
   --  system.traceback_entries%s
   --  system.traceback_entries%b
   --  system.unsigned_types%s
   --  system.wch_con%s
   --  system.wch_con%b
   --  system.wch_jis%s
   --  system.wch_jis%b
   --  system.wch_cnv%s
   --  system.wch_cnv%b
   --  system.traceback%s
   --  system.traceback%b
   --  system.secondary_stack%s
   --  system.standard_library%s
   --  ada.exceptions%s
   --  system.exceptions_debug%s
   --  system.exceptions_debug%b
   --  system.soft_links%s
   --  system.wch_stw%s
   --  system.wch_stw%b
   --  ada.exceptions.last_chance_handler%s
   --  ada.exceptions.last_chance_handler%b
   --  ada.exceptions.traceback%s
   --  ada.exceptions.traceback%b
   --  system.address_image%s
   --  system.address_image%b
   --  system.exception_table%s
   --  system.exception_table%b
   --  ada.numerics%s
   --  ada.numerics.big_numbers%s
   --  system.exceptions%s
   --  system.exceptions.machine%s
   --  system.exceptions.machine%b
   --  system.img_int%s
   --  system.memory%s
   --  system.memory%b
   --  system.secondary_stack%b
   --  system.soft_links.initialize%s
   --  system.soft_links.initialize%b
   --  system.soft_links%b
   --  system.standard_library%b
   --  system.traceback.symbolic%s
   --  system.traceback.symbolic%b
   --  ada.exceptions%b
   --  math_library%s
   --  math_library%b
   --  END ELABORATION ORDER

end ada_main;
