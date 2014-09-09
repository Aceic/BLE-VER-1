class pkt_seq_item extends ovm_sequence_item;

 `ovm_object_utils(pkt_seq_item)


// Common BT packet Variables
//added by Sundeep
	bit [5:0] channel_index;
	rand bit enc;

	rand bit [0:7]  preamble;             	// STARTING  OF PACKET
	rand bit [31:0] access_address;        
        rand bit [23:0] crc;
	//bit [31:0] mic;

	// Type of packet
	typedef enum {ADV,DATA} bt_pkt_type;
        bt_pkt_type pkt_type;     			// data or adv packet.

	// Adv Header fields :
	typedef enum  {ADV_IND,ADV_DIRECT_IND,ADV_NONCONN_IND,SCAN_REQ,SCAN_RSP,CONNECT_REQ,ADV_SCAN_IND} adv_pdu_type;
 	adv_pdu_type adv_type;   // pdu type

        bit [3:0] frm_adv_pdu_type;
	bit duty_cycle_type;

	bit frm_adv_pdu_tx_add; // Indicates whether source device addr is public or random
	bit frm_adv_pdu_rx_add; // Indicates whether target device addr is public or random

	bit [1:0] adv_filter_policy;
	bit [2:0] adv_channel_map;

        bit [5:0] frm_adv_pdu_length; // length field for ADV packets is 6 bits

	// Device Address :
        rand bit [47:0] source_device_addr;   	// To store source device address   // not used in do_pack
	rand bit [47:0] target_device_addr;   	// To store target device address   // not used in do_pack
	rand bit [1:0]  rand_dev_addr_type;   	// For random device address which can be of static type (00)
                                        	//  Non resolvable private address type(01) or 
				                                	//  Resolvable private address type (10)
	// Advertising PDU fields :
	bit [7:0]  frm_adv_data[];					
	rand bit [47:0] frm_adv_addr;   	// To store source device address
	rand bit [47:0] frm_adv_init_add;   	// To store target device address    // not used in do_pack

	// Scanning PDU fields :
	rand bit [7:0]  frm_scan_rsp_data[];					
	bit [47:0] frm_scan_addr;   	// To store target device address
	bit scan_type;                          // To show active or passive scanning
        bit [15:0] scan_interval;
	bit [15:0] scan_window;

	// Connect Pdu fields:
	bit [47:0] frm_conn_init_add;   	// To store source device address

 // LLDATA Field
	rand bit [31:0] frm_conn_acc_add;       
        bit [23:0] frm_crc_init;         	// CRC initial value for conn_req pkt

  rand bit [7:0]  frm_winsize;				//
  rand bit [15:0] frm_winoffset;
  rand bit [15:0] frm_interval;
  rand bit [15:0] frm_latency;
  rand bit [15:0] frm_timeout;
  rand bit [39:0] frm_chm;
  rand bit [4:0]  frm_hop;
  rand bit [2:0]  frm_sca;

	rand bit [15:0] frm_interval_min;
	rand bit [15:0] frm_interval_max;
// Rand variable for connection state data pdu
  rand bit [1:0] llid;
  rand bit nesn;
  rand bit sn;
  rand bit md;
  rand bit [4:0] data_pdu_length;
  rand bit [7:0] data_payload[];
  rand bit [31:0] mic;

// Rand variable for connection state control pdu
  rand bit [7:0] 	opcode;
	rand bit [0:15] instant;
	rand bit [0:63] enc_rand;
	rand bit [0:15] enc_ediv;
	rand bit [0:63] enc_skd;
	rand bit [0:31] enc_iv;
	rand bit [0:63] feature_set_data;
	rand bit [0:7] 	version_vernr;
	rand bit [0:15] version_compid;
	rand bit [0:15] version_sub_vernr;
	rand bit [0:119] conn_param_offset;
	rand bit [0:7] 	error_code;
	rand bit [0:7] 	unknown_reject_opcode;
// Storing complete header values

  bit [15:0] data_header;
  bit [15:0] adv_header;
// whether to drive packets when TX = 1 or RX = 1. Based on these bits, in Tx agent driver I will drive the data on posedge of TX_ON or posedge of RX_ON.
  bit tx;
  bit rx;
  bit frame_pkt[];

  // constraint 
	// PREAMBLE
	constraint PREAMBLE{if(frm_conn_acc_add[0]==1)
                             preamble==8'b10101010;
		            else
		             preamble==8'b01010101;
                           }

	// BLE pkt access address
//	constraint PKT_ACCESS_ADDRESS { (pkt_type == 0)  -> access_address == 32'h8E89BED6;}
								
																		
	// Constraint on Advertising address channel PDU
  constraint ADV_PDU_TYPE{solve adv_type before frm_adv_pdu_type;
                          (adv_type == ADV_IND)        ->    frm_adv_pdu_type == 4'b0000;    // Adv undirected
			  (adv_type == ADV_DIRECT_IND) ->    frm_adv_pdu_type == 4'b0001;    // Adv directed high/low duty cycle
			  (adv_type == ADV_NONCONN_IND)->    frm_adv_pdu_type == 4'b0010;  // Non connectable undirect
                          (adv_type == SCAN_REQ)       ->    frm_adv_pdu_type == 4'b0011;    // Scan req pdu
                          (adv_type == SCAN_RSP)       ->    frm_adv_pdu_type == 4'b0100;    // Scan rsp
			  (adv_type == CONNECT_REQ)    ->    frm_adv_pdu_type == 4'b0101;   // Conn req
		          (adv_type == ADV_SCAN_IND)   ->    frm_adv_pdu_type == 4'b0110;   // Scannable undirected.
			 }
			   

	// Access Address
 constraint AA{ if(pkt_type == 1) {foreach(frm_conn_acc_add[i])
		  if(i<27)
		    ((frm_conn_acc_add[i] && frm_conn_acc_add[i+1] && frm_conn_acc_add[i+2] && frm_conn_acc_add[i+3] && frm_conn_acc_add[i+4] && frm_conn_acc_add[i+5]) != 1);      //No 6 consecutive ones

		 foreach(frm_conn_acc_add[j])
		  if(j<27)
		    ((frm_conn_acc_add[j] || frm_conn_acc_add[j+1] || frm_conn_acc_add[j+2] || frm_conn_acc_add[j+3] || frm_conn_acc_add[j+4] || frm_conn_acc_add[j+5]) != 0);      //No 6 consecutive zeroes
	
	         frm_conn_acc_add != 32'h8E89BED6;          // Shall not be Advertising packet's Access address.

		 frm_conn_acc_add[31:24] != frm_conn_acc_add[23:16]; frm_conn_acc_add[23:16] != frm_conn_acc_add[15:8]; frm_conn_acc_add[15:8] != frm_conn_acc_add[7:0];       // All 4 octects shall not be same  ;
 
              		 
                 $countones(frm_conn_acc_add ^ (32'h8E89BED6))>1;
                  
		 ($countones(frm_conn_acc_add ^ (frm_conn_acc_add*2)) -(1&frm_conn_acc_add[0]))<25; // Shall not have more than 24 transitions

                 ($countones(frm_conn_acc_add[31:26] ^ (frm_conn_acc_add[31:26]*2))-(1&frm_conn_acc_add[26]))>1; //Shall have a minimum of 2 transitions in most significant six bits

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
 	constraint ADV_LENGTH{solve adv_type before frm_adv_pdu_length; solve frm_adv_pdu_length before frm_adv_data; 
	                      solve frm_adv_pdu_length before frm_scan_rsp_data; solve adv_type before frm_adv_data; 
                               if(adv_type==1 || adv_type == 3)
			        {
				 frm_adv_pdu_length == 12;
                                 frm_adv_data.size() == 'd0;
				 frm_scan_rsp_data.size() == 'd0;
				}
                               else if(adv_type == 5)
			        {
				 frm_adv_pdu_length == 'd34;
                                 frm_adv_data.size() == 'd0;
				 frm_scan_rsp_data.size() == 'd0;
				}
		               else if(adv_type == 0 || adv_type ==2 || adv_type == 4 || adv_type == 6)  
		                {
			         frm_adv_pdu_length inside {[6:37]};
			         frm_adv_data.size() == (frm_adv_pdu_length - 6);
				 frm_scan_rsp_data.size() == (frm_adv_pdu_length - 6);
		                }
                             }


 constraint LLID{llid != 2'b00;}

 constraint dt_length{solve data_pdu_length before data_payload; solve opcode before data_pdu_length;
                      if(llid == 2'b10)
		       {
		        data_pdu_length != 0;
			data_pdu_length inside {[1:31]};
                        data_payload.size() == (data_pdu_length-4);
		       }
		      else if(llid == 2'b11)
		       {
		         opcode inside {[0:19]};
		         if(opcode == 8'h00)
			  {
			   data_pdu_length == 12;
			   data_payload.size() == 0;
			  }
			 else if(opcode == 8'h01)
			  {
			   data_pdu_length == 8;
			   data_payload.size() == 0;
			  }
                         else if(opcode == 8'h02)
			  {
			   data_pdu_length == 2;
			   data_payload.size() == 0;
			  }
			 else if(opcode == 8'h03)
			  {
			   data_pdu_length == 23;
			   data_payload.size() == 0;
			  }
			 else if(opcode == 8'h04)
			  {
			   data_pdu_length == 13;
			   data_payload.size() == 0;
			  }
			 else if(opcode == 8'h05)
			  {
			   data_pdu_length == 1;
			   data_payload.size() == 0;
			  }
			 else if(opcode == 8'h06)
			  {
			   data_pdu_length == 1;
			   data_payload.size() == 0;
			  }
			 else if(opcode == 8'h07)
			  {
			   data_pdu_length == 2;
			   data_payload.size() == 0;
			  }
			 else if(opcode == 8'h08)
			  {
			   data_pdu_length == 9;
			   data_payload.size() == 0;
			  }
			 else if(opcode == 8'h09)
			  {
			   data_pdu_length == 9;
			   data_payload.size() == 0;
			  }
			 else if(opcode == 8'h0A)
			  {
			   data_pdu_length == 1;
			   data_payload.size() == 0;
			  }
			 else if(opcode == 8'h0B)
			  {
			   data_pdu_length == 1;
			   data_payload.size() == 0;
			  }
			 else if(opcode == 8'h0C)
			  {
			   data_pdu_length == 6;
			   data_payload.size() == 0;
			  }
			 else if(opcode == 8'h0D)
			  {
			   data_pdu_length == 2;
			   data_payload.size() == 0;
			  }
			 else if(opcode == 8'h0E)
			  {
			   data_pdu_length == 9;
			   data_payload.size() == 0;
			  }
			 else if(opcode == 8'h0F)
			  {
			   data_pdu_length == 24;
			   data_payload.size() == 0;
			  }
			 else if(opcode == 8'h10)
			  {
			   data_pdu_length == 24;
			   data_payload.size() == 0;
			  }
			 else if(opcode == 8'h11)
			  {
			   data_pdu_length == 3;
			   data_payload.size() == 0;
			  }
			 else if(opcode == 8'h12)
			  {
			   data_pdu_length == 1;
			   data_payload.size() == 0;
			  }
			 else if(opcode == 8'h13)
			  {
			   data_pdu_length == 1;
			   data_payload.size() == 0;
			  }
		       }
		      else 
		       {
                        data_pdu_length inside {[0:31]};
                        data_payload.size() == (data_pdu_length-4);
		       }
		     }

// Data rand variable declaration	 
 extern function new(string name = "pkt_seq_item");
 extern function void do_print(ovm_printer printer);
 extern function void do_pack(ovm_packer packer);
 extern function void do_unpack(ovm_packer packer);
 extern function void form_crc();
// extern function calculate_crc(ref bit [7:0] pdu[],ref bit [23:0] conn_crc_init);
 extern function automatic calculate_crc(ref bit [15:0] header, ref bit [7:0] pdu[], ref bit [23:0] conn_crc_init);
endclass: pkt_seq_item

function pkt_seq_item::new(string name = "pkt_seq_item");
 super.new(name);
endfunction

function void pkt_seq_item::do_pack(ovm_packer packer);
 super.do_pack(packer);

 if(pkt_type==0)
  begin
   packer.pack_field(8'b01010101,8);
   packer.pack_field(32'h6B7D9171,32); // Access addr for adv [LSB:MSB]
   frm_adv_pdu_type = {<< {frm_adv_pdu_type}};
   packer.pack_field(frm_adv_pdu_type,4);
   frm_adv_pdu_type = {<< {frm_adv_pdu_type}};   
   packer.pack_field(2'b00,2);
   packer.pack_field(frm_adv_pdu_tx_add,1);
   packer.pack_field(frm_adv_pdu_rx_add,1);
   frm_adv_pdu_length = {<< {frm_adv_pdu_length}};
   packer.pack_field(frm_adv_pdu_length,6);
   frm_adv_pdu_length = {<< {frm_adv_pdu_length}};   
   packer.pack_field(2'b00,2);
   if(frm_adv_pdu_type == 1)
    begin
     frm_adv_addr = {<< {frm_adv_addr}};
     packer.pack_field(frm_adv_addr,48);
     frm_adv_addr = {<< {frm_adv_addr}};     
     frm_conn_init_add = {<< {frm_conn_init_add}};
     packer.pack_field(frm_conn_init_add,48);
     frm_conn_init_add = {<< {frm_conn_init_add}};     
    end
   else if(frm_adv_pdu_type == 3)
    begin
     frm_scan_addr = {<< {frm_scan_addr}};
     packer.pack_field(frm_scan_addr,48);
     frm_scan_addr = {<< {frm_scan_addr}};     
     frm_adv_addr = {<< {frm_adv_addr}};
     packer.pack_field(frm_adv_addr,48);
     frm_adv_addr = {<< {frm_adv_addr}};     
    end
   else if(frm_adv_pdu_type == 0 || frm_adv_pdu_type == 2 || frm_adv_pdu_type == 6)
    begin
     frm_adv_addr = {<< {frm_adv_addr}};
     packer.pack_field(frm_adv_addr,48);
     frm_adv_addr = {<< {frm_adv_addr}};     
     foreach(frm_adv_data[i])
      begin
       frm_adv_data[i] = {<< {frm_adv_data[i]}};
       packer.pack_field(frm_adv_data[i],8);
       frm_adv_data[i] = {<< {frm_adv_data[i]}};       
      end
    end
   else if(frm_adv_pdu_type == 4)
    begin
     frm_adv_addr = {<< {frm_adv_addr}};
     packer.pack_field(frm_adv_addr,48);
     frm_adv_addr = {<< {frm_adv_addr}};     
     foreach(frm_scan_rsp_data[i])
      begin
       frm_scan_rsp_data[i] = {<< {frm_scan_rsp_data[i]}};
       packer.pack_field(frm_scan_rsp_data[i],8);
       frm_scan_rsp_data[i] = {<< {frm_scan_rsp_data[i]}};       
      end
    end
   else if(frm_adv_pdu_type == 5)
    begin
     frm_conn_init_add = {<< {frm_conn_init_add}};
     packer.pack_field(frm_conn_init_add,48);
     frm_conn_init_add = {<< {frm_conn_init_add}};     
     frm_adv_addr = {<< {frm_adv_addr}};
     packer.pack_field(frm_adv_addr,48);
     frm_adv_addr = {<< {frm_adv_addr}};     
     frm_conn_acc_add = {<< {frm_conn_acc_add}};
     packer.pack_field(frm_conn_acc_add,32);
     frm_conn_acc_add = {<< {frm_conn_acc_add}};     
     frm_crc_init = {<< {frm_crc_init}};
     packer.pack_field(frm_crc_init,24);
     frm_crc_init = {<< {frm_crc_init}};     
     frm_winsize = {<< {frm_winsize}};
     packer.pack_field(frm_winsize,8);
     frm_winsize = {<< {frm_winsize}};     
     frm_winoffset = {<< {frm_winoffset}};
     packer.pack_field(frm_winoffset,16);
     frm_winoffset = {<< {frm_winoffset}};     
     frm_interval = {<< {frm_interval}};
     packer.pack_field(frm_interval,16);
     frm_interval = {<< {frm_interval}};
     frm_latency = {<< {frm_latency}};
     packer.pack_field(frm_latency,16);
     frm_latency = {<< {frm_latency}};     
     frm_timeout = {<< {frm_timeout}};
     packer.pack_field(frm_timeout,16);
     frm_timeout = {<< {frm_timeout}};     
     frm_chm = {<< {frm_chm}};
     packer.pack_field(frm_chm,40);
     frm_chm = {<< {frm_chm}};     
     frm_hop = {<< {frm_hop}};
     packer.pack_field(frm_hop,5);
     frm_hop = {<< {frm_hop}};     
     frm_sca = {<< {frm_sca}};
     packer.pack_field(frm_sca,3);
     frm_sca = {<< {frm_sca}};     
    end
   form_crc();  // call seq_item.print() after calling pack method in tx_driver to get calculated value. If called in ble_controller, it shows 0 since form_crc is calculated only in pack method which in turn is called in tx_driver
   packer.pack_field(crc,24);
  end
 else
  begin
   packer.pack_field(preamble,8);
   frm_conn_acc_add = {<< {frm_conn_acc_add}};
   packer.pack_field(frm_conn_acc_add,32);
   frm_conn_acc_add = {<< {frm_conn_acc_add}};

   llid = {<< {llid}};
   packer.pack_field(llid,2);
   llid = {<< {llid}};
   packer.pack_field(nesn,1);
   packer.pack_field(sn,1);
   packer.pack_field(md,1);
   packer.pack_field(3'b000,3);
   data_pdu_length = {<< {data_pdu_length}};
   packer.pack_field(data_pdu_length,5);
   data_pdu_length = {<< {data_pdu_length}};
   packer.pack_field(3'b000,3);

   if(llid == 2'b11)
   begin
   packer.pack_field(opcode,8);
    if (opcode == 8'h00)
     begin
      frm_winsize = {<< {frm_winsize}};
     packer.pack_field(frm_winsize,8);
     frm_winsize = {<< {frm_winsize}};     
     frm_winoffset = {<< {frm_winoffset}};
     packer.pack_field(frm_winoffset,16);
     frm_winoffset = {<< {frm_winoffset}};     
     frm_interval = {<< {frm_interval}};
     packer.pack_field(frm_interval,16);
     frm_interval = {<< {frm_interval}};
     frm_latency = {<< {frm_latency}};
     packer.pack_field(frm_latency,16);
     frm_latency = {<< {frm_latency}};     
     frm_timeout = {<< {frm_timeout}};
     packer.pack_field(frm_timeout,16);
     frm_timeout = {<< {frm_timeout}};
     packer.pack_field(instant,16);
     end
    else if(opcode == 8'h01)
     begin
     frm_chm = {<< {frm_chm}};
     packer.pack_field(frm_chm,40);
     frm_chm = {<< {frm_chm}};
     packer.pack_field(instant,16);
     end
    else if(opcode == 8'h02 || opcode == 8'h0D)
     begin
     packer.pack_field(error_code,8);
     end
    else if(opcode == 8'h03)
     begin
     packer.pack_field(enc_rand,64);
     packer.pack_field(enc_ediv,16);
     packer.pack_field(enc_skd,64);
     packer.pack_field(enc_iv,32);
     end
    else if(opcode == 8'h04)
     begin
     packer.pack_field(enc_skd,64);
     packer.pack_field(enc_iv,32);
     end
    else if(opcode == 8'h08 || opcode == 8'h09 || opcode == 8'h0E)
     begin
     packer.pack_field(feature_set_data,64);     
     end
    else if(opcode == 8'h0C)
     begin
     packer.pack_field(version_vernr,8);     
     packer.pack_field(version_compid,16);     
     packer.pack_field(version_sub_vernr,16);     
     end
    else if(opcode == 8'h0F || opcode == 8'h10)
     begin
     packer.pack_field(frm_interval_min,16);     
     packer.pack_field(frm_interval_max,16);     
     packer.pack_field(frm_latency,16);     
     packer.pack_field(frm_timeout,16);     
     packer.pack_field(conn_param_offset,120);     
     end
    else if(opcode == 8'h11)
     begin
     packer.pack_field(unknown_reject_opcode,8);     
     packer.pack_field(error_code,8);
     end
    else if(opcode == 8'h07)
     begin
     packer.pack_field(unknown_reject_opcode,8);     
     end
    else if((opcode == 8'h05)||(opcode == 8'h06)||(opcode == 8'h0A)||(opcode == 8'h0B)||(opcode == 8'h12)||(opcode == 8'h13));
    else 
     $display("INVALID OPCODE");
   end
   else
   begin
   foreach(data_payload[i])
      begin
       data_payload[i] = {<< {data_payload[i]}};
       packer.pack_field(data_payload[i],8);
       data_payload[i] = {<< {data_payload[i]}};       
      end
   end
    packer.pack_field(mic,32);
    packer.pack_field(crc,24);
  end
 
endfunction

function void pkt_seq_item::do_unpack(ovm_packer packer);
int dummy;

 	preamble = packer.unpack_field(8);
	preamble = {<< {preamble}};
	access_address = packer.unpack_field(32);
	access_address = {<< {access_address}};

	if(access_address=='h8e89bed6)
	begin
		pkt_type = ADV;
		frm_adv_pdu_type = packer.unpack_field(4);
		frm_adv_pdu_type = {<< {frm_adv_pdu_type}};
		dummy = packer.unpack_field(2);
		frm_adv_pdu_tx_add = packer.unpack_field(1);
		frm_adv_pdu_rx_add = packer.unpack_field(1);
		frm_adv_pdu_length = packer.unpack_field(6);
		frm_adv_pdu_length = {<< {frm_adv_pdu_length}};
		dummy = packer.unpack_field(2);

		if(frm_adv_pdu_type == 4'd1)
		begin
			frm_adv_addr = packer.unpack_field(48);
			frm_adv_addr = {<< {frm_adv_addr}};
			source_device_addr = frm_adv_addr;
			frm_conn_init_add = packer.unpack_field(48);
			frm_conn_init_add = {<< {frm_conn_init_add}};
			target_device_addr = frm_conn_init_add;
			frm_adv_data = new [0];
		end
		else if(frm_adv_pdu_type == 4'd3)
		begin
			frm_scan_addr = packer.unpack_field(48);
			frm_scan_addr = {<< {frm_scan_addr}};
			source_device_addr = frm_scan_addr;
			frm_adv_addr = packer.unpack_field(48);
			frm_adv_addr = {<< {frm_adv_addr}};
			target_device_addr = frm_adv_addr;
			frm_adv_data = new [0];
		end
    
		else if(frm_adv_pdu_type == 4'd0 || frm_adv_pdu_type == 4'd2 || frm_adv_pdu_type == 4'd6)
		begin
			frm_adv_addr = packer.unpack_field(48);
			frm_adv_addr = {<< {frm_adv_addr}};
			source_device_addr = frm_adv_addr;
			frm_adv_data = new [frm_adv_pdu_length - 6];
			foreach(frm_adv_data[i])
			begin
				frm_adv_data[i] = packer.unpack_field(8);
				frm_adv_data[i] = {<< {frm_adv_data[i]}};
			end
		end
		else if(frm_adv_pdu_type == 4)
		begin
			frm_adv_addr = packer.unpack_field(48);
			frm_adv_addr = {<< {frm_adv_addr}};
			source_device_addr = frm_adv_addr;
			frm_scan_rsp_data = new [frm_adv_pdu_length - 6];
			foreach(frm_scan_rsp_data[i])
			begin
				frm_scan_rsp_data[i] = packer.unpack_field(8);
				frm_scan_rsp_data[i] = {<< {frm_scan_rsp_data[i]}};
			end
		end
		else if(frm_adv_pdu_type == 5)
		begin
			frm_conn_init_add = packer.unpack_field(48);
			frm_conn_init_add = {<< {frm_conn_init_add}};
			source_device_addr = frm_conn_init_add;
			frm_adv_addr = packer.unpack_field(48);
			frm_adv_addr = {<< {frm_adv_addr}};
			target_device_addr = frm_adv_addr;
			frm_conn_acc_add = packer.unpack_field(32);
			frm_conn_acc_add = {<< {frm_conn_acc_add}};
			frm_crc_init = packer.unpack_field(24);
			frm_crc_init = {<< {frm_crc_init}};
			frm_winsize = packer.unpack_field(8);
			frm_winsize = {<< {frm_winsize}};
			frm_winoffset = packer.unpack_field(16);
			frm_winoffset = {<< {frm_winoffset}};
			frm_interval = packer.unpack_field(16);
			frm_interval = {<< {frm_interval}};
			frm_latency = packer.unpack_field(16);
			frm_latency = {<< {frm_latency}};
			frm_timeout = packer.unpack_field(16);
			frm_timeout = {<< {frm_timeout}};
			frm_chm = packer.unpack_field(40);
			frm_chm = {<< {frm_chm}};
			frm_hop = packer.unpack_field(5);
			frm_hop = {<< {frm_hop}};
			frm_sca = packer.unpack_field(3);
			frm_sca = {<< {frm_sca}};
		end
		crc = packer.unpack_field(24);
	end

//Data
	else
	begin
		pkt_type = DATA;
		llid = packer.unpack_field(2);
		llid = {<< {llid}};
		nesn = packer.unpack_field(1);
		sn = packer.unpack_field(1);
		md = packer.unpack_field(1);
		dummy = packer.unpack_field(3);
		data_pdu_length = packer.unpack_field(5);
		data_pdu_length = {<< {data_pdu_length}};
		dummy = packer.unpack_field(3);

		if(llid == 2'b11)
		begin
			opcode = packer.unpack_field(8);
	 		opcode = {<< {opcode}};
			if (opcode == 8'h00)
			begin
				frm_winsize = packer.unpack_field(8);
				frm_winsize = {<< {frm_winsize}};
				frm_winoffset = packer.unpack_field(16);
				frm_winoffset = {<< {frm_winoffset}};
				frm_interval = packer.unpack_field(16);
				frm_interval = {<< {frm_interval}};
				frm_latency = packer.unpack_field(16);
				frm_latency = {<< {frm_latency}};
				frm_timeout = packer.unpack_field(16);
				frm_timeout = {<< {frm_timeout}};
				instant = packer.unpack_field(16);
			end
			else if(opcode == 8'h01)
			begin
				frm_chm = packer.unpack_field(40);
				frm_chm = {<< {frm_chm}};
				instant = packer.unpack_field(16);
			end
			else if(opcode == 8'h02 || opcode == 8'h0D)
				error_code = packer.unpack_field(8);
			else if(opcode == 8'h03)
			begin
				enc_rand = packer.unpack_field(64);
				enc_ediv = packer.unpack_field(16);
				enc_skd = packer.unpack_field(64);
				enc_iv = packer.unpack_field(32);
			end
			else if(opcode == 8'h04)
			begin
				enc_skd = packer.unpack_field(64);
				enc_iv = packer.unpack_field(32);
			end
			else if(opcode == 8'h08 || opcode == 8'h09 || opcode == 8'h0E)
				feature_set_data = packer.unpack_field(64);
			else if(opcode == 8'h0C)
			begin
				version_vernr = packer.unpack_field(8);
				version_compid = packer.unpack_field(16);
				version_sub_vernr = packer.unpack_field(16);
			end
			else if(opcode == 8'h0F || opcode == 8'h10)
			begin
				frm_interval_min = packer.unpack_field(16);
				frm_interval_max = packer.unpack_field(16);
				frm_latency = packer.unpack_field(16);
				frm_timeout = packer.unpack_field(16);
				conn_param_offset = packer.unpack_field(120);
			end
			else if(opcode == 8'h11)
			begin
				unknown_reject_opcode = packer.unpack_field(8);
				error_code = packer.unpack_field(8);
			end
			else if(opcode == 8'h07)
				unknown_reject_opcode = packer.unpack_field(8);
	    else if((opcode == 8'h05)||(opcode == 8'h06)||(opcode == 8'h0A)||(opcode == 8'h0B)||(opcode == 8'h12)||(opcode == 8'h13));
			else
				$display("INVALID OPCODE");
		end
		else if(llid == 2'b10 && llid == 2'b01)
		begin
			if (enc)
				data_payload = new [data_pdu_length - 4];
			else
				data_payload = new [data_pdu_length];
			foreach(data_payload[i])
			begin
				data_payload[i] = packer.unpack_field(8);
				data_payload[i] = {<< {data_payload[i]}};
			end
		end
		if (enc)
		begin
			mic = packer.unpack_field(32);
	 		mic = {<< {mic}};
	 		mic = {<< byte{mic}};
		end
		crc = packer.unpack_field(24);
	end
endfunction

function void pkt_seq_item::do_print(ovm_printer printer);
  super.do_print(printer);

  printer.print_field("pkt_type", this.pkt_type, 1, OVM_BIN);
  if(pkt_type == 0)
  begin
	  printer.print_field("adv_pdu_type", this.frm_adv_pdu_type, 4, OVM_BIN);
	  printer.print_field("txadd", this.frm_adv_pdu_tx_add, 1, OVM_BIN);
	  printer.print_field("rxadd", this.frm_adv_pdu_rx_add, 1, OVM_BIN);
	  printer.print_field("length", this.frm_adv_pdu_length, 6, OVM_UNSIGNED); 
	  printer.print_field("source_device_addr", this.source_device_addr, 48, OVM_HEX);
	  printer.print_field("target_device_addr", this.target_device_addr, 48, OVM_HEX);
	
		if (frm_adv_pdu_type == 4'd5)
		begin
		  printer.print_field("conn_access_address", this.frm_conn_acc_add, 32, OVM_HEX);
		  printer.print_field("crc_init", this.frm_crc_init, 24, OVM_HEX);
		  printer.print_field("winsize", this.frm_winsize, 8, OVM_HEX);
		  printer.print_field("winoffset", this.frm_winoffset, 16, OVM_HEX);
		  printer.print_field("interval", this.frm_interval, 16, OVM_HEX);
		  printer.print_field("latency", this.frm_latency, 16, OVM_HEX);
		  printer.print_field("timeout", this.frm_timeout, 16, OVM_HEX);
		  printer.print_field("chm", this.frm_chm, 40, OVM_HEX);
		  printer.print_field("hop", this.frm_hop, 5, OVM_HEX);
		  printer.print_field("sca", this.frm_sca, 3, OVM_HEX);
		end
	
		if ((frm_adv_pdu_type == 4'd0) || (frm_adv_pdu_type == 4'd2) || (frm_adv_pdu_type == 4'd6))
		begin
			foreach (frm_adv_data[i])
			  printer.print_field($sformatf("adv_data[%0d]",i), this.frm_adv_data[i], 8, OVM_HEX);
		end
		if (frm_adv_pdu_type == 4'd4)
		begin
			foreach (frm_scan_rsp_data[i])
			  printer.print_field($sformatf("scan_rsp_data[%0d]",i), this.frm_scan_rsp_data[i], 8, OVM_HEX);
		end
  end

  else
  begin
  printer.print_field("preamble", this.preamble, 8, OVM_BIN);
  printer.print_field("access_address", this.access_address, 32, OVM_HEX);
  printer.print_field("llid", this.llid, 2, OVM_BIN);
  printer.print_field("nesn", this.nesn, 1, OVM_BIN);
  printer.print_field("sn", this.sn, 1, OVM_BIN);
  printer.print_field("md", this.md, 1, OVM_BIN);
  printer.print_field("length", this.data_pdu_length, 5, OVM_UNSIGNED);

	if (llid == 2'b10 && llid == 2'b01)
	begin
	foreach (data_payload[i])
  printer.print_field($sformatf("data_payload[%0d]",i), this.data_payload[i], 8, OVM_HEX);
	end
	else if(llid == 2'b11)
	begin
	  printer.print_field("opcode", this.opcode, 8, OVM_HEX);

		if (opcode == 8'h00)
		begin
		  printer.print_field("winsize", this.frm_winsize, 8, OVM_HEX);
		  printer.print_field("winoffset", this.frm_winoffset, 16, OVM_HEX);
		  printer.print_field("interval", this.frm_interval, 16, OVM_HEX);
		  printer.print_field("latency", this.frm_latency, 16, OVM_HEX);
		  printer.print_field("timeout", this.frm_timeout, 16, OVM_HEX);
		  printer.print_field("instant", this.instant, 16, OVM_HEX);
		end
		if ((opcode == 8'h0f) || (opcode == 8'h10))
		begin
	  	printer.print_field("interval_min", this.frm_interval_min, 16, OVM_HEX);
	  	printer.print_field("interval_max", this.frm_interval_max, 16, OVM_HEX);
	  	printer.print_field("latency", this.frm_latency, 16, OVM_HEX);
	  	printer.print_field("timeout", this.frm_timeout, 16, OVM_HEX);
	  	printer.print_field("conn_param_offset", this.conn_param_offset, 120, OVM_HEX);
		end
	  if (opcode == 8'h01)
		begin
			printer.print_field("chm", this.frm_chm, 40, OVM_HEX);
	  	printer.print_field("instant", this.instant, 16, OVM_HEX);
		end
		if ((opcode == 8'h03) || (opcode == 8'h04))
		begin
	  	if (opcode == 8'h03)
			begin
	  		printer.print_field("rand", this.enc_rand, 64, OVM_HEX);
	  		printer.print_field("ediv", this.enc_ediv, 16, OVM_HEX);
			end
	  	printer.print_field("skd", this.enc_skd, 64, OVM_HEX);
	  	printer.print_field("iv", this.enc_iv, 32, OVM_HEX);
		end
		if ((opcode == 8'h08) || (opcode == 8'h09) || (opcode == 8'h0e))
		begin
	  	printer.print_field("feature_set", this.feature_set_data, 64, OVM_HEX);
	  	printer.print_field("version_no", this.version_vernr, 8, OVM_HEX);
	  	printer.print_field("company_id", this.version_compid, 16, OVM_HEX);
	  	printer.print_field("sub_version_no", this.version_sub_vernr, 16, OVM_HEX);
		end
		if ((opcode == 8'h0d) || (opcode == 8'h11))
		  printer.print_field("error_code", this.error_code, 8, OVM_HEX);
		if ((opcode == 8'h07) || (opcode == 8'h11))
		  printer.print_field("unknown_reject_opcode", this.unknown_reject_opcode, 8, OVM_HEX);
  end
	if (enc)
	  printer.print_field("mic", this.mic, 32, OVM_HEX);
  end
  printer.print_field("crc", this.crc, 24, OVM_HEX);
endfunction

function void pkt_seq_item::form_crc();


   if(this.pkt_type == 0)
   begin
      
      adv_header = {2'b00,frm_adv_pdu_length,frm_adv_pdu_rx_add,frm_adv_pdu_tx_add,2'b00,frm_adv_pdu_type};
      crc = 'h555555;
      calculate_crc(adv_header, frm_adv_data, crc);
      
   end

   else
   begin
      
      data_header = {llid,nesn,sn,md,3'b000,data_pdu_length,3'b000};
      //calculate_crc(data_header, data_payload, crc);

   end

endfunction 

// Cannot assign packed data type to unpacket data type

function automatic pkt_seq_item::calculate_crc(ref bit [15:0] header, ref bit [7:0] pdu[], ref bit [23:0] conn_crc_init);
      
      // 24th bit has to be transmitted into AIR first

      for(int i=0;i<16;i++)
      begin
         
	 bit lfsr_value = conn_crc_init[23] ^ header[i];
	     conn_crc_init[23] = conn_crc_init[22];
	     conn_crc_init[22] = conn_crc_init[21];
	     conn_crc_init[21] = conn_crc_init[20];
	     conn_crc_init[20] = conn_crc_init[19];
	     conn_crc_init[19] = conn_crc_init[18];
	     conn_crc_init[18] = conn_crc_init[17];
	     conn_crc_init[17] = conn_crc_init[16];
	     conn_crc_init[16] = conn_crc_init[15];
	     conn_crc_init[15] = conn_crc_init[14];
	     conn_crc_init[14] = conn_crc_init[13];
	     conn_crc_init[13] = conn_crc_init[12];
	     conn_crc_init[12] = conn_crc_init[11];
	     conn_crc_init[11] = conn_crc_init[10];
	     conn_crc_init[10] = conn_crc_init[9] ^ lfsr_value;
	     conn_crc_init[9]  = conn_crc_init[8] ^ lfsr_value ;
	     conn_crc_init[8]  = conn_crc_init[7];
	     conn_crc_init[7]  = conn_crc_init[6];
	     conn_crc_init[6]  = conn_crc_init[5] ^ lfsr_value;
	     conn_crc_init[5]  = conn_crc_init[4];
	     conn_crc_init[4]  = conn_crc_init[3] ^ lfsr_value;
	     conn_crc_init[3]  = conn_crc_init[2] ^ lfsr_value;
	     conn_crc_init[2]  = conn_crc_init[1];
	     conn_crc_init[1]  = conn_crc_init[0] ^ lfsr_value;
	     conn_crc_init[0]  = lfsr_value;

      end

      if(adv_type != 'd1)
      begin
        
	for(int i=0;i<48;i++)
           begin
         
	     bit lfsr_value = conn_crc_init[23] ^ frm_adv_addr[i];
	     conn_crc_init[23] = conn_crc_init[22];
	     conn_crc_init[22] = conn_crc_init[21];
	     conn_crc_init[21] = conn_crc_init[20];
	     conn_crc_init[20] = conn_crc_init[19];
	     conn_crc_init[19] = conn_crc_init[18];
	     conn_crc_init[18] = conn_crc_init[17];
	     conn_crc_init[17] = conn_crc_init[16];
	     conn_crc_init[16] = conn_crc_init[15];
	     conn_crc_init[15] = conn_crc_init[14];
	     conn_crc_init[14] = conn_crc_init[13];
	     conn_crc_init[13] = conn_crc_init[12];
	     conn_crc_init[12] = conn_crc_init[11];
	     conn_crc_init[11] = conn_crc_init[10];
	     conn_crc_init[10] = conn_crc_init[9] ^ lfsr_value;
	     conn_crc_init[9]  = conn_crc_init[8] ^ lfsr_value ;
	     conn_crc_init[8]  = conn_crc_init[7];
	     conn_crc_init[7]  = conn_crc_init[6];
	     conn_crc_init[6]  = conn_crc_init[5] ^ lfsr_value;
	     conn_crc_init[5]  = conn_crc_init[4];
	     conn_crc_init[4]  = conn_crc_init[3] ^ lfsr_value;
	     conn_crc_init[3]  = conn_crc_init[2] ^ lfsr_value;
	     conn_crc_init[2]  = conn_crc_init[1];
	     conn_crc_init[1]  = conn_crc_init[0] ^ lfsr_value;
	     conn_crc_init[0]  = lfsr_value;
	  end

	
	foreach (pdu[j])
         begin
            
            for(int i=0;i<8;i++)
            begin
            
                bit lfsr_value = conn_crc_init[23] ^ pdu[j][i];
                conn_crc_init[23] = conn_crc_init[22];
                conn_crc_init[22] = conn_crc_init[21];
                conn_crc_init[21] = conn_crc_init[20];
                conn_crc_init[20] = conn_crc_init[19];
                conn_crc_init[19] = conn_crc_init[18];
                conn_crc_init[18] = conn_crc_init[17];
                conn_crc_init[17] = conn_crc_init[16];
                conn_crc_init[16] = conn_crc_init[15];
                conn_crc_init[15] = conn_crc_init[14];
                conn_crc_init[14] = conn_crc_init[13];
                conn_crc_init[13] = conn_crc_init[12];
                conn_crc_init[12] = conn_crc_init[11];
                conn_crc_init[11] = conn_crc_init[10];
                conn_crc_init[10] = conn_crc_init[9] ^ lfsr_value;
                conn_crc_init[9]  = conn_crc_init[8] ^ lfsr_value ;
                conn_crc_init[8]  = conn_crc_init[7];
                conn_crc_init[7]  = conn_crc_init[6];
                conn_crc_init[6]  = conn_crc_init[5] ^ lfsr_value;
                conn_crc_init[5]  = conn_crc_init[4];
                conn_crc_init[4]  = conn_crc_init[3] ^ lfsr_value;
                conn_crc_init[3]  = conn_crc_init[2] ^ lfsr_value;
                conn_crc_init[2]  = conn_crc_init[1];
                conn_crc_init[1]  = conn_crc_init[0] ^ lfsr_value;
                conn_crc_init[0]  = lfsr_value;

            end


         end
  
     end
  
   
   if(adv_type == 'd1)
      begin
         
	 for(int i=0;i<48;i++)
           begin
         
	     bit lfsr_value = conn_crc_init[23] ^ frm_adv_addr[i];
	     conn_crc_init[23] = conn_crc_init[22];
	     conn_crc_init[22] = conn_crc_init[21];
	     conn_crc_init[21] = conn_crc_init[20];
	     conn_crc_init[20] = conn_crc_init[19];
	     conn_crc_init[19] = conn_crc_init[18];
	     conn_crc_init[18] = conn_crc_init[17];
	     conn_crc_init[17] = conn_crc_init[16];
	     conn_crc_init[16] = conn_crc_init[15];
	     conn_crc_init[15] = conn_crc_init[14];
	     conn_crc_init[14] = conn_crc_init[13];
	     conn_crc_init[13] = conn_crc_init[12];
	     conn_crc_init[12] = conn_crc_init[11];
	     conn_crc_init[11] = conn_crc_init[10];
	     conn_crc_init[10] = conn_crc_init[9] ^ lfsr_value;
	     conn_crc_init[9]  = conn_crc_init[8] ^ lfsr_value ;
	     conn_crc_init[8]  = conn_crc_init[7];
	     conn_crc_init[7]  = conn_crc_init[6];
	     conn_crc_init[6]  = conn_crc_init[5] ^ lfsr_value;
	     conn_crc_init[5]  = conn_crc_init[4];
	     conn_crc_init[4]  = conn_crc_init[3] ^ lfsr_value;
	     conn_crc_init[3]  = conn_crc_init[2] ^ lfsr_value;
	     conn_crc_init[2]  = conn_crc_init[1];
	     conn_crc_init[1]  = conn_crc_init[0] ^ lfsr_value;
	     conn_crc_init[0]  = lfsr_value;
	  end

      for(int i=0;i<48;i++)
           begin
         
	     bit lfsr_value = conn_crc_init[23] ^ frm_conn_init_add[i];
	     conn_crc_init[23] = conn_crc_init[22];
	     conn_crc_init[22] = conn_crc_init[21];
	     conn_crc_init[21] = conn_crc_init[20];
	     conn_crc_init[20] = conn_crc_init[19];
	     conn_crc_init[19] = conn_crc_init[18];
	     conn_crc_init[18] = conn_crc_init[17];
	     conn_crc_init[17] = conn_crc_init[16];
	     conn_crc_init[16] = conn_crc_init[15];
	     conn_crc_init[15] = conn_crc_init[14];
	     conn_crc_init[14] = conn_crc_init[13];
	     conn_crc_init[13] = conn_crc_init[12];
	     conn_crc_init[12] = conn_crc_init[11];
	     conn_crc_init[11] = conn_crc_init[10];
	     conn_crc_init[10] = conn_crc_init[9] ^ lfsr_value;
	     conn_crc_init[9]  = conn_crc_init[8] ^ lfsr_value ;
	     conn_crc_init[8]  = conn_crc_init[7];
	     conn_crc_init[7]  = conn_crc_init[6];
	     conn_crc_init[6]  = conn_crc_init[5] ^ lfsr_value;
	     conn_crc_init[5]  = conn_crc_init[4];
	     conn_crc_init[4]  = conn_crc_init[3] ^ lfsr_value;
	     conn_crc_init[3]  = conn_crc_init[2] ^ lfsr_value;
	     conn_crc_init[2]  = conn_crc_init[1];
	     conn_crc_init[1]  = conn_crc_init[0] ^ lfsr_value;
	     conn_crc_init[0]  = lfsr_value;
	  end
         
           
      end
 
      

endfunction
