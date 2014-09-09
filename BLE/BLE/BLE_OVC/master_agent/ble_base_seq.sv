
class ble_base_seq extends ovm_sequence #(ahb_seq_item);

 `ovm_object_utils(ble_base_seq)

  ble_ral_block ble_ral;      // ral block handle
  ble_ovc_config cfg;          // ovc config handle
  ovm_object temp;            
  uvm_reg_data_t r_data;

  uvm_status_e status; 
  // Common BT packet Variables

	rand bit [7:0]  preamble;             	// STARTING  OF PACKET
	rand bit [31:0] access_address;        
       bit [23:0] crc;
			 bit [31:0] mic;
       bit [7:0] temp_data;
	
 // Type of packet
	typedef enum {ADV,DATA} bt_pkt_type;
  rand bt_pkt_type    pkt_type;     			// data or adv packet.

	// Adv Header fields :
  //	typedef enum  {ADV_IND,ADV_DIRECT_IND_HIGH_DUTY_CYCLE,ADV_SCAN_IND,ADV_NONCONN_IND,ADV_DIRECT_IND_LOW_DUTY_CYCLE} adv_pdu_type;
 	rand adv_pdu_type   adv_type;           // pdu type
  rand bit [3:0]      frm_adv_pdu_type;
	rand bit        		frm_adv_pdu_tx_add; // Indicates whether source device addr is public or random
	rand bit        		frm_adv_pdu_rx_add; // Indicates whether target device addr is public or random
  rand bit [5:0]  		frm_adv_pdu_length; // length field for ADV packets is 6 bits
	rand bit 						adv_filter_policy;	// Advertising filter policy																		@@@@@@@@@@@
	rand bit 						duty_cycle_type;		// conn_directed_duty_cycle type		// 0 -> Low duty // 1 -> High duty
	rand bit [2:0]			adv_channel_map;		
	rand bit [31:0] 		adv_interval_min; //  Range: 0x0020 to 0x4000 
                                        //  N = 0xXXXX
                                        //  Default: N = 0x0800 (1.28 second)
                                        //  Time = N * 0.625 msec
                                        //  Time Range: 20 ms to 10.24 sec
  rand bit [31:0] adv_interval_max;
  
	// Device Address :
  rand bit [47:0] source_device_addr;   	// To store source device address
	rand bit [47:0] target_device_addr;   	// To store target device address
	rand bit [1:0]  rand_dev_addr_type;   	// For random device address which can be of static type (00)
                                        	//  Non resolvable private address type(01) or 
				                                	//  Resolvable private address type (10)

 

	// Advertising PDU fields :
	rand bit [7:0]  frm_adv_data[$];	
        bit [7:0] adv_rx_data[$];	
        bit [5:0] len;			
	rand bit [47:0] frm_adv_addr;       	// To store source device address
	rand bit [47:0] frm_adv_init_add;   	// To store target device address

	// Scanning PDU fields :
	rand bit [7:0]  frm_scan_rsp_data[];					
//	rand bit [47:0] frm_adv_addr;   			// To store source device address
	rand bit [47:0] frm_scan_addr;   				// To store target device address
	rand bit 				scan_type;							// passive or active
	rand bit        scan_device_add_type;		// random or public
	rand bit [1:0]	scan_filter_policy;			
	rand bit [5:0]	scan_resp_data_length;	
	rand bit [2:0]	scan_adv_channel_map;	
	rand bit [15:0] scan_interval;					// Range: 0x0004 to 0x4000
																					//	N = 0xXXXX
																					//	Default: 0x0010 (10 ms)
																					//	Time = N * 0.625 msec
																					//	Time Range: 2.5 msec to 10 . 24 seconds

	rand bit [15:0] scan_window;						//  Range: 0x0004 to 0x4000
																					//	N = 0xXXXX
																					//	Default: 0x0010 (10 ms)
																					//	Time = N * 0.625 msec
																					//	Time Range: 2.5 msec to 10240 msec 


	
	// Connect Pdu fields:
	rand bit [1:0]  init_filter_policy;		//
	rand bit 				peer_address_type;		//
	rand bit 				own_address_type;			//

	rand bit [47:0] frm_conn_init_add;   	// To store source device address
	rand bit [47:0] frm_conn_adv_add;   	// To store target device address

	rand bit [31:0] frm_conn_acc_add;      // 
  rand bit [23:0] frm_conn_crc;         	// CRC initial value for conn_req pkt
  rand bit [7:0]  frm_conn_winsize;				//
  rand bit [15:0] frm_conn_winoffset;
  rand bit [15:0] frm_conn_interval_min;
 rand bit [15:0] frm_conn_interval_max;
  //rand bit [15:0] frm_conn_interval;
  rand bit [15:0] frm_conn_latency;
  rand bit [15:0] frm_conn_timeout;
  rand bit [36:0] frm_conn_chm;
 //rand bit [16:0] frm_conn_latency1;
  //rand bit [16:0] frm_conn_latency2;
	rand bit [15:0] CE_length_min;
	rand bit [15:0] CE_length_max;

  rand bit [4:0]  frm_conn_hop;
  rand bit [2:0]  frm_conn_sca;



  // constraint 
	constraint PKT_TYPE { pkt_type == 0;}		// default adv_packet
	
	// PREAMBLE
	constraint PREAMBLE{if(access_address[0]==1)
                 preamble==8'b01010101;
		            else
		             preamble==8'b10101010;
                }
// CRC
	constraint CRC{if(pkt_type == 0)
                  frm_conn_crc == 24'h555555;}


	// BLE pkt access address
	
	constraint PKT_ACCESS_ADDRESS { (pkt_type == 0)  -> access_address == 32'h8E89BED6;}
								
																		
	// Constraint on Advertising address channel PDU
  constraint ADV_PDU_TYPE{(adv_type== ADV_IND)   ->    frm_adv_pdu_type == 4'b0000;    // Adv undirected
			                    (adv_type==ADV_DIRECT_IND_HIGH_DUTY_CYCLE) ->frm_adv_pdu_type == 4'b0001;    // Adv directed high/low duty cycle
			                    (adv_type==ADV_DIRECT_IND_LOW_DUTY_CYCLE) -> frm_adv_pdu_type == 4'b0001;    // Adv directed high/low duty cycle
			     			          (adv_type==ADV_NONCONN_IND)->frm_adv_pdu_type == 4'b0010;  // Non connectable undirect
		                      (adv_type==ADV_SCAN_IND)  -> frm_adv_pdu_type == 4'b0110;   // Scannable undirected.
			                     }
			   

	//Duty cycle
	constraint DUTY_CYCLE_TYPE{(adv_type==ADV_DIRECT_IND_HIGH_DUTY_CYCLE) -> duty_cycle_type == 1;
														 (adv_type==ADV_DIRECT_IND_LOW_DUTY_CYCLE) ->  duty_cycle_type == 0;}

	// Adv channel map
	constraint ADV_CHANNEL_MAP{	adv_channel_map inside {[1:7]};}

	// connect Access Address
		constraint ACCESS_ADDRESS{ if(pkt_type == 0) 
                  {foreach(frm_conn_acc_add[i])
		                if(i<27)
		               ((frm_conn_acc_add[i] && frm_conn_acc_add[i+1] && frm_conn_acc_add[i+2] && frm_conn_acc_add[i+3] && frm_conn_acc_add[i+4] && frm_conn_acc_add[i+5]) != 1);
                    //No 6 consecutive ones

		               foreach(frm_conn_acc_add[j])
		               if(j<27)
		               ((frm_conn_acc_add[j] || frm_conn_acc_add[j+1] || frm_conn_acc_add[j+2] || frm_conn_acc_add[j+3] || frm_conn_acc_add[j+4] || frm_conn_acc_add[j+5]) != 0); 
                    //No 6 consecutive zeroes
	                  frm_conn_acc_add != 32'h8E89BED6;          // Shall not be Advertising packet's Access address.
    		            frm_conn_acc_add[31:24] != frm_conn_acc_add[23:16]; frm_conn_acc_add[23:16] != frm_conn_acc_add[15:8]; frm_conn_acc_add[15:8] != frm_conn_acc_add[7:0];                       // All 4 octects shall not be same  ;
                     // abc(frm_conn_acc_add)==1;             // Shall not differ by Advertising channel Access address by 1 bit.
                    $countones(frm_conn_acc_add ^ (32'h8E89BED6))>1;
      	            ($countones(frm_conn_acc_add ^ (frm_conn_acc_add*2)) -(1&frm_conn_acc_add[0]))<25; // Shall not have more than 24 transitions
                    ($countones(frm_conn_acc_add[31:26] ^ (frm_conn_acc_add[31:26]*2))-(1&frm_conn_acc_add[26]))>1; 
                    //Shall have a minimum of 2 transitions in most significant six bits
                    }
                else   
    	              {
		                  frm_conn_acc_add == 32'h8E89BED6;
		                } 
	          }	

  
    


	// Constraint on Random device address
  constraint DEVICE_ADDRESS{ rand_dev_addr_type!=2'b11; solve rand_dev_addr_type before source_device_addr;
                if(frm_adv_pdu_tx_add==1) 
		{ 
		 if(rand_dev_addr_type==2'b00) {
                  source_device_addr[47:46]==2'b11;
		  $countones(source_device_addr[45:0])!=46;           // Constraint for static random address.
		  $countones(source_device_addr[45:0])!=0;}
		 else if(rand_dev_addr_type==2'b01) {
		  source_device_addr[47:46]==2'b00;                   // Constraint for Non resolvable private address. 
		  $countones(source_device_addr[45:0])!=46;
		  $countones(source_device_addr[45:0])!=0; }
		}
	       }	

	// lenght
 	constraint ADV_LENGTH{solve adv_type before frm_adv_data;
												solve adv_type before frm_adv_pdu_length; 
												solve frm_adv_pdu_length before frm_adv_data;
 	                      (adv_type== ADV_DIRECT_IND_HIGH_DUTY_CYCLE || adv_type == ADV_DIRECT_IND_LOW_DUTY_CYCLE) -> 
																				( frm_adv_pdu_length == 12 && frm_adv_data.size() == 0 && frm_scan_rsp_data.size() == 0);
		                		(adv_type == ADV_SCAN_IND|| adv_type == ADV_NONCONN_IND || adv_type == ADV_IND) -> 
		                    					{
			                    						frm_adv_pdu_length inside {[6:37]};
			                   							frm_adv_data.size()== (frm_adv_pdu_length - 6);
														      }
                       }
    
  
  
  constraint ADV_INTERVAL_RANGE {adv_interval_min <= adv_interval_max;
                                 adv_interval_min inside {['h20:'h4000]};
                                 adv_interval_max inside {['h20:'h4000]};
                                } 

	// scan response data length 
	constraint SCAN_RESPONSE_DATA_LENGTH {solve scan_resp_data_length before frm_scan_rsp_data;
																				scan_resp_data_length inside {[6:37]};
																				frm_scan_rsp_data.size()== (scan_resp_data_length - 6);
																				}
																				
	 constraint SCAN_INTERVAL_RANGE {
                                 scan_interval inside {['h4:'h4000]};
                                 scan_window inside {['h4:'h4000]};
                                } 

// connect interval
	constraint CON_INTERVAL_RANGE {frm_conn_interval_min <= frm_conn_interval_max;
                                 frm_conn_interval_min inside {['h20:'h0c80]};
                                 frm_conn_interval_max inside {['h20:'h0c80]};
                                } 
	/*/ Connection latency
	constraint CONN_LATENCY_RANGE1 { frm_conn_latency2 inside {[0:'h1f3]};frm_conn_latency1 inside {[0 :((frm_conn_timeout/frm_conn_interval)-1)]}; 
                                   if(frm_conn_latency1<frm_conn_latency2) 
                                     {
                                       frm_conn_latency == frm_conn_latency1;
                                     }
                                   else
                                     {
										                   frm_conn_latency == frm_conn_latency2;
                                     }
                                  }*/

	// Connectin latency
	constraint CONN_LATENCY_RANGE { frm_conn_latency inside {[0:'h1f3]};}
	// Hop
	constraint HOP{frm_conn_hop inside {[5:16]};}

	// supervision timeout
	constraint SUPER_TIMEOUT{ frm_conn_timeout inside {['hA:'h0c80 ]};}
	// channel map
	constraint CHM { $countones(frm_conn_chm) >= 2;}
	// CE_Length
	constraint CE_LENGTH { CE_length_max >=CE_length_max ;}

	


	extern function new(string name = "ble_base_seq");
	extern function void do_print(ovm_printer printer);
	extern task body();
endclass

	task ble_base_seq::body();
	 assert(m_sequencer.get_config_object("ovc_config",temp,0));
           // `ovm_fatal("AHB_SEQ","can not get config in seq")
       assert($cast(cfg,temp));
            //`ovm_fatal("AHB_SEQ","$casting failure")
          ble_ral = cfg.ble_ral;
	
	      	assert(this.randomize());// with {adv_type == ADV_IND;}); 
	endtask

	function ble_base_seq::new(string name = "ble_base_seq");
 	super.new(name);
	endfunction

	function void ble_base_seq::do_print(ovm_printer printer);
  super.do_print(printer);

//	printer.print_field("frm_conn_acc_add", this.frm_conn_acc_add, 32, OVM_HEX);
//  printer.print_field("access_address", this.access_address, 32, OVM_HEX);
//  printer.print_field("preamble", this.preamble, 8, OVM_BIN);
//  printer.print_field("crc_init", this.crc_init, 24, OVM_HEX);
//  printer.print_field("frm_conn_crc", this.frm_conn_crc, 24, OVM_HEX);
//  printer.print_field("mic", this.mic, 32, OVM_HEX);
//	printer.print_field("frm_adv_pdu_length", this.frm_adv_pdu_length, 6, OVM_UNSIGNED); 
// // printer.print_field("frm_adv_pdu_type", this.adv_pdu_type, 4, OVM_BIN);
//  printer.print_field("source_device_addr", this.source_device_addr, 48, OVM_HEX);
//  printer.print_field("target_device_addr", this.target_device_addr, 48, OVM_HEX);
//  printer.print_field("frm_adv_pdu_tx_add", this.adv_pdu_txadd, 1, OVM_BIN);
//  printer.print_field("frm_adv_pdu_rx_add", this.adv_pdu_rxadd, 1, OVM_BIN);
//  printer.print_field("pkt_type", this.pkt_type, 1, OVM_BIN);
//	foreach(adv_data[i])
//	printer.print_field($sformatf("frm_adv_data[%0d]",i), this.adv_data[i], 32, OVM_HEX);
	
//	foreach(frm_scan_rsp_data[i])
//	printer.print_field($sformatf("frm_scan_rsp_data[i]"i), this.frm_scan_rsp_data[i], 32, OVM_HEX);
  /*
	printer.print_field("conn_data[0]", this.conn_data[0], 32, OVM_HEX);
  printer.print_field("conn_data[1]", this.conn_data[1], 32, OVM_HEX);
  printer.print_field("conn_data[2]", this.conn_data[2], 32, OVM_HEX);
  printer.print_field("conn_data[3]", this.conn_data[3], 32, OVM_HEX);
  printer.print_field("conn_data[4]", this.conn_data[4], 32, OVM_HEX);
  printer.print_field("conn_data[5]", this.conn_data[5], 32, OVM_HEX);
  printer.print_field("conn_data[6]", this.conn_data[6], 32, OVM_HEX);
  printer.print_field("conn_data[7]", this.conn_data[7], 32, OVM_HEX);    
  printer.print_field("ctrl_data[0]", this.conn_data[0], 32, OVM_HEX);
  printer.print_field("ctrl_data[1]", this.conn_data[1], 32, OVM_HEX);
  printer.print_field("ctrl_data[2]", this.conn_data[2], 32, OVM_HEX);
  printer.print_field("ctrl_data[3]", this.conn_data[3], 32, OVM_HEX);
  printer.print_field("ctrl_data[4]", this.conn_data[4], 32, OVM_HEX);
  printer.print_field("ctrl_data[5]", this.conn_data[5], 32, OVM_HEX);
  printer.print_field("ctrl_data[6]", this.conn_data[6], 32, OVM_HEX);
  printer.print_field("ctrl_data[7]", this.conn_data[7], 32, OVM_HEX);    */

  //printer.print_field("control_pdu_opcode", this.control_pdu_opcode, 16, OVM_HEX);
  //printer.print_field("data_pdu_llid", this.data_pdu_llid, 2, OVM_BIN);
  // printer.print_field("data_pdu_sn", this.data_pdu_sn, 1, OVM_BIN); 
  //printer.print_field("data_pdu_nesn", this.data_pdu_nesn, 1, OVM_BIN); 
  //printer.print_field("data_pdu_md", this.data_pdu_md, 1, OVM_BIN); 
  //printer.print_field("data_pdu_length", this.data_pdu_length, 5, OVM_UNSIGNED);


endfunction


//**************************************************************************************************************
//                              ADV parameter set sequence
//**************************************************************************************************************


	class adv_param_set_seq extends ble_base_seq;

		`ovm_object_utils(adv_param_set_seq)

	 bit [3:0]      local_frm_adv_pdu_type;
	 bit        		local_frm_adv_pdu_tx_add; // Indicates whether source device addr is public or random
	 bit        		local_frm_adv_pdu_rx_add; // Indicates whether target device addr is public or random
   bit [5:0]  		local_frm_adv_pdu_length; // length field for ADV packets is 6 bits
	 bit            local_adv_filter_policy;
//	 bit [2:0]			local_adv_channel_map;	
//	 bit 						local_duty_cycle_type;	
	 bit [31:0]     adv_parameter_reg;
	 bit [31:0]     adv_interval_min_reg;
	 bit [31:0]     adv_interval_max_reg;
   	
 
   bit status_randomize = 1;
    
  /* 	constraint ADV_LENGTH{//solve adv_type before adv_payload; //solve adv_type before adv_pdu_length;// solve adv_pdu_length before adv_payload;
                     (adv_type== ADV_DIRECT_IND_HIGH_DUTY_CYCLE || adv_type == ADV_DIRECT_IND_LOW_DUTY_CYCLE) ->  frm_adv_pdu_length == 12;
		                 (adv_type == ADV_SCAN_IND|| adv_type == ADV_NONCONN_IND || adv_type == ADV_IND) -> 
		                     {
			                    frm_adv_pdu_length inside {[0:31]};
			                    frm_adv_data.size()==frm_adv_pdu_length;
		                			}
                       }

    */
  
		extern function new(string name = "adv_param_set_seq");
		extern task body();
		

	endclass
	
	function adv_param_set_seq::new(string name = "adv_param_set_seq");
		super.new(name);
	endfunction

	task adv_param_set_seq::body();
	super.body();
  	 	if(status_randomize) // if status_randomize ==  1, random test. 
	begin
	adv_parameter_reg[3:0] = frm_adv_pdu_type;
	adv_parameter_reg[4]   = duty_cycle_type;
	adv_parameter_reg[5]   = frm_adv_pdu_tx_add;
	adv_parameter_reg[6]   = frm_adv_pdu_rx_add;
	adv_parameter_reg[8:7] = adv_filter_policy;
	adv_parameter_reg[11:9]= adv_channel_map;
	adv_parameter_reg[17:12]=frm_adv_pdu_length;
	adv_parameter_reg[31:18]= 0;

  adv_interval_min_reg = adv_interval_min;
  adv_interval_max_reg = adv_interval_max; 
	
	// write sequence
	
  ble_ral.reg_write('h0,'h2);   //state reg for ADV----- bit 1 high
  ble_ral.reg_write('h4,adv_parameter_reg);

  ble_ral.reg_write('h14,adv_interval_min_reg);
  ble_ral.reg_write('h18,adv_interval_max_reg);

  req = ahb_seq_item::type_id::create("req");

  if(frm_adv_data.size != 0)
   begin
   start_item(req);
    assert(req.randomize() with {hsize == 0;hwrite == 1 ; haddr == 'h200;hwdata.size() == frm_adv_data.size();});
          req.print();
   finish_item(req);
   end
//  ble_ral.mem_write('h200,frm_adv_data);
// if(adv_type== ADV_DIRECT_IND_HIGH_DUTY_CYCLE || adv_type == ADV_DIRECT_IND_LOW_DUTY_CYCLE)
//   ble_ral.mem_read('h200,frm_adv_pdu_length - 12,frm_adv_data);
// else  
//   ble_ral.mem_read('h200,frm_adv_pdu_length - 6,frm_adv_data);
 // ble_ral.mem_read('h200,0,frm_adv_data);
  
 
  
	// write method ADV_data;
	end
	else
	begin
	adv_parameter_reg[3:0] = local_frm_adv_pdu_type;
	adv_parameter_reg[4]   = duty_cycle_type;
	adv_parameter_reg[5]   = local_frm_adv_pdu_tx_add;
	adv_parameter_reg[6]   = local_frm_adv_pdu_rx_add;
	adv_parameter_reg[8:7] = local_adv_filter_policy;
	adv_parameter_reg[11:9]= adv_channel_map;
	adv_parameter_reg[17:12]=local_frm_adv_pdu_length;
	adv_parameter_reg[31:18]= 0;

	//write to ral model
	ble_ral.reg_write('h4,adv_parameter_reg);
  ble_ral.reg_write('h14,adv_interval_min_reg);
  ble_ral.reg_write('h18,adv_interval_max_reg);


 // ble_ral.mem_write('h200,frm_adv_data);
	end
	endtask
	

class adv_directed_pdu_seq extends adv_param_set_seq;
  `ovm_object_utils(adv_directed_pdu_seq);
     adv_pdu_type   local_adv_type; 
	
    constraint PDU_TYPE {adv_type == local_adv_type;}

    function new (string name = "adv_directed_pdu_seq");
      super.new(name);
    endfunction

    task body();
      super.body();
    endtask

endclass
	

//**************************************************************************************************************
//                               Device address write sequence
//**************************************************************************************************************

	class advertising_add_set_seq extends ble_base_seq;

		`ovm_object_utils(advertising_add_set_seq)
  	 
		 int status_randomize = 0;
		 bit [31:0] local_own_device_address_lsb;
		 bit [31:0] local_own_device_address_msb;
		 bit [31:0] local_directed_device_address_lsb;
		 bit [31:0] local_directed_device_address_msb;
		
		 
		constraint OWN_DEVICE_ADDRESS {   frm_adv_addr == source_device_addr;}
		constraint DIRECTED_DEVICE_ADDRESS { frm_adv_init_add == target_device_addr;}

		extern function new(string name = "advertising_add_set_seq");
		extern task body();
		

	endclass
	
	function advertising_add_set_seq::new(string name = "advertising_add_set_seq");
		super.new(name);
	endfunction

	task advertising_add_set_seq::body();
	
	super.body();
	 	if(status_randomize) // if status_randomize ==  1, directed test. 
		begin
	local_own_device_address_lsb = frm_adv_addr[31:0];     	
	local_own_device_address_msb = {16'b0,frm_adv_addr[47:32]};    // write sequence
	local_directed_device_address_lsb = {frm_adv_init_add[31:0]};
	local_directed_device_address_msb = {16'b0,frm_adv_init_add[47:32]};
	
	//method to write in reg model 
	//address
		//address
		ble_ral.reg_write('h8,local_own_device_address_lsb);
	  ble_ral.reg_write('hc,local_own_device_address_msb);
		ble_ral.reg_write('h1c,local_directed_device_address_lsb);
	  ble_ral.reg_write('h20,local_directed_device_address_msb);
   
	end
	else
	begin
		//write to ral model
	//address
		//address
		ble_ral.reg_write('h8,local_own_device_address_lsb);
	  ble_ral.reg_write('hc,local_own_device_address_msb);
		ble_ral.reg_write('h1c,local_directed_device_address_lsb);
	  ble_ral.reg_write('h20,local_directed_device_address_msb);
	end
	endtask


//**************************************************************************************************************
//                               scan response parameter set seq
//**************************************************************************************************************


	class scan_resp_param_set_seq extends ble_base_seq;

		`ovm_object_utils(scan_resp_param_set_seq)

	 bit [5:0]	local_scan_resp_data_length;		
	 
	 bit [31:0] scan_parameter_reg;
																			
	constraint OWN_DEVICE_ADDRESS {   frm_scan_addr == source_device_addr;}
																			
																					
																					 
   bit status_randomize = 1;
    
 		extern function new(string name = "scan_resp_param_set_seq");
		extern task body();
		

	endclass
	
	function scan_resp_param_set_seq::new(string name = "scan_resp_param_set_seq");
		super.new(name);
	endfunction

	task scan_resp_param_set_seq::body();
	super.body();
  if(status_randomize) // if status_randomize ==  1, random test. 
	begin
	scan_parameter_reg[0] 	  = 0;
	scan_parameter_reg[1]     = 0;
	scan_parameter_reg[3:2]   = 0;
	scan_parameter_reg[9:4]   = scan_resp_data_length;											// scan response data length
	scan_parameter_reg[12:10] = 0;			
	scan_parameter_reg[31:13] = 0;												// reserved

 //	frm_scan_rsp_data	;																	// response data


	// write sequence
		//scan_paramreg
		ble_ral.reg_write('h28,scan_parameter_reg);
				//address
 // 	ble_ral.mem_write(`scan_resp_data_base_addr,frm_adv_data);
		
	end
	else
	begin
	scan_parameter_reg[0] 	  = 0;
	scan_parameter_reg[1]     = 0;
	scan_parameter_reg[3:2]   = 0;
	scan_parameter_reg[9:4]   = local_scan_resp_data_length;											// scan response data length
	scan_parameter_reg[12:10] = 0;			
	scan_parameter_reg[31:13] = 0;												// reserved

 	// scan response data 
	
	// write sequence
		//scan_paramreg
		ble_ral.reg_write('h28,scan_parameter_reg);
				//address
 // 	ble_ral.mem_write(`scan_resp_data_base_addr,frm_adv_data);
	end
	endtask





//**************************************************************************************************************
//             scan enable parameter set sequence - to configure in scan state
//**************************************************************************************************************

	class scan_enable_param_set_seq extends ble_base_seq;

		`ovm_object_utils(scan_enable_param_set_seq)

	 bit 				local_scan_type;							// passive or active
	 bit        local_scan_device_add_type;		// random or public
	 bit [1:0]	local_scan_filter_policy;	
	 bit [5:0]	local_scan_response_data_length;		
	 bit [2:0]	local_scan_adv_channel_map;	
	 bit [31:0] local_scan_interval;				
	 bit [31:0] local_scan_window;					
	 bit [31:0] local_own_device_address_lsb;
	 bit [31:0] local_own_device_address_msb;

	 bit [31:0] scan_parameter_reg;
																			
	constraint OWN_DEVICE_ADDRESS {   frm_scan_addr == source_device_addr;}
																			
																					
																					 
   bit status_randomize = 1;
    
 		extern function new(string name = "scan_enable_param_set_seq");
		extern task body();
		

	endclass
	
	function scan_enable_param_set_seq::new(string name = "scan_enable_param_set_seq");
		super.new(name);
	endfunction

	task scan_enable_param_set_seq::body();
	super.body();
  if(status_randomize) // if status_randomize ==  1, random test. 
	begin
	//scan_parameter_reg[0] 	  = scan_type;
	scan_parameter_reg[0] 	  = 0;
	scan_parameter_reg[1]     = scan_device_add_type;
	scan_parameter_reg[3:2]   = scan_filter_policy;
	scan_parameter_reg[9:4]   = 'b0;											// scan response data length
	scan_parameter_reg[12:10] = scan_adv_channel_map;			
	scan_parameter_reg[31:13] = 0;												// reserved

  local_scan_interval = {16'b0,scan_interval};
  local_scan_window = 	{16'b0,scan_window}; 
	
	local_own_device_address_lsb = frm_scan_addr[31:0];     	
	local_own_device_address_msb = {16'b0,frm_scan_addr[47:32]};   



	// write sequence
		//scan_paramreg
		ble_ral.reg_write('h0,'h4);       // state register ---- bit 3 high for scan state
  	ble_ral.reg_write('h28,scan_parameter_reg);
		//scan window
	  ble_ral.reg_write('h44,local_scan_window);
		//scan interwal
	  ble_ral.reg_write('h40,local_scan_interval);
		//address
		ble_ral.reg_write('h2c,local_own_device_address_lsb);
	  ble_ral.reg_write('h30,local_own_device_address_msb);
		
	end
	else
	begin
	scan_parameter_reg[0] 	  = local_scan_type;
	scan_parameter_reg[1]     = local_scan_device_add_type;
	scan_parameter_reg[3:2]   = local_scan_filter_policy;
	scan_parameter_reg[9:4]   = 'b0;											// scan response data length
	scan_parameter_reg[12:10] = local_scan_adv_channel_map;			
	scan_parameter_reg[31:13] = 0;												// reserved

  local_scan_interval = {16'b0,scan_interval};
  local_scan_window = 	{16'b0,scan_window}; 
	
	local_own_device_address_lsb = frm_scan_addr[31:0];     	
	local_own_device_address_msb = {16'b0,frm_scan_addr[47:32]};   


	//write to ral model
	// write sequence
		//scan_paramreg
		ble_ral.reg_write('h0,'h4);  
  	ble_ral.reg_write('h28,scan_parameter_reg);
		//scan window
	  ble_ral.reg_write('h44,local_scan_window);
		//scan interwal
	  ble_ral.reg_write('h40,local_scan_interval);
		//address
		ble_ral.reg_write('h2c,local_own_device_address_lsb);
	  ble_ral.reg_write('h30,local_own_device_address_msb);
	end
	endtask

	

//**************************************************************************************************************
//                     sequence to start advertising
//**************************************************************************************************************

	class adv_enable extends ble_base_seq;
	
		`ovm_object_utils(adv_enable);
	

	bit [31:0] command_reg;
	extern function new(string name = "adv_enable");
	extern task body();

	endclass

	function adv_enable::new(string name = "adv_enable");

		super.new(name);

	endfunction

	task adv_enable::body();
	super.body();	
	command_reg[0] = 1'b1;
  command_reg[18] = 1'b1;
	// adv_enable 
  ble_ral.reg_write('he4,command_reg);

	endtask


          


//**************************************************************************************************************
//                     sequence to read intrrupts
//**************************************************************************************************************
	
class adv_received_interrupt_seq extends ble_base_seq;
   `ovm_object_utils(adv_received_interrupt_seq)

    function new (string name = "adv_received_interrupt_seq");
      super.new(name);
    endfunction

    task  body();
      super.body();
      
        begin
          wait(ble_ral.ble_rm.ISR_h.pkt_received.value);
          ble_ral.ble_rm.ISR_h.pkt_received.set(0);
        end
    endtask

endclass
          
	

//**************************************************************************************************************
//                     sequence to enable interrupts
//**************************************************************************************************************
	

	class interrupt_enable extends ble_base_seq;
	
		`ovm_object_utils(interrupt_enable);
	

	bit [31:0] interrupt_enable;
	extern function new(string name = "interrupt_enable");
	extern task body();

	endclass

	function interrupt_enable::new(string name = "interrupt_enable");

		super.new(name);

	endfunction

	task interrupt_enable::body();
	  super.body();	
	interrupt_enable = {32'hFFFFFFFF};

	// Enable all the interrupt
  ble_ral.reg_write('he0,interrupt_enable);


	endtask

	

//************************************************************************************

class adv_set_config_seq extends ble_base_seq;

		`ovm_object_utils(adv_set_config_seq)

	 bit [3:0]      local_frm_adv_pdu_type;
	 bit        		local_frm_adv_pdu_tx_add; // Indicates whether source device addr is public or random
	 bit        		local_frm_adv_pdu_rx_add; // Indicates whether target device addr is public or random
   bit [5:0]  		local_frm_adv_pdu_length; // length field for ADV packets is 6 bits
	 bit            local_adv_filter_policy;
//	 bit [2:0]			local_adv_channel_map;	
//	 bit 						local_duty_cycle_type;	
	 bit [31:0]     adv_parameter_reg;
	 bit [31:0]     adv_interval_min_reg;
	 bit [31:0]     adv_interval_max_reg;
   string  local_pdu_type;
  bit [5:0]  local_length;

 
  // bit status_randomize = 1;
    
  /* 	constraint ADV_LENGTH{//solve adv_type before adv_payload; //solve adv_type before adv_pdu_length;// solve adv_pdu_length before adv_payload;
                     (adv_type== ADV_DIRECT_IND_HIGH_DUTY_CYCLE || adv_type == ADV_DIRECT_IND_LOW_DUTY_CYCLE) ->  frm_adv_pdu_length == 12;
		                 (adv_type == ADV_SCAN_IND|| adv_type == ADV_NONCONN_IND || adv_type == ADV_IND) -> 
		                     {
			                    frm_adv_pdu_length inside {[0:31]};
			                    frm_adv_data.size()==frm_adv_pdu_length;
		                			}
                       }

    */
  
		extern function new(string name = "adv_set_config_seq");
		extern task body();
		

	endclass



function adv_set_config_seq::new(string name = "adv_set_config_seq");
		super.new(name);
	endfunction

	task adv_set_config_seq::body();
   // adv_pdu_type = pdu_type;

	super.body();
    assert(this.randomize() with {adv_type == ADV_NONCONN_IND;frm_adv_pdu_length ==10;});
//  	 	if(status_randomize) // if status_randomize ==  1, random test. 
//	begin
	adv_parameter_reg[3:0] = frm_adv_pdu_type;
	adv_parameter_reg[4]   = duty_cycle_type;
	adv_parameter_reg[5]   = frm_adv_pdu_tx_add;
	adv_parameter_reg[6]   = frm_adv_pdu_rx_add;
	adv_parameter_reg[8:7] = adv_filter_policy;
	adv_parameter_reg[11:9]= adv_channel_map;
	adv_parameter_reg[17:12]=frm_adv_pdu_length;
	adv_parameter_reg[31:18]= 0;
//
//  adv_interval_min_reg = adv_interval_min;
//  adv_interval_max_reg = adv_interval_max;
// 
//	
//	// write sequence
////	ble_ral.ble_rm.adv_params_h.set(adv_parameter_reg);
////  ble_ral.ble_rm.adv_interval_min_reg_h.set(adv_interval_min_reg);
////  ble_ral.ble_rm.adv_interval_max_reg_h.set(adv_interval_max_reg);

  ble_ral.reg_write('h4,adv_parameter_reg);                   //adv_parameter_register 
  ble_ral.reg_write('h14,adv_interval_min_reg);               //adv_interval_min_register
  ble_ral.reg_write('h18,adv_interval_max_reg);               //adv_interval_max_register
  ble_ral.reg_write('h8,source_device_addr[31:0]);            //source device address register
  ble_ral.reg_write('hc,{16'b0,source_device_addr[47:32]});   
  ble_ral.reg_write('h1c,target_device_addr[31:0]);           // target device address (incase of ADV_DIRECT)
  ble_ral.reg_write('h20,{16'b0,target_device_addr[47:32]});


//  ble_ral.mem_write(32'h200,frm_adv_data);
   
 
// ble_ral.ral_read('d500, adv_rd_data); 
 //   foreach(frm_adv_data[i])
  //      begin
   //       ble_ral.adv_tx_data.add('d500 + i , frm_adv_data[i]);
   //     end

  //  temp_data = ble_ral.adv_tx_data.get('d501);

       len = frm_adv_data.size();
  //  for(int i= 0;i<len;i++)
  //   begin
  //    adv_rx_data[i] = ble_ral.adv_tx_data.get('d500 + i);
  //  end
   ble_ral.mem_read('h200,len ,adv_rx_data);

  //r_data = ble_ral.ble_rm.adv_params_h.get();

	// write method ADV_data;
	//end
//	else
//	begin
//	adv_parameter_reg[3:0] = local_frm_adv_pdu_type;
//	adv_parameter_reg[4]   = duty_cycle_type;
//	adv_parameter_reg[5]   = local_frm_adv_pdu_tx_add;
//	adv_parameter_reg[6]   = local_frm_adv_pdu_rx_add;
//	adv_parameter_reg[8:7] = local_adv_filter_policy;
//	adv_parameter_reg[11:9]= adv_channel_map;
//	adv_parameter_reg[17:12]=local_frm_adv_pdu_length;
//	adv_parameter_reg[31:18]= 0;
//
//	//write to ral model
////	.set();
//	end
	endtask
//**************************************************************************************************************
//                     sequence to start scanning
//**************************************************************************************************************

	class scan_enable extends ble_base_seq;
	
		`ovm_object_utils(scan_enable);
	

	bit [31:0] command_reg;
	extern function new(string name = "scan_enable");
	extern task body();

	endclass

	function scan_enable::new(string name = "scan_enable");
		super.new(name);
	endfunction

	task scan_enable::body();
	super.body();	
	command_reg[2] = 1;
  command_reg[18] = 1;

	// adv_enable 
  ble_ral.reg_write('he4,command_reg);

	endtask
	




//********************************************
	class create_conn_param_set_seq extends ble_base_seq;

		`ovm_object_utils(create_conn_param_set_seq)

	 bit 				local_own_device_add_type;
	 bit        local_peer_device_add_type;		// random or public
	 bit [1:0]	local_init_filter_policy;	
	 bit [31:0] local_conn_access_address;

	 bit [31:0] local_frm_conn_crc;

	 bit [31:0] conn_parameter_reg;
																			
																						 
   bit status_randomize = 1;
    
 		extern function new(string name = "create_conn_param_set_seq");
		extern task body();
		

	endclass
	
	function create_conn_param_set_seq::new(string name = "create_conn_param_set_seq");
		super.new(name);
	endfunction

	task create_conn_param_set_seq::body();
	super.body();
  if(status_randomize) // if status_randomize ==  1, random test. 
	begin
	conn_parameter_reg[1:0]	  = init_filter_policy;
	conn_parameter_reg[2]     = peer_address_type;
	conn_parameter_reg[3]   	= own_address_type;
	conn_parameter_reg[31:4] 	= 0;												// reserved

	local_frm_conn_crc = {8'b0,frm_conn_crc};

	local_conn_access_address = frm_conn_acc_add;

  	// write sequence
		//scan_paramreg
  	ble_ral.reg_write('h48,conn_parameter_reg);
		//scan window
	  ble_ral.reg_write('h5c,local_conn_access_address);
		//scan interwal
	  ble_ral.reg_write('h60,local_frm_conn_crc);
			
	end
	else
	begin
	conn_parameter_reg[1:0]	  = local_init_filter_policy;
	conn_parameter_reg[2]     = local_peer_device_add_type;
	conn_parameter_reg[3]   	= local_own_device_add_type;
	conn_parameter_reg[31:4] 	= 0;												// reserved

	
  	// write sequence
		//conn_paramreg
  	ble_ral.reg_write('h48,conn_parameter_reg);
		//channel access address
	  ble_ral.reg_write('h5c,local_conn_access_address);
		//crc init
	  ble_ral.reg_write('h60,local_frm_conn_crc);
			
	end
	endtask

//********************************************
	class conn_timing_param_set_seq extends ble_base_seq;

		`ovm_object_utils(conn_timing_param_set_seq)

	 bit [15:0] local_conn_interval_min;				
	 bit [15:0] local_conn_interval_max;				
	 bit [15:0] local_latency;
	 bit [15:0] local_sup_timeout;
	 bit [4:0]  local_conn_hop;
	 bit [2:0]  local_sca;


																			
																						 
   bit status_randomize = 1;
    
 		extern function new(string name = "conn_timing_param_set_seq");
		extern task body();
		

	endclass
	
	function conn_timing_param_set_seq::new(string name = "conn_timing_param_set_seq");
		super.new(name);
	endfunction

	task conn_timing_param_set_seq::body();
	super.body();
  if(status_randomize) // if status_randomize ==  1, random test. 
	begin
	
	local_conn_interval_min = frm_conn_interval_min;
  local_conn_interval_max = frm_conn_interval_max;
  local_latency           = frm_conn_latency;
  local_sup_timeout       = frm_conn_timeout;
  local_conn_hop					= frm_conn_hop;
  local_sca 							=	frm_conn_sca; 

  	// write sequence
		// Conn Interval Min
  	ble_ral.reg_write('h48,local_conn_interval_min);
		//  Conn Interval Max
	  ble_ral.reg_write('h5c,local_conn_interval_max);
		//  Conn Latency
	  ble_ral.reg_write('h60,local_latency);
		//  Sup Timeout                                   	
    ble_ral.reg_write('h60,local_sup_timeout);
		//  Conn Hop
		ble_ral.reg_write('h60,local_conn_hop);
		// Conn  SCA
		ble_ral.reg_write('h60,local_sca);

	end
	else
	begin
	
  	// write sequence
		// Conn Interval Min
  	ble_ral.reg_write('h48,local_conn_interval_min);
		//  Conn Interval Max
	  ble_ral.reg_write('h5c,local_conn_interval_max);
		//  Conn Latency
	  ble_ral.reg_write('h60,local_latency);
		//  Sup Timeout                                   	
    ble_ral.reg_write('h60,local_sup_timeout);
		//  Conn Hop
		ble_ral.reg_write('h60,local_conn_hop);
		// Conn  SCA
		ble_ral.reg_write('h60,local_sca);
	end
	endtask

// ***********************************************************************************

// data_channel_set_seq:

	class data_channel_set_seq extends ble_base_seq;

		`ovm_object_utils(data_channel_set_seq)
  	 
		 int status_randomize = 0;
		 bit [31:0] local_frm_conn_chm_lsb;	
		 bit [31:0] local_frm_conn_chm_msb;	
		 
	
		extern function new(string name = "data_channel_set_seq");
		extern task body();
		

	endclass
	
	function data_channel_set_seq::new(string name = "data_channel_set_seq");
		super.new(name);
	endfunction

	task data_channel_set_seq::body();
	
	super.body();
	 	if(status_randomize) // if status_randomize ==  1, directed test. 
		begin
		local_frm_conn_chm_lsb = frm_conn_chm[31:0];	
		local_frm_conn_chm_msb = {27'b0,frm_conn_chm[36:32]};	
	//method to write in reg model 
	//address
		//address
		ble_ral.reg_write('h80,local_frm_conn_chm_lsb);
	  ble_ral.reg_write('h84,local_frm_conn_chm_msb);
			end
	else
	begin
		//write to ral model
	//address
		//address
		ble_ral.reg_write('h80,local_frm_conn_chm_lsb);
	  ble_ral.reg_write('h84,local_frm_conn_chm_msb);
			end
	endtask

//************************************************************************************
// Device address write sequence :

	class connection_add_set_seq extends ble_base_seq;

		`ovm_object_utils(connection_add_set_seq)
  	 
		 int status_randomize = 0;
		 bit [31:0] local_own_device_address_lsb;
		 bit [31:0] local_own_device_address_msb;
		 bit [31:0] local_directed_device_address_lsb;
		 bit [31:0] local_directed_device_address_msb;
		
		 
		constraint OWN_DEVICE_ADDRESS {   frm_conn_init_add == source_device_addr;}
		constraint DIRECTED_DEVICE_ADDRESS { frm_conn_adv_add == target_device_addr;}

		extern function new(string name = "connection_add_set_seq");
		extern task body();
		

	endclass
	
	function connection_add_set_seq::new(string name = "connection_add_set_seq");
		super.new(name);
	endfunction

	task connection_add_set_seq::body();
	
	super.body();
	 	if(status_randomize) // if status_randomize ==  1, directed test. 
		begin
	local_own_device_address_lsb = frm_conn_init_add[31:0];     	
	local_own_device_address_msb = {16'b0,frm_conn_init_add[47:32]};    // write sequence
	local_directed_device_address_lsb = {frm_conn_adv_add[31:0]};
	local_directed_device_address_msb = {16'b0,frm_conn_adv_add[47:32]};
	
	//method to write in reg model 
	//address
		//address
		ble_ral.reg_write('h4c,local_own_device_address_lsb);
	  ble_ral.reg_write('h50,local_own_device_address_msb);
		ble_ral.reg_write('h54,local_directed_device_address_lsb);
	  ble_ral.reg_write('h58,local_directed_device_address_msb);
	end
	else
	begin
		//write to ral model
	//address
		//address
		ble_ral.reg_write('h4c,local_own_device_address_lsb);
	  ble_ral.reg_write('h50,local_own_device_address_msb);
		ble_ral.reg_write('h54,local_directed_device_address_lsb);
	  ble_ral.reg_write('h58,local_directed_device_address_msb);
	end
	endtask


//**************************************************************************************************************
//                     sequence to start init
//**************************************************************************************************************

	class init_enable extends ble_base_seq;
	
		`ovm_object_utils(init_enable);
	

	bit [31:0] command_reg;
	extern function new(string name = "init_enable");
	extern task body();

	endclass

	function init_enable::new(string name = "init_enable");
		super.new(name);
	endfunction

	task init_enable::body();
	super.body();	
	command_reg[4] = 1;

	// adv_enable 
  ble_ral.reg_write('he4,command_reg);
	

	endtask


//**************************************************************************************************************
//                     sequence to configure init state
//**************************************************************************************************************

	class init_config_seq extends ble_base_seq;
	
		`ovm_object_utils(init_enable);
	

	
	extern function new(string name = "init_config_seq");
	extern task body();

	endclass

	function init_config_seq::new(string name = "init_config_seq");
		super.new(name);
	endfunction

	task init_config_seq::body();
	super.body();	
	

	// adv_enable 
  ble_ral.reg_write('h0,'h8);       // state register ---- bit 3 high for INIT state
	

	endtask



//**************************************************************************************************************
//                     sequence to reset RAL
//**************************************************************************************************************

class ral_reset_seq extends ble_base_seq;
  `ovm_object_utils(ral_reset_seq)
  
  function new(string name = "ral_reset_seq");
		super.new(name);
	endfunction
  
  task body();
    super.body();
    ble_ral.reset();
  endtask

endclass
