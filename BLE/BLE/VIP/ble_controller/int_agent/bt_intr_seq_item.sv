class bt_intr_seq_item extends ovm_sequence_item;
 
 `ovm_object_utils(bt_intr_seq_item)

 parameter PDU_TYPE_WIDTH = 4, 
            ADV_LENGTH = 6,
	    DATA_LENGTH = 5,
	    LLID = 2;

   logic  data;                                                    // if 0 -> Advertising PKT fields, if 1 :-> Data PKT field
   logic  [(PDU_TYPE_WIDTH-1):0] adv_pdu_type;                     
   logic  [(LLID-1):0] llid;                                       // 01 or 10 -> Data PDU
                                                                   // 11 -> Control PDU
   logic  [(ADV_LENGTH-1):0] adv_length;
   logic  [(DATA_LENGTH-1):0] data_length;

  extern function new(string name = "bt_intr_seq_item");
  extern function void do_print(ovm_printer printer); 
  
endclass


  function bt_intr_seq_item::new(string name = "bt_intr_seq_item");
   super.new(name);
  endfunction


  function void bt_intr_seq_item::do_print(ovm_printer printer);
   super.do_print(printer);
   printer.print_field("data", this.data, 1, OVM_BIN);
   printer.print_field("ADV pdu type", this.adv_pdu_type, 4, OVM_BIN);
   printer.print_field("ADV length", this.adv_length, 6, OVM_UNSIGNED);
   printer.print_field("Data length", this.data_length, 5, OVM_UNSIGNED);
   printer.print_field("LLID", this.llid, 2, OVM_BIN);
  endfunction
