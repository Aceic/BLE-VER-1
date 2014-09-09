


class tx_monitor extends ovm_monitor;

  `ovm_component_utils(tx_monitor)                                    // Factory registration of interrupt monitor
 
  tx_agent_cfg tx_cfg;                                                   // Instance of environment configuration class 
 
  bit frame[];                                        

	parameter PREAMBLE_WIDTH       = 'd8,
	          ACCESS_ADDRESS_WIDTH = 'd32,
						HEADER_WIDTH         = 'd16,
						CRC_WIDTH            = 'd24,
						BYTE                 = 'd8;


  ovm_analysis_port #(pkt_seq_item) ap_tx;                           // For collecting packet from BLE controller during transmission

  pkt_seq_item  pkt_trans;                                              // Instance of BLE packet sequence item.

  virtual pp_intf vif_pkt;                                              // Virtual instance of Packet processing interface.
  extern function new(string name = "tx_monitor", ovm_component parent = null);  

  extern function void build();
  extern function void connect();
  extern task run();

  extern task ble_tx_rx();                                              // Task to switch between BLE TX and BLE RX mode
  extern task soft_reset();                                             // Task for monitoring soft reset from host

  extern task ble_tx();                                                 // Task for generating interrupt during BLE TX duration 

  extern task preamble_AA_check(output logic [7:0] preamble, logic [31:0] acc_addr);                     // Task for Preamble and Access address detection
  extern function bit access_addr_check(logic [7:0] preamble, logic [31:0] access_addr); // Function to perform constraint check on data channel access address

endclass: tx_monitor

  function tx_monitor:: new(string name = "tx_monitor", ovm_component parent = null);
   super.new(name, parent);
   ap_tx = new("ovm_analysis_port",this);
  endfunction


  function void tx_monitor:: build();
   ovm_object temp;
   `ovm_info(get_name(),"BUILD PHASE",OVM_LOW)

   if(!get_config_object("tx_cfg",temp,0))
    `ovm_fatal("TX_MONITOR","Cannot get config handle..")
 
   if(!$cast(tx_cfg,temp))
    `ovm_fatal("TX_MONITOR","Casting failed")

  endfunction: build

  function void tx_monitor:: connect();

   `ovm_info(get_name(),"CONNECT PHASE",OVM_LOW)

   vif_pkt  = tx_cfg.vif;
 
  endfunction: connect

  task tx_monitor:: run();
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

      
     join_any: MONITOR_RUN
     disable MONITOR_RUN;
     @(vif_pkt.clock);
   end
  endtask: run


  task tx_monitor:: ble_tx_rx();

   forever
    begin
    
      begin
       wait(vif_pkt.BLE_TX == 1);
       ble_tx();                                                                    // Generating interrupt during the TX duration.
       wait(vif_pkt.BLE_TX == 0);
      end
    
    end

  endtask

// Task for generating interrupt during the TX duration.

  task tx_monitor:: ble_tx();
   logic [7:0] preamble;
	 logic [31:0] acc_addr;
   logic [0:15] header;
   logic [0:3] pdu_type;
	 logic tx_add;
	 logic rx_add;
	 logic [0:5] adv_length;

	 logic [0:1] llid;
	 logic sn, nesn, md;
   logic [0:4] data_length;
   
   
	 pkt_trans = pkt_seq_item::type_id::create("pkt_trans");

   fork: TX
    begin
     wait(vif_pkt.BLE_TX == 0);
     `ovm_info("TX_MONITOR","\n NO packet detected within the BLE TX duration",OVM_LOW)
    end

    begin
     preamble_AA_check(preamble, acc_addr);                              // Task to perform preamble and access address check

       for(int j=0;j<HEADER_WIDTH;j++)
       begin
        @(posedge vif_pkt.clock);	  
        header[j] = vif_pkt.PPI; 
       end  

			       
			 if(preamble == 8'b10101010 && acc_addr == 32'h8E89BED6)
			 begin
			  `ovm_info("TX_MONITOR",$sformatf("\nAccess address for ADV channel is %h preamble is %b",acc_addr,preamble),OVM_LOW)

			  adv_length = header[(HEADER_WIDTH-8):(HEADER_WIDTH-3)];
				adv_length = {<< {adv_length}};

        //$display(" length is %d **************************************",adv_length);
				frame = new[((PREAMBLE_WIDTH+ACCESS_ADDRESS_WIDTH+HEADER_WIDTH+CRC_WIDTH) + (adv_length * 8))];

			  for(int i=0;i<PREAMBLE_WIDTH;i++)
			  begin
         frame[i] = preamble[i];
			  end
        //$display("preamble is %b**************************************",preamble);
        for(int i=0;i<ACCESS_ADDRESS_WIDTH;i++)
			  begin
         frame[i+PREAMBLE_WIDTH] = acc_addr[i];
			  end
       
       // $display("acc addr is %h **************************************",acc_addr);
        for(int i=0;i<HEADER_WIDTH;i++)
			  begin
         frame[i+PREAMBLE_WIDTH+ACCESS_ADDRESS_WIDTH] = header[i];
			  end

			 // $display("frame[0:7] is %p and preamble is %b",frame[0:7],preamble);
			 // $display("frame[8:39] is %p and acc_addr is %h",frame[8:39],acc_addr);
				
			  for(int i=0;i<((adv_length * BYTE) + CRC_WIDTH);i++)
			  begin
			   @(posedge vif_pkt.clock);
         frame[i+PREAMBLE_WIDTH+ACCESS_ADDRESS_WIDTH+HEADER_WIDTH] = vif_pkt.PPI;  
			  end

			//	$display("header is %p",frame[40:55]);
			//  $display("SA is %p",frame[56:103]);
			//  $display("TA is %p",frame[104:151]);
			//  $display("CRC is %p",frame[152:175]);
			 end
       else
			 begin
			  `ovm_info("TX_MONITOR",$sformatf("\nAccess address for data channel is %h preamble is %b",acc_addr,preamble),OVM_LOW)
        data_length = header[(HEADER_WIDTH-8):(HEADER_WIDTH-4)];
				data_length = {<< {data_length}};

        //$display("acc addr is %h **************************************",acc_addr);
        //$display("preamble is %b**************************************",preamble);
        //$display(" length is %d **************************************",data_length);

				frame = new[(PREAMBLE_WIDTH+ACCESS_ADDRESS_WIDTH+HEADER_WIDTH+CRC_WIDTH) + (data_length * 8)];

			  for(int i=0;i<PREAMBLE_WIDTH;i++)
			  begin
         frame[i] = preamble[i];
			  end
       
			 // $display("frame[0:7] is %p and preamble is %b",frame[0:7],preamble);
			 // $display("frame[0:7] is %p and preamble is %b",frame[0:7],preamble);
        for(int i=0;i<ACCESS_ADDRESS_WIDTH;i++)
			  begin
         frame[i+PREAMBLE_WIDTH] = acc_addr[i];
			  end

			  $display("frame[8:39] is %p and acc_addr is %h",frame[8:39],acc_addr);
        for(int i=0;i<HEADER_WIDTH;i++)
			  begin
         frame[i+PREAMBLE_WIDTH+ACCESS_ADDRESS_WIDTH] = header[i];
			  end

        for(int i=0;i<((data_length * 8) + CRC_WIDTH);i++)
			  begin
			   @(posedge vif_pkt.clock);
         frame[i+PREAMBLE_WIDTH+ACCESS_ADDRESS_WIDTH+HEADER_WIDTH] = vif_pkt.PPI;  
			  end
			 end
       
			  
     pkt_trans.unpack(frame);
		// $display("tx_monitor::Frame packet size is %d and Frame packet is %p",frame.size(),frame);
 
     ap_tx.write(pkt_trans);

     `ovm_info("TX_MONITOR","Packet sequence item sent to SB",OVM_LOW)
		 pkt_trans.print();
     end
   join_any : TX
   disable TX;

  endtask: ble_tx


// Task for preamble and access address detection  

  task tx_monitor:: preamble_AA_check(output logic [7:0] preamble, logic [31:0] acc_addr);

   logic [0:7] pream = 8'dx;
  // logic [7:0] pream = 8'dx;
   logic [0:31] access_addr = 32'dx;
  // logic [31:0] acc_addr = 32'dx;
   bit pass=0;
   
    forever
     begin
      @(posedge vif_pkt.clock);
       pream = {pream[1:7], access_addr[0]};
       access_addr = {access_addr[1:31], vif_pkt.PPI};
       acc_addr = {<< {access_addr}};
       preamble = {<< {pream}};
       pass = access_addr_check(preamble, acc_addr);
       if(pass == 1'b1)
       begin
			 `ovm_info("TX_MONITOR",$sformatf("\nAccess address detected and verified at %t Acc_addr is %h preamble is %b",$time,acc_addr,preamble),OVM_LOW)
        break;
       end
     end

  endtask

// Function to verify the constraints applied on access address
// returns 1 for correct preamble and access address match
// else returns 0

  function bit tx_monitor:: access_addr_check(logic [7:0] preamble, logic [31:0] access_addr);
   
   if(preamble == 8'b10101010 && access_addr == 32'h8E89BED6) 
    return(1);
   else if((preamble == 8'b10101010 && access_addr[0] == 1'b0) || (preamble == 8'b01010101 && access_addr[0] == 1'b1))
    begin

     foreach(access_addr[i])                           // No consecutive 6 ones
      begin
       if(i<27)
        if((access_addr[i] && access_addr[i+1] && access_addr[i+2] && access_addr[i+3] && access_addr[i+4] && access_addr[i+5]) == 1)
	 begin
	  return(0);
	 end
      end

     foreach(access_addr[i])                           // No consecutive 6 zeros
      begin
       if(i<27)
        if((access_addr[i] || access_addr[i+1] || access_addr[i+2] || access_addr[i+3] || access_addr[i+4] || access_addr[i+5]) == 0)
	 begin
	  return(0);
	 end
      end
 
     if(access_addr == 32'h8E89BED6)                  // Shall not be advertising channel access address
      return(0);
	          
     if((access_addr[31:24] == access_addr[23:16]) && (access_addr[23:16] == access_addr[15:8]) && (access_addr[15:8] == access_addr[7:0]))
      return(0);                                      // All 4 octets should not be same
        
     if(($countones(access_addr ^ (access_addr*2)) -(1&access_addr[0])) >= 25) 
     return(0);                                       // Shall not have more than 24 transitions

     if(($countones(access_addr[31:26] ^ (access_addr[31:26]*2))-(1&access_addr[26])) < 2) 
      return(0);                                      //Shall have a minimum of 2 transitions in most significant six bits

     `ovm_info("TX_MONITOR","\n All constraints for data channel access address have passed",OVM_LOW) 

      return(1);
    end

   else
    return(0);
  
  endfunction 


  task tx_monitor:: soft_reset();
    
   forever
    begin
     //wait(ral_block.ble_rm.command_reg_h.soft_reset.value);
     //break();
    end

  endtask





