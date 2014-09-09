`include "define.sv"
`include "tb_defs.sv"
`include "seq_item.sv"

class ahb_base_seq extends ovm_sequence #(ahb_seq_item);
  `ovm_object_utils(ahb_base_seq)

  ovm_object temp;
  ble_ovc_config cfg;
 // ahb_reg_block ahb_rm;
    ble_ral_block ble_ral;

    uvm_reg_data_t r_data;
    uvm_status_e status;
 // string adv_data;
  bit [31:0] data; 
  function new(string name ="ahb_base_seq");
    super.new(name);
  endfunction
    
  task body();
    super.body();
       if(!(m_sequencer.get_config_object("config",temp,0)))
          `ovm_fatal("AHB_SEQ","can not get config in seq")
       if(!($cast(cfg,temp)))
          `ovm_fatal("AHB_SEQ","$casting failure")
          ble_ral = cfg.ble_ral;
   endtask

endclass


//***********************************************************************************

class ahb_simple_seq extends ahb_base_seq;
  `ovm_object_utils(ahb_simple_seq)

    extern function new(string name = "ahb_simple_seq");
    extern task body();

endclass

    function ahb_simple_seq::new(string name = "ahb_simple_seq");
      super.new(name);
    endfunction

    task ahb_simple_seq::body();
          int i = 1;
         req = ahb_seq_item::type_id::create("req");
      repeat(2)
				begin  
				req = ahb_seq_item::type_id::create("req");

			  start_item(req);
					         // req.haddr = 32'h0100_0000 ;
         // req.hwrite = 1;
         // req.hsize = 2;
         // req.hburst = 0;
         // req.hwdata[0] = $random;
        	//assert(req.randomize() with {hwrite == 1;haddr == 400;hburst == `SINGLE;hsize==2 ;});
  
				assert(req.randomize() with {hsize == 2;hburst == `INCR8;hwrite == 1 ; htrans == 'd2;});
          req.print();
          finish_item(req);
					i = i+1;
        end
    endtask
  
//*****************************************************************************


class adv_state_seq extends ahb_base_seq;
  `ovm_object_utils(adv_state_seq)
    
    function new(string name = "adv_state_seq");
      super.new(name);
    endfunction

    task body();
       
         string adv_data = "ABCD";
         bit [((8*31)-1) : 0 ] binary_data;
         req = ahb_seq_item::type_id::create("req");
       
        //start_item(req);
        
        assert(req.randomize() with {haddr == `status_register; hsize == 2;hburst == `SINGLE;});
         super.body();
        // ble_ral.ble_rm.state_reg_h.write(status,req.hwdata[0],UVM_FRONTDOOR,.parent(this));
       
       //assert(req.randomize() with {haddr == `adv_types;hsize == 2;hburst == `SINGLE ; types == `CONN_UNDIRECTED ; addr_type == `random ; fitler_policy == `OFF , length == 0});
        
             data = {20'b0,6'b0,`OFF,`random,`CONN_UNDIRECTED};
      
      //assert(ble_ral.ble_rm.randomize() with {ADV_types_h.addr_type.value == 1; ADV_types_h.random_type.value == 2'b11;});
         
    
    //   ahb_rm.ADV_types_h.update(status,.path(UVM_FRONTDOOR),.parent(this));
  

       //  assert(req.randomize() with {addr_type == `random; random_type == 2'b00;});
      //adv_source device addr
      //   ahb_rm.adv_source_device_addr_lsb_h.update(status,.path(UVM_FRONTDOOR),.parent(this));
        //  ahb_rm.adv_source_device_addr_lsb_h.write(status,req.device_addr[31:0],UVM_FRONTDOOR,.parent(this));
       //  ahb_rm.adv_source_device_addr_msb_h.update(status,.path(UVM_FRONTDOOR),.parent(this));
        // ahb_rm.adv_source_device_addr_msb_h.write(status,{16'b0,req.device_addr[47:32]},UVM_FRONTDOOR,.parent(this));
    
      //  finish_item(req);
      ble_ral.ble_rm.print();
    endtask

endclass





//*************************************************************************************************************
//
//
class adv_seq extends ahb_base_seq;
  `ovm_object_utils(adv_seq)

  seq_item tx;
  function new(string name = "adv_seq");
      super.new(name);
    endfunction
 
  task body();
    tx = seq_item::type_id::create("tx");
    tx.randomize() with { pkt_type == ADV ; length == MEDIUM;};
  

    super.body();
     
   // ble_ral.ble_rm.print();
    //if(ble_ral.ble_rm == null)
    ble_ral.ble_rm.state_register_h.write(status,{28'h0,2'h1,2'h2},UVM_FRONTDOOR,.parent(this));
    ble_ral.ble_rm.adv_params_h.write(status,{20'b0,tx.adv_pdu_length,1'b0,tx.adv_pdu_txadd,tx.adv_pdu},UVM_FRONTDOOR,.parent(this));
    ble_ral.ble_rm.adv_source_device_addr_lsb_h.write(status,tx.source_device_addr[31:0],UVM_FRONTDOOR,.parent(this));
    ble_ral.ble_rm.adv_source_device_addr_msb_h.write(status,{16'b0,tx.source_device_addr[47:32]},UVM_FRONTDOOR,.parent(this));
    //adv_rm.adv_interval_reg.write(status,tx.)
   


//  ble_ral.ble_rm.state_register_h.set('h2);
//  ble_ral.ble_rm.ADV_types_h.set({tx.adv_pdu_length,1'b0,tx.adv_pdu_txadd,tx.adv_type});
//
//    r_data = ble_ral.ble_rm.ADV_types_h.get();
//  ble_ral.ble_rm.adv_source_device_addr_lsb_h.set(tx.source_device_addr[31:0]);
//  ble_ral.ble_rm.adv_source_device_addr_msb_h.set({16'b0,tx.source_device_addr[47:32]});
 // adv_rm.adv_interval_reg.write(status,tx.)

 

  endtask

endclass

