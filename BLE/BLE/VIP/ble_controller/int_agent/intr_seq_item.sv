
class intr_seq_item extends ovm_sequence_item;

  `ovm_object_utils(intr_seq_item)                                         // Factory registration of interrupt sequence item
 
  bit int_o;                                                              // Interrupt output
  
  extern function void do_print(ovm_printer printer); 
  extern function new(string name = "intr_seq_item");
endclass: intr_seq_item

  function intr_seq_item::new(string name = "intr_seq_item");
   super.new(name);
  endfunction


  function void intr_seq_item::do_print(ovm_printer printer);
   super.do_print(printer);
 
   printer.print_field("interrupt_o", this.int_o, 1, OVM_BIN);
  endfunction
