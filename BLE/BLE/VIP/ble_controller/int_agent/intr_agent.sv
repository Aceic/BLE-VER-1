


class intr_agent extends ovm_agent;

  `ovm_component_utils(intr_agent)                                                   // Factory registration of interrupt agent

  intr_driver     intr_drvh;                                                         // Instance of interrupt driver
  intr_monitor    intr_monh;                                                         // Instance of interrupt monitor
  intr_sequencer  intr_seqrh;                                                        // Instance of interrupt sequencer

  intr_config      int_cfg;                                                          // Instance of environment config object
 
  ovm_object temp;
  extern function new(string name = "intr_agent", ovm_component parent = null);
  extern function void build();
  extern function void connect();

endclass: intr_agent


  function intr_agent:: new(string name = "intr_agent", ovm_component parent = null);
   super.new(name, parent);
  endfunction 


  function void intr_agent:: build();
   `ovm_info(get_name(),"BUILD PHASE",OVM_LOW)
   
  
    if(!get_config_object("int_cfg",temp,0))
      `ovm_fatal("INTR_AGENT","can not get config object...!!!") 
    if(!$cast(int_cfg,temp))
      `ovm_fatal("INTR_AGENT","casting failed...!!!") 
      
    
     intr_drvh = intr_driver::type_id::create("intr_drvh",this);
     intr_seqrh = intr_sequencer::type_id::create("intr_seqrh",this);
     intr_monh = intr_monitor::type_id::create("intr_monh",this);
   set_config_object("*","sequencer",intr_seqrh,0);                               // Setting interrupt agent's sequencer handle in config space to
                                                                                  // execute seq_item in interrupt monitor
   
  endfunction: build

  function void intr_agent:: connect();
   
   `ovm_info(get_name(),"CONNECT PHASE",OVM_LOW)
    intr_drvh.seq_item_port.connect(intr_seqrh.seq_item_export);                  // connection between driver and sequencer for routing seq_item 

  endfunction:connect



