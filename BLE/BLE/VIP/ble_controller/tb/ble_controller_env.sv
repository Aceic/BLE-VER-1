class ble_controller_env extends ovm_env;

   `ovm_component_utils(ble_controller_env)

   // Main Config class handle declaration

   // TX agent config class handle
     tx_agent tx_agent_h;

   // RX agent config class handle
     rx_agent rx_agent_h;
   // INT agent config class handle
     intr_agent intr_agent_h;

   // TX agent handle
     pkt_seq_item seq_item, data_seq_item, rsp;
   
   controller_config ctrl_cfg;

   // RX agent handle
   // INT agent handle
   // register class handle
   bit [31:0] state_reg;
   bit [31:0] command_reg;

   reg_file reg_h;

   // BLE scoreboard handle

   tlm_analysis_fifo #(pkt_seq_item) rx_mon_fifo;

   // list of global events 

   // fsm_states and fsm_sub_states

   enum {ADV,SCAN,INIT,CONN_MASTER,CONN_SLAVE,STAND_BY} main_state;
   typedef enum {ACTIVE,PASSIVE} sub_state;

   // seq_item

   // Standard UVM methods

   // BLE ENV temp variables

   bit [31:0]  adv_interval_min;
   bit [31:0]  adv_interval_max;
   bit [31:0]  adv_interval;// = ((adv_interval_min)*0.625);
   bit [31:0] adv_parameter_reg;
   bit scan_type;
   bit [15:0] scan_window;
   bit [15:0] scan_interval;
   int temp = 'd0;
   bit flag;

   extern function new(string name = "ble_controller_env", ovm_component parent);
   extern function void build();
   extern function void connect();
 
   extern task run();
   extern task drive_pkt(pkt_seq_item txn);
   extern task start_adv();
   extern task start_conn_undir_adv();
   extern task send_scan_rsp();
   extern task start_conn_dir_adv_ld();
   extern task start_scannable_adv();
   extern task start_non_conn_adv();
   extern task start_conn_dir_adv_hd();
   extern task start_scan();
   extern task start_active_scanning();
   extern task start_passive_scanning();
   extern task update_seq_item(ref pkt_seq_item data_seq_item);


endclass

function ble_controller_env::new(string name = "ble_controller_env", ovm_component parent);
   
   super.new(name,parent);

endfunction

function void ble_controller_env::build();
   ovm_object tmp;
     if(!get_config_object("controller_config",tmp))
      `ovm_fatal("BLE_CTRLR","can not get config object")
   if(!$cast(ctrl_cfg,tmp))
      `ovm_fatal("BLE_CTRLR","can not cast config object handle")

       // Build Tx agent, Rx agent, Int agent, and all their cfg classes and register class handle
   // Build Global events
   tx_agent_h = tx_agent::type_id::create("tx_agent_h",this);
   rx_agent_h = rx_agent::type_id::create("rx_agent_h",this);
   intr_agent_h = intr_agent::type_id::create("intr_agent_h",this);
   // Register class build

   reg_h = reg_file::type_id::create("reg_h");
    set_config_object("tx_agent_h*","tx_cfg",ctrl_cfg.tx_cfg,0);   
    set_config_object("rx_agent_h*","rx_cfg",ctrl_cfg.rx_cfg,0);
    set_config_object("intr_agent_h*","int_cfg",ctrl_cfg.int_cfg,0);

   rx_mon_fifo = new("rx_mon_fifo",this);

endfunction

function void ble_controller_env::connect();

   // Controller to Scoreboard connection
   //
   // RX Monitor analysis port connection to ble_controller

   rx_agent_h.rx_mon.rx_mon_port.connect(rx_mon_fifo.analysis_export);
   rx_agent_h.rx_mon.rx_intr_port.connect(intr_agent_h.intr_monh.pkt_fifo_rx.analysis_export); // RX monitor to interrupt agent
	 

endfunction

task ble_controller_env::run();
     
   
   wait(ctrl_cfg.ble_ral.ble_rm.command_reg_h.adv_start.value == 1 || ctrl_cfg.ble_ral.ble_rm.command_reg_h.scan_start.value == 1);
    if(ctrl_cfg.ble_ral.ble_rm.state_register_h.ADV.value == 1)
      begin
      ctrl_cfg.tx_cfg.vif.TX_FLAG = 1;
      end
     else if(ctrl_cfg.ble_ral.ble_rm.state_register_h.SCAN.value == 1 || ctrl_cfg.ble_ral.ble_rm.state_register_h.INIT.value == 1)
      begin
      $display("Rx flag is going low............... %d",$time);
      
      ctrl_cfg.tx_cfg.vif.TX_FLAG = 0;
      `ovm_info("BLE_ENV:body",$sformatf("TX flag value is %b",ctrl_cfg.tx_cfg.vif.TX_FLAG),OVM_LOW);
      end
    
   forever
      begin
        
        wait(ctrl_cfg.ble_ral.ble_rm.command_reg_h.start_read.value == 1);
        begin
        
         //  ctrl_cfg.ble_ral.reg_read('h0,state_reg);
            state_reg = ctrl_cfg.ble_ral.ble_rm.state_register_h.get();
            ctrl_cfg.ble_ral.reg_read('he4,command_reg);
            
            update_seq_item(data_seq_item);
            ctrl_cfg.ble_ral.ble_rm.command_reg_h.start_read.set(0);
            $display("start_read value is... %b",ctrl_cfg.ble_ral.ble_rm.command_reg_h.start_read.value);
          end
	
         begin

	      if(state_reg[1] == 1)
	         begin
	          main_state = ADV;
		  $display("Values of state and command registers are %b %b",state_reg,command_reg);
	          start_adv();
	         end
	        end

	 begin
	    
	    if(state_reg[2] == 1)
	         begin
	          main_state = SCAN;
		  $display("Value of state and command registers are %b %b",state_reg,command_reg);
	          start_scan();
	         end

	 end

	 begin
	 end

	 begin
	    if(state_reg[0] == 1)
	    begin
	       main_state = STAND_BY;
	    end
	 end

	 begin
	    if(main_state == STAND_BY)
               break;
	 end
      end 



endtask
//**************************************************************************************************************
task ble_controller_env::drive_pkt(pkt_seq_item txn);

   
   ovm_test_done.raise_objection(this);
    tx_agent_h.m_sequencer.execute_item(txn);
   
   // To check timescale and the delay value
   #625us;
   ovm_test_done.drop_objection(this);

endtask

task ble_controller_env::start_adv();
   
   if(main_state == ADV && command_reg[0] == 1 && adv_parameter_reg[3:0]=='d0)  // Connectable Undirected ADV
      start_conn_undir_adv();
   else if(main_state == ADV && command_reg[0] == 1 && adv_parameter_reg[3:0] == 'd1) // Connectable directed low duty cycle ADV
      start_conn_dir_adv_ld();
   else if(main_state == ADV && command_reg[0] == 1 &&  adv_parameter_reg[3:0] == 'd6) // Scannable non-connectable ADV
      start_scannable_adv();
   else if(main_state == ADV && command_reg[0] == 1 &&  adv_parameter_reg[3:0] == 'd2) // Non-Connectable ADV
      start_non_conn_adv();
   else if(main_state == ADV && command_reg[0] == 1 &&  adv_parameter_reg[3:0] == 'd1) // Connectable directed high duty cycle ADV
      start_conn_dir_adv_hd();
   
   if(command_reg[1] == 1)	// Command: STOP ADV
      main_state = STAND_BY;

endtask


task ble_controller_env::start_conn_undir_adv();
  
   ovm_test_done.raise_objection(this);
   `ovm_info("BLE_ENV:body","Started Connectable Undirected ADV",OVM_LOW)
   data_seq_item.print();
   drive_pkt(data_seq_item);
   `ovm_info("BLE_ENV:body","Completed Connectable Undirected ADV",OVM_LOW)
   //****************************************************************
   fork
      begin
         
	 wait(flag);  // Need to wait here for signal from Rx_Agent, as of now, analysis port is not used here. will be done.

      end

      begin
         
         #(adv_interval);//*1000000000);   // Made delay very less to see progression

      end

   join_any

   disable fork;
   
   
   ovm_test_done.drop_objection(this);
   if(temp == 0)
     flag = 'b1;    // If flag is high, need to initiate scan_rsp packet or have to wait for connect_req packet
  
  if(flag)
   begin
      `ovm_info("BLE_ENV:body","Need to send either SCAN_RSP packet or wait for CONNECT_REQ packet",OVM_LOW)
      send_scan_rsp();
      command_reg[1] = 'b1;
      main_state = STAND_BY;
      temp++;
   end
   else
      temp++;

  
  if(temp == 4)   // Doing the task of RAL, stopping ADV
  			
      begin
         command_reg[1] = 'b1;
         `ovm_info("BLE_ENV:body",$sformatf("temp value is %d ",temp),OVM_LOW)
      end
  else 
     begin
	 start_adv();
     end


   


   //****************************************************************

endtask

task ble_controller_env::send_scan_rsp();

   `ovm_info("BLE_ENV:body","Started sending scan response",OVM_LOW)
 
    // All the parameters for SCAN_RSP are already configured while configuring ADV parameters, for sending scan_rsp pkt,  adv_type of seq_item need     to be driven appropriately, do_pack funtion takes care of forming the packet.
 
   seq_item = data_seq_item;
   seq_item.frm_adv_pdu_type   =   4'b0100;
   $cast(seq_item.adv_type,'d4);
   seq_item.print();
   drive_pkt(seq_item);
   
   `ovm_info("BLE_ENV:body","Completed sending scan response",OVM_LOW)


endtask

task ble_controller_env::start_conn_dir_adv_ld();

   `ovm_info("BLE_ENV:body","Started Connectable directed ADV LD",OVM_LOW)
   data_seq_item.print();
   drive_pkt(data_seq_item);
   `ovm_info("BLE_ENV:body","Completed Connectable directed ADV LD",OVM_LOW)
   main_state = STAND_BY;

endtask

task ble_controller_env::start_scannable_adv();


   `ovm_info("BLE_ENV:body","Started Scannable nonConnectable ADV ",OVM_LOW)
   data_seq_item.print();
   drive_pkt(data_seq_item);
   `ovm_info("BLE_ENV:body","Completed Scannable nonConnectable ADV ",OVM_LOW)
   main_state = STAND_BY;


endtask

task ble_controller_env::start_non_conn_adv();

   ovm_test_done.raise_objection(this);
   `ovm_info("BLE_ENV:body","Started nonConnectable ADV ",OVM_LOW)
   data_seq_item.print();
   drive_pkt(data_seq_item);
   temp++;
   `ovm_info("BLE_ENV:body","Completed nonConnectable ADV ",OVM_LOW)
   
   `ovm_info("BLE_ENV:body",$sformatf("Time before delay %d ",$time),OVM_LOW)

   #(adv_interval*1000000000);
   `ovm_info("BLE_ENV:body",$sformatf("Time after delay %d ",$time),OVM_LOW)
  // ovm_test_done.drop_objection(this);

   
   if(temp == 4)			// Doing the task of RAL, stopping ADV
   begin
      command_reg[1] = 'b1;
      `ovm_info("BLE_ENV:body",$sformatf("temp value is %d ",temp),OVM_LOW)
      
   end
   else
      start_adv();


endtask

task ble_controller_env::start_conn_dir_adv_hd();

   `ovm_info("BLE_ENV:body","Started Connectable directed ADV HD",OVM_LOW)
   data_seq_item.print();
   drive_pkt(data_seq_item);
   `ovm_info("BLE_ENV:body","Completed Connectable directed ADV HD",OVM_LOW)
   main_state = STAND_BY;

endtask

task ble_controller_env::update_seq_item(ref pkt_seq_item data_seq_item);
   
// Temporary intermediate registers to get the data from registers and are used to copy values to seq_item

   bit [31:0] temp_reg;
   bit [7:0] adv_data[$];
   data_seq_item = pkt_seq_item::type_id::create("data_seq_item");

// updating ADV parameters from registers to seq_item

   ctrl_cfg.ble_ral.reg_read('h4,adv_parameter_reg);  // Get ADV parameters 
 
   if(state_reg[1] == 'b1 || state_reg[2] == 'b1 || state_reg[3] == 'b1)
      $cast(data_seq_item.pkt_type,'b0);				// Setting pkt type ( ADV/DATA)
   
   data_seq_item.frm_adv_pdu_type   =   adv_parameter_reg[3:0]; 
   
   if(adv_parameter_reg != 'b0000)
   begin
      case(adv_parameter_reg[3:0])					// Setting ADV Type(ADV_IND/ADV_NON...)
         4'b0000: $cast(data_seq_item.adv_type,'d0);
         4'b0001: $cast(data_seq_item.adv_type,'d1);
         4'b0010: $cast(data_seq_item.adv_type,'d2);
         4'b0110: $cast(data_seq_item.adv_type,'d6);
      endcase
   end


   data_seq_item.duty_cycle_type   =   adv_parameter_reg[4];
   data_seq_item.frm_adv_pdu_tx_add   =   adv_parameter_reg[5];
   data_seq_item.frm_adv_pdu_rx_add   =   adv_parameter_reg[6];
   data_seq_item.adv_filter_policy   =   adv_parameter_reg[8:7];
   data_seq_item.adv_channel_map   =   adv_parameter_reg[11:9];
   data_seq_item.frm_adv_pdu_length   =   adv_parameter_reg[17:12];
 
if(ctrl_cfg.ble_ral.ble_rm.state_register_h.ADV.value == 1)
  begin
   data_seq_item.frm_adv_data = new[data_seq_item.frm_adv_pdu_length-6];

   ctrl_cfg.ble_ral.mem_read('h200,(adv_parameter_reg[17:12])-6,adv_data);   // Get ADV Data
   foreach(adv_data[i])
      begin
      
         data_seq_item.frm_adv_data[i] = adv_data[i];
      end
  end
  // ADV Interval min and max update

  
   ctrl_cfg.ble_ral.reg_read('h14,adv_interval_min);  // Get ADV Interval parameters 
   ctrl_cfg.ble_ral.reg_read('h18,adv_interval_max);  

   adv_interval = ((adv_interval_min)*0.625);


// Device address of source and target for different ADV types update

   
   ctrl_cfg.ble_ral.reg_read('h8,temp_reg);  
   data_seq_item.frm_adv_addr[31:0] = temp_reg;
   ctrl_cfg.ble_ral.reg_read('hc,temp_reg);  
   data_seq_item.frm_adv_addr[47:32] = temp_reg[15:0];

   
   ctrl_cfg.ble_ral.reg_read('h1c,temp_reg);  
   data_seq_item.frm_conn_init_add[31:0] = temp_reg;
   ctrl_cfg.ble_ral.reg_read('h20,temp_reg);  
   data_seq_item.frm_conn_init_add[47:32] = temp_reg[15:0];

   // SCAN Parameters update  
   	//scan type

   ctrl_cfg.ble_ral.reg_read('h28,temp_reg);  
   data_seq_item.scan_type = temp_reg[0];
   scan_type = temp_reg[0];
   
   // Other parameters are not considered for time being
	// scan window and scan interval
   
   ctrl_cfg.ble_ral.reg_read('h44,temp_reg);  
   scan_window = temp_reg[15:0];
   
   ctrl_cfg.ble_ral.reg_read('h40,temp_reg);  
   scan_interval = temp_reg[15:0];

   	// device address for scan
  
   ctrl_cfg.ble_ral.reg_read('h2c,temp_reg);  
   data_seq_item.frm_scan_addr[31:0] = temp_reg;
   ctrl_cfg.ble_ral.reg_read('h30,temp_reg);  
   data_seq_item.frm_scan_addr[47:32] = temp_reg[15:0];



endtask


task ble_controller_env::start_scan();

   `ovm_info("BLE_ENV:body","Entered into SCAN state",OVM_LOW);
   $display("Scan type is %b",scan_type);
   if(main_state == SCAN && command_reg[2] == 1 && scan_type =='b0)  // Active Scanning
      start_active_scanning();
   else if(main_state == SCAN && command_reg[2] == 1 && scan_type == 'b1) // Passive Scanning
      start_passive_scanning();
   
   if(command_reg[3] == 1)	// Command: STOP SCAN
      main_state = STAND_BY;
endtask

task ble_controller_env::start_active_scanning();

   `ovm_info("BLE_ENV:body","Started SCANNING ",OVM_LOW)
  
   // Wait for pkt from RX_Agent
   // Analysis port declaration and usage

   
   fork

      begin
        
	// Wait for Pkt from RX Agent
	// Analysis port usage
	// check for type of adv and if ADV_IND, drive SCAN_REQ packet
	rx_mon_fifo.get(rsp);
	`ovm_info("BLE_ENV:body","Received packet from Rx Monitor",OVM_LOW);
	$display("type of packet received from rx is %b",rsp.frm_adv_pdu_type);

        ovm_test_done.raise_objection(this);
        seq_item = data_seq_item;
        seq_item.frm_adv_pdu_type   =   4'b0011;
        $cast(seq_item.adv_type,'d3);
				$display("*************************************************************");
        seq_item.print();
        drive_pkt(seq_item);
        `ovm_info("BLE_ENV:body","Completed sending scan_req packet ",OVM_LOW)
        ovm_test_done.drop_objection(this);    // Objection is used to return the control to here after driving the packet.

	// Wait for scan_rsp packet, and if received do nothing and wait for next adv packet
        temp++;

	if(temp == 'd3)
	main_state = STAND_BY;
	else
	start_scan();


      end

      begin

         #(scan_window * 0.625 * 1000000000);   // Wait for scan Window

      end

   join_any
   `ovm_info("BLE_ENV:body","came out of loop",OVM_LOW);
   disable fork;
   //#((scan_window-scan_interval) * 0.625 * 1000000000);   // Wait for scan_window-scan_interval before continuing with next cycle of scanning

endtask


task ble_controller_env::start_passive_scanning();

   `ovm_info("BLE_ENV:body","Entered passive scanning, exiting",OVM_LOW);
   fork

      begin
        
	// Wait for Pkt from RX Agent
	// Analysis port usage
	// check for type of adv and if ADV_IND, drive SCAN_REQ packet
	rx_mon_fifo.get(rsp);
	`ovm_info("BLE_ENV:body","Received packet from Rx Monitor",OVM_LOW);
	$display("type of packet received from rx is %b",rsp.frm_adv_pdu_type);
        ovm_test_done.raise_objection(this);
        seq_item = data_seq_item;
        seq_item.frm_adv_pdu_type   =   4'b0011;
        $cast(seq_item.adv_type,'d3);
        seq_item.print();
        drive_pkt(seq_item);
        `ovm_info("BLE_ENV:body","Completed sending scan_req packet ",OVM_LOW)
        ovm_test_done.drop_objection(this);    // Objection is used to return the control to here after driving the packet.

	// Wait for scan_rsp packet, and if received do nothing and wait for next adv packet

        temp++;
        if(temp == 'd3)
        main_state = STAND_BY;
        else
        start_scan();


      end

      begin

         #(scan_window * 0.625 * 1000);//000000);   // Wait for scan Window

      end

   join_any

   disable fork;
  // #((scan_window-scan_interval) * 0.625 * 100000);//0000);   // Wait for scan_window-scan_interval before continuing with next cycle of scanning

   


endtask
