


class intr_monitor extends ovm_monitor;

  `ovm_component_utils(intr_monitor)                                    // Factory registration of interrupt monitor

  parameter ADV_IND         = 4'd0,                                     // Connectable undirected advertising PDU
            ADV_DIRECT_IND  = 4'd1,                                     // Connectable directed advertising PDU
	          ADV_NONCONN_IND = 4'd2,                                     // non connectable undirected advertising PDU
	          ADV_SCAN_IND    = 4'd6,                                     // Scannable undirected advertising PDU
	          SCAN_REQ        = 4'd3,                                     // Scan request PDU
	          SCAN_RSP        = 4'd4,                                     // Scan response PDU
	          CONNECT_REQ     = 4'd5;                                     // Connect request PDU
 
 // env_config env_cfg;                                                   // Instance of environment configuration class 
    intr_config int_cfg;


   ble_ral_block  ral_block;                                         // Pointer for RAL block to update ISR register as per interrupt generated

  tlm_analysis_fifo #(bt_intr_seq_item) pkt_fifo_tx;                        // For collecting packet from BLE controller during transmission
  tlm_analysis_fifo #(bt_intr_seq_item) pkt_fifo_rx;                        // For collecting packet from BLE controller during reception
  ovm_analysis_port #(intr_seq_item) intr_ap;                           // To send interrupt to the scoreboard. 

  intr_sequencer intr_seqrh;                                            // Instance of Interrupt sequencer

  bt_intr_seq_item  pkt_trans_tx;                                              // Instance of BLE packet sequence item.
  bt_intr_seq_item  pkt_trans_rx;                                              // Instance of BLE packet sequence item.
  intr_seq_item intr_trans;                                             // Instance of interrupt sequence item

  virtual pp_intf vif_pkt;                                              // Virtual instance of Packet processing interface.
  virtual intr_if vif_intr;                                             // Virtual instance of Interrupt interface.

  extern function new(string name = "intr_monitor", ovm_component parent = null);  

  extern function void build();
  extern function void connect();
  extern task run();

  extern task ble_tx_rx();                                              // Task to switch between BLE TX and BLE RX mode
  extern task timeout_interrupts();                                     // Task for generating timeout interrupts
  extern task low_power_interrupts();                                   // Task for generating low power interrupts
  extern task miscellaneous_data_channel_interrupts();                  // 
  extern task soft_reset();                                             // Task for monitoring soft reset from host

  extern task ble_tx();                                                 // Task for generating interrupt during BLE TX duration 
  extern task ble_rx();                                                 // Task for generating interrupt during BLE RX duration

  extern task advertising_channel(bt_intr_seq_item pkt_trans);                                    // Task for generating interrupt for advertising PDU
  extern task data_channel(bt_intr_seq_item pkt_trans);                                           // Task for generating interrupt for data PDU

  extern task length_timer(logic [5:0] length);                                       // Task for implementing advertising length timer.

  
  extern task monitor_int_if();                                         // Task to monitor interrupt interface.

endclass: intr_monitor

  function intr_monitor:: new(string name = "intr_monitor", ovm_component parent = null);
   super.new(name, parent);
   pkt_fifo_tx = new("pkt_fifo_tx",this);
   pkt_fifo_rx = new("pkt_fifo_rx",this);
   intr_ap = new("ovm_analysis_port",this);
  endfunction


  function void intr_monitor:: build();
   ovm_object temp1, temp2;
   `ovm_info(get_name(),"BUILD PHASE",OVM_LOW)

   if(!get_config_object("int_cfg",temp1,0))
    `ovm_fatal("INTR_MONITOR","Cannot get config handle..")
 
   if(!$cast(int_cfg,temp1))
    `ovm_fatal("INTR_MONITOR","Casting failed")

   if(!get_config_object("sequencer",temp2,0))
    `ovm_fatal("INTR_MONITOR","Cannot get config handle..")
 
   if(!$cast(intr_seqrh,temp2))
    `ovm_fatal("INTR_MONITOR","Casting failed")

   ral_block = int_cfg.ble_ral;
  endfunction: build

  function void intr_monitor:: connect();

   `ovm_info(get_name(),"CONNECT PHASE",OVM_LOW)

   vif_pkt  = int_cfg.pkt_if;
   vif_intr = int_cfg.int_if;
 
  endfunction: connect

  task intr_monitor:: run();
   `ovm_info(get_name(),"RUN PHASE",OVM_LOW)

   forever
    begin
     fork: MONITOR_RUN
     
     //begin
       //soft_reset();                                          // Task to monitor soft reset
      //end

      begin
       ble_tx_rx();                                           // Task to switch between BLE TX and BLE RX mode 
      end

      begin
       monitor_int_if();                                      // Monitoring interrupt interface to send interrupt sequence item to interrupt scoreboard
      end

      begin
       timeout_interrupts();                                  // Task to generate timeout related interrupts
      end

      begin
       low_power_interrupts();                                // Task to generate Low Power related interrupts
      end

      begin
       miscellaneous_data_channel_interrupts();               // Task to generate other data channel interrupts.
      end

     join_any: MONITOR_RUN
     disable MONITOR_RUN;
     @(vif_pkt.clock);
   end  
  endtask: run


  task intr_monitor:: ble_tx_rx();

   fork: TX_RX
    ble_tx();
    ble_rx();
   join_any: TX_RX
   disable TX_RX;

  endtask

// Task for generating interrupt during the TX duration.

  task intr_monitor:: ble_tx();

   pkt_fifo_tx.get(pkt_trans_tx);

   repeat(56)
    @(posedge vif_pkt.clock);

   if(pkt_trans_tx.data == 0)
    begin
         
      advertising_channel(pkt_trans_tx);                      // Task for generating interrupt for advertising channel
    end
   else if(pkt_trans_tx.data == 1)
    begin
      
      data_channel(pkt_trans_tx);                             // Task for generating interrupt for data channel
    end
   
  endtask: ble_tx


// Task for generating interrupt during the RX duration.
  
  task intr_monitor:: ble_rx();
   
   
   intr_seq_item intr_adv_trans = intr_seq_item::type_id::create("intr_adv_trans");
   intr_adv_trans.int_o = 1;
   
   wait(int_cfg.pkt_received == 1'b1);
	 int_cfg.pkt_received = 1'b0;
   `ovm_info("INTR_MONITOR",$sformatf("packet received at %t",$time),OVM_LOW)
   fork
    ral_block.ble_rm.ISR_h.pkt_received.set(1);                                         // Update ISR for packet received interrupt
    intr_seqrh.execute_item(intr_adv_trans);
   join_none

   pkt_fifo_rx.get(pkt_trans_rx);
   if(pkt_trans_rx.data == 0)
    advertising_channel(pkt_trans_rx);                         // Task for generating interrupt for advertising channel
   else if(pkt_trans_rx.data == 1)
    data_channel(pkt_trans_rx);                                // Task for generating interrupt for data channel
   
  endtask

// Task for generating interrupt for advertising channel

  task intr_monitor:: advertising_channel(bt_intr_seq_item pkt_trans);
   //bit [3:0] pdu_type;
 
   intr_seq_item intr_adv_trans = intr_seq_item::type_id::create("intr_adv_trans");
   intr_adv_trans.int_o = 1;
   // PDU type check
   
    if(pkt_trans.adv_pdu_type == ADV_IND || pkt_trans.adv_pdu_type == ADV_DIRECT_IND || pkt_trans.adv_pdu_type == ADV_NONCONN_IND
      || pkt_trans.adv_pdu_type == ADV_SCAN_IND)
     begin
      fork
       begin
        ral_block.ble_rm.ISR_h.adv_pkt_start.set(1);                                            // Update ISR for advertise pdu start interrupt         
				
	      intr_seqrh.execute_item(intr_adv_trans); 
       end
       length_timer(pkt_trans.adv_length);
      join
      ral_block.ble_rm.ISR_h.adv_pkt_stop.set(1);                                             // Update ISR for advertise pdu stop interrupt       
	    intr_seqrh.execute_item(intr_adv_trans);
     end
    else if(pkt_trans.adv_pdu_type == SCAN_REQ)
     begin
      fork
       begin
        ral_block.ble_rm.ISR_h.scan_req_start.set(1);                                          // Update ISR for scan_req start interrupt                    
      	intr_seqrh.execute_item(intr_adv_trans); 
       end
       length_timer(pkt_trans.adv_length);
      join
        ral_block.ble_rm.ISR_h.scan_req_stop.set(1);                                            // Update ISR for scan_req_stop interrupt                     
        intr_seqrh.execute_item(intr_adv_trans);
     end
    else if(pkt_trans.adv_pdu_type == SCAN_RSP)
     begin
      fork
       begin
	     ral_block.ble_rm.ISR_h.scan_rsp_start.set(1);                                          // Update ISR for scan_rsp start interrupt               
	     intr_seqrh.execute_item(intr_adv_trans); 
       end
       length_timer(pkt_trans.adv_length);
      join
	    ral_block.ble_rm.ISR_h.scan_rsp_stop.set(1);                                          // Update ISR for scan_rsp_stop interrupt
      intr_seqrh.execute_item(intr_adv_trans);
     end
    else if(pkt_trans.adv_pdu_type == CONNECT_REQ)
     begin
      fork
       begin
	     ral_block.ble_rm.ISR_h.conn_req_pkt_start.set(1);                                      // Update ISR for conn_req_pkt_start interrupt                   
	     intr_seqrh.execute_item(intr_adv_trans);
       end
       length_timer(pkt_trans.adv_length);
      join 
      ral_block.ble_rm.ISR_h.conn_req_pkt_stop.set(1);                                        // Update ISR for conn_req_pkt_stop interrupt
	    ral_block.ble_rm.ISR_h.conn_event_create.set(1);                                        // Update ISR for connection create interrupt
      intr_seqrh.execute_item(intr_adv_trans);
      end
     else
      `ovm_error("INTR_MONITOR","\n Packet type not matching advertising channel PDU")
               
  endtask

// Task for generating interrupt for data channel

  task intr_monitor:: data_channel(bt_intr_seq_item pkt_trans);

   intr_seq_item intr_adv_trans = intr_seq_item::type_id::create("intr_adv_trans");
   intr_adv_trans.int_o = 1;

   // PDU type check (Data or control)
   
   fork
   if(pkt_trans.llid == 2'b01 || pkt_trans.llid == 2'b10)
    begin
     ral_block.ble_rm.ISR_h.data_pdu_start.set(1);                                              // Update ISR for data_pdu_start interrupt
     intr_seqrh.execute_item(intr_adv_trans);
    end
   else if(pkt_trans.llid == 2'b11)
    begin
     ral_block.ble_rm.ISR_h.ctrl_pdu_start.set(1);                                              // Update ISR for control pdu_start interrupt  	     
     intr_seqrh.execute_item(intr_adv_trans);
    end
   join_none

    
   fork
    if(pkt_trans.data_length == 5'd0 && pkt_trans.llid == 2'b01)
     begin
      ral_block.ble_rm.ISR_h.empty_pkt.set(1);                                                  // Update ISR for empty pdu recieved interrupt  	    
      intr_seqrh.execute_item(intr_adv_trans);
     end
    else if(pkt_trans.data_length == 5'd0 && pkt_trans.llid ==2'b10)
     `ovm_error("INTR_MONITOR","DATA PDU with LLID = 2'b10 cannot be empty PDU:: BLE Protocol Violation")
   join_none


   length_timer(pkt_trans.data_length);
  
 // fork
   if(pkt_trans.llid == 2'b01 || pkt_trans.llid == 2'b10)
    begin
     ral_block.ble_rm.ISR_h.data_pdu_stop.set(1);                                               // Update ISR for data_pdu_stop interrupt  	     
     intr_seqrh.execute_item(intr_adv_trans);
    end
   else if(pkt_trans.llid == 2'b11)
    begin
     ral_block.ble_rm.ISR_h.ctrl_pdu_stop.set(1);                                               // Update ISR for control pdu_stop interrupt  	     
     intr_seqrh.execute_item(intr_adv_trans);
    end
//  join
  endtask
  
// Task to implement the advertising PDU length timer

  task intr_monitor:: length_timer(logic [5:0] length);
      
   `ovm_info("INTR_MONITOR",$sformatf("\n Start  of length timer at time %0d",$time),OVM_LOW)
   repeat((length * 8) + 24)                               // Wait for the length of PDU
    @(posedge vif_pkt.clock);         
   
   `ovm_info("INTR_MONITOR",$sformatf("\n Stop  of length timer at time %0d",$time),OVM_LOW)
   
  endtask

// Task for generating timeout interrupts.

  task intr_monitor:: timeout_interrupts();
 
   intr_seq_item intr_adv_trans = intr_seq_item::type_id::create("intr_adv_trans");
   intr_adv_trans.int_o = 1;

   forever
    begin
     fork : TIMEOUT

      begin
       wait(int_cfg.supervision_TO == 1'b1);
			 int_cfg.supervision_TO = 1'b0;
       ral_block.ble_rm.ISR_h.conn_supervision_timeout.set(1);                              //  Update ISR for supervision timeout interrupt  	    
       intr_seqrh.execute_item(intr_adv_trans);
      end
      
      begin
       wait(int_cfg.procedure_rsp_TO == 1'b1);
       int_cfg.procedure_rsp_TO = 1'b0;
       ral_block.ble_rm.ISR_h.proc_rsp_timeout.set(1);                                      //  Update ISR for procedure response timeout interrupt  	    
       intr_seqrh.execute_item(intr_adv_trans);
      end
      
      begin
       wait(int_cfg.authentication_payload_TO == 1'b1);
			 int_cfg.authentication_payload_TO = 1'b0;
       ral_block.ble_rm.ISR_h.authenticate_timeout.set(1);                                  //  Update ISR for authentication timeout interrupt  	     
       intr_seqrh.execute_item(intr_adv_trans);
      end
     
     join_any : TIMEOUT
     disable TIMEOUT;
     @(vif_pkt.clock);
   end  
  endtask

// Task for generating low power interrupts.

  task intr_monitor:: low_power_interrupts();

   intr_seq_item intr_adv_trans = intr_seq_item::type_id::create("intr_adv_trans");
   intr_adv_trans.int_o = 1;

   forever
    begin
     fork : LP

      begin
       wait(int_cfg.sleep == 1'b1);
			 int_cfg.sleep = 1'b0;
       ral_block.ble_rm.ISR_h.sleep.set(1);                                                 //  Update ISR for sleep state interrupt  	    
       intr_seqrh.execute_item(intr_adv_trans);
      end
      
      begin
       wait(int_cfg.deep_sleep == 1'b1);
			 int_cfg.deep_sleep = 1'b0;
       ral_block.ble_rm.ISR_h.deep_sleep.set(1);                                            //  Update ISR for deep sleep state interrupt  	    
       intr_seqrh.execute_item(intr_adv_trans);
      end
          
     join_any : LP
     disable LP;
     @(vif_pkt.clock);
   end


  endtask

  task intr_monitor:: miscellaneous_data_channel_interrupts();


   intr_seq_item intr_adv_trans = intr_seq_item::type_id::create("intr_adv_trans");
   intr_adv_trans.int_o = 1;

   forever
    begin
     fork : DC

      begin
       wait(int_cfg.pkt_dropped == 1'b1);
			 int_cfg.pkt_dropped = 1'b0;
			 `ovm_info("INTR_MONITOR","Pkt dropped interrupt",OVM_LOW) 
       ral_block.ble_rm.ISR_h.pkt_drop.set(1);                                                   // Update ISR for packet dropped interrupt  	    
       intr_seqrh.execute_item(intr_adv_trans);
      end
       
			begin
       wait(int_cfg.conn_event_est == 1'b1);
			 int_cfg.conn_event_est = 1'b0;
       ral_block.ble_rm.ISR_h.conn_event_estb.set(1);                                            // Update ISR for connection event established interrupt  	    
       intr_seqrh.execute_item(intr_adv_trans);
      end
      
      begin
       wait(int_cfg.conn_event_est_failed == 1'b1);
			 int_cfg.conn_event_est_failed = 1'b0;
       ral_block.ble_rm.ISR_h.conn_estb_failure.set(1);                                      // Update ISR for connection establishment failure interrupt  
       intr_seqrh.execute_item(intr_adv_trans);
      end
      
      begin
       wait(int_cfg.conn_event_close == 1'b1);
			 int_cfg.conn_event_close = 1'b0;
       ral_block.ble_rm.ISR_h.conn_event_close.set(1);                                       // Update ISR for connection event closed interrupt  	    
       intr_seqrh.execute_item(intr_adv_trans);
      end
      
      begin
       wait(int_cfg.conn_event_kill_by_host == 1'b1);
			 int_cfg.conn_event_kill_by_host = 1'b0;
       ral_block.ble_rm.ISR_h.kill_conn_by_host.set(1);                                    // Update ISR for connection lost interrupt due to kill command by host	
       intr_seqrh.execute_item(intr_adv_trans);
      end

      begin
       wait(int_cfg.duplicate_pkt_received == 1'b1);
       int_cfg.duplicate_pkt_received = 1'b0;
			 ral_block.ble_rm.ISR_h.duplicate_pkt.set(1);                                          // Update ISR for duplicate packet received interrupt  	    
       intr_seqrh.execute_item(intr_adv_trans);
      end
      
      begin
       wait(int_cfg.bad_pkt_received == 1'b1);
       int_cfg.bad_pkt_received = 1'b0;
			 ral_block.ble_rm.ISR_h.bad_pkt.set(1);                                                // Update ISR for bad packet received interrupt  	    
       intr_seqrh.execute_item(intr_adv_trans);
      end
      
      begin
       wait(int_cfg.kill_conn_after_ack == 1'b1);
       int_cfg.kill_conn_after_ack = 1'b0;
			 ral_block.ble_rm.ISR_h.kill_conn_aftr_ack.set(1);                                     // Update ISR for connection lost interrupt due to MD bit as 0  	    
       intr_seqrh.execute_item(intr_adv_trans);
      end
      
      begin
       wait(int_cfg.encryption_start == 1'b1);
       int_cfg.encryption_start = 1'b0;
			 ral_block.ble_rm.ISR_h.encryption_start.set(1);                                       // Update ISR for encryption start interrupt  	    
       intr_seqrh.execute_item(intr_adv_trans);
      end

      begin
       wait(int_cfg.encryption_pause == 1'b1);
       int_cfg.encryption_pause = 1'b0;
			 ral_block.ble_rm.ISR_h.encryption_pause.set(1);                                       // Update ISR for encryption pause interrupt  	    
       intr_seqrh.execute_item(intr_adv_trans);
      end

     join_any : DC
     disable DC;
     @(vif_pkt.clock);
    end
  endtask

// Task for monitoring the soft reset in RAL.

  task intr_monitor:: soft_reset();
    
   forever
    begin
     //wait(ral_block.ble_rm.command_reg_h.soft_reset.value);
     //break();
    end

  endtask

// Task for monitoring the interrupt interface and send Interrupt seq_item to scoreboard
  task intr_monitor:: monitor_int_if();
   
   forever
    begin
     intr_trans = intr_seq_item::type_id::create("intr_trans");
     wait(vif_intr.int_o == 1'b1);
    
     intr_trans.int_o = 1;
     `ovm_info("INTR_MONITOR",$sformatf("\n Monitored interrupt on Interrupt interface at time %0d",$time),OVM_LOW) 
     intr_ap.write(intr_trans);
     wait(vif_intr.int_o == 1'b0);
    end

  endtask



