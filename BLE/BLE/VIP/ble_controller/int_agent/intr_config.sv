

class intr_config extends ovm_object;

  `ovm_object_utils(intr_config)                           // Factory registration of interrupt configuration object.

  virtual intr_if int_if;                                  // Instance of virtual interrupt interface
  virtual pp_intf pkt_if;                                  // ppi interface
 
 // ovm_event_pool global_event_pool = ovm_event_pool::get_global_pool();
 
  ble_ral_block ble_ral;
  
	// Interrupt flags
  bit pkt_received;
  bit conn_event_est;         
  bit conn_event_est_failed;  
  bit conn_event_close;       
  bit conn_event_kill_by_host;
  bit duplicate_pkt_received; 
  bit bad_pkt_received;       
  bit kill_conn_after_ack;   
  bit encryption_start;   
  bit encryption_pause;      
  bit pkt_dropped; 	

  bit supervision_TO;          
  bit procedure_rsp_TO;         
  bit authentication_payload_TO;
	
	bit deep_sleep;
	bit sleep;
	
  extern function new(string name = "intr_config");        // OVM constructor

endclass

  function intr_config::new(string name = "intr_config");
   super.new(name);
  endfunction

            
