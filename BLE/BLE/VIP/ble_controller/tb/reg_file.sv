class reg_file extends ovm_object;
   
   `ovm_object_utils(reg_file)

   bit [5:0] cmd_reg = 'b0000001;
   bit [4:0] adv_reg = 'b01000;

// cmd_reg[0] - start adv
// cmd_reg[1] - stop adv
// cmd_reg[2] - start scan
// cmd_reg[3] - stop scan
// cmd_reg[4] - start init
// cmd_reg[5] - soft RESET

// adv_reg[0] - start undir_adv
// adv_reg[1] - start dir_adv_ld low duty cycle
// adv_reg[2] - start scannable_adv
// adv_reg[3] - start non_conn_adv
// adv_reg[4] - start dir_adv_hd high duty cycle

   function new(string name = "reg_file");
   
      super.new(name);

   endfunction



endclass
