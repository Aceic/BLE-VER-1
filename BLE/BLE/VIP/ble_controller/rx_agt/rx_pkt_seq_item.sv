class rx_pkt_seq_item extends ovm_sequence_item;
	`ovm_object_utils(rx_pkt_seq_item)

	typedef enum {ADV,DATA} bt_pkt_type;
	typedef enum  {ADV_IND,ADV_DIRECT_IND,ADV_NONCONN_IND,SCAN_REQ,SCAN_RSP,CONNECT_REQ,ADV_SCAN_IND} adv_pdu_type;

	rand bit [7:0]  frm_scan_rsp_data[];
	rand bit [7:0]  frm_adv_data[];
  rand bit [7:0] 	data_payload[];

	rand bit [0:7]	preamble;
			 bit [0:31] access_address;
	rand bit [23:0] crc;
			 bit [5:0]	channel_index;
	rand bit				enc;

	rand bit [3:0]	frm_adv_pdu_type;
	rand bit				frm_adv_pdu_tx_add;
	rand bit				frm_adv_pdu_rx_add;
	rand bit [5:0]	frm_adv_pdu_length;
	rand bit [47:0] source_device_addr;
	rand bit [47:0] target_device_addr;
	rand bit [1:0]  rand_dev_addr_type;

	rand bit [31:0] frm_conn_acc_add;
	rand bit [23:0] frm_crc_init;
	rand bit [7:0]  frm_winsize;
	rand bit [15:0] frm_winoffset;
	rand bit [15:0] frm_interval;
	rand bit [15:0] frm_interval_min;
	rand bit [15:0] frm_interval_max;
	rand bit [15:0] frm_latency;
	rand bit [15:0] frm_timeout;
	rand bit [39:0] frm_chm;
	rand bit [4:0]  frm_hop;
	rand bit [2:0]  frm_sca;

	rand bit [1:0] 	llid;
	rand bit 				nesn;
	rand bit 				sn;
	rand bit				md;
	rand bit [4:0] 	data_pdu_length;
	rand bit [31:0] mic;

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

  rand bt_pkt_type pkt_type;
 	rand adv_pdu_type adv_type;


// constraint 
	constraint PREAMBLE {if (access_address[0])
												preamble==8'b10101010;
											else
												preamble==8'b01010101;
	}
  constraint ADV_PDU_TYPE {solve adv_type before frm_adv_pdu_type;
                          (adv_type == ADV_IND) 				-> frm_adv_pdu_type == 4'b0000;
													(adv_type == ADV_DIRECT_IND) 	-> frm_adv_pdu_type == 4'b0001;
													(adv_type == ADV_NONCONN_IND)	-> frm_adv_pdu_type == 4'b0010;
                          (adv_type == SCAN_REQ)       	-> frm_adv_pdu_type == 4'b0011;
                          (adv_type == SCAN_RSP)       	-> frm_adv_pdu_type == 4'b0100;
													(adv_type == CONNECT_REQ)    	-> frm_adv_pdu_type == 4'b0101;
													(adv_type == ADV_SCAN_IND)   	-> frm_adv_pdu_type == 4'b0110;
	}
	constraint AA {
		if(pkt_type == 0) {
			if(adv_type == 5) {
				foreach(frm_conn_acc_add[i])
					if(i<27)
						((frm_conn_acc_add[i] && frm_conn_acc_add[i+1] && frm_conn_acc_add[i+2] && frm_conn_acc_add[i+3] && frm_conn_acc_add[i+4] && frm_conn_acc_add[i+5]) != 1);
	
				foreach(frm_conn_acc_add[j])
					if(j<27)
						((frm_conn_acc_add[j] || frm_conn_acc_add[j+1] || frm_conn_acc_add[j+2] || frm_conn_acc_add[j+3] || frm_conn_acc_add[j+4] || frm_conn_acc_add[j+5]) != 0);
	
		    frm_conn_acc_add != 32'h8E89BED6;
				frm_conn_acc_add[31:24] != frm_conn_acc_add[23:16]; frm_conn_acc_add[23:16] != frm_conn_acc_add[15:8]; frm_conn_acc_add[15:8] != frm_conn_acc_add[7:0];
	
				$countones(frm_conn_acc_add ^ (32'h8E89BED6))>1;
				($countones(frm_conn_acc_add ^ (frm_conn_acc_add*2)) -(1&frm_conn_acc_add[0]))<25;
	    	($countones(frm_conn_acc_add[31:26] ^ (frm_conn_acc_add[31:26]*2))-(1&frm_conn_acc_add[26]))>1;
		  }
	  }
	}	
	constraint DEVICE_ADDRESS {
		rand_dev_addr_type!=2'b11;
		solve rand_dev_addr_type before source_device_addr;
		if(frm_adv_pdu_tx_add==1) { 
			if(rand_dev_addr_type==2'b00) {
				source_device_addr[47:46]==2'b11;
				$countones(source_device_addr[45:0])!=46;
				$countones(source_device_addr[45:0])!=0;
			}
			else if(rand_dev_addr_type==2'b01) {
				source_device_addr[47:46]==2'b00;
				$countones(source_device_addr[45:0])!=46;
				$countones(source_device_addr[45:0])!=0;
			}
		}
	}	
 	constraint ADV_LENGTH {
		solve adv_type before frm_adv_pdu_length;
		solve frm_adv_pdu_length before frm_adv_data; 
		solve frm_adv_pdu_length before frm_scan_rsp_data;
		solve adv_type before frm_adv_data; 

		if(adv_type==1 || adv_type == 3) {
			frm_adv_pdu_length == 12;
			frm_adv_data.size() == 'd0;
			frm_scan_rsp_data.size() == 'd0;
		}
		else if(adv_type == 5) {
			frm_adv_pdu_length == 'd34;
			frm_adv_data.size() == 'd0;
			frm_scan_rsp_data.size() == 'd0;
		}
		else if(adv_type == 0 || adv_type ==2 || adv_type == 4 || adv_type == 6) {
			frm_adv_pdu_length inside {[6:37]};
			frm_adv_data.size() == (frm_adv_pdu_length - 6);
			frm_scan_rsp_data.size() == (frm_adv_pdu_length - 6);
		}
	}
	constraint LLID {llid != 2'b00;}
	constraint DT_LENGTH {
		solve data_pdu_length before data_payload;
		solve opcode before data_pdu_length;
		if(llid == 2'b10) {
			data_pdu_length != 0;
			if (enc) {
				data_pdu_length inside {[1:31]};
				data_payload.size() == (data_pdu_length-4);
			}
			else {
				data_pdu_length inside {[1:27]};
				data_payload.size() == (data_pdu_length);
			}
		}
		else if(llid == 2'b11) {
			opcode inside {[0:19]};
			if(opcode == 8'h00) {
				data_pdu_length == 12;
			  data_payload.size() == 0;
			}
			else if(opcode == 8'h01) {
				data_pdu_length == 8;
			  data_payload.size() == 0;
			}
      else if(opcode == 8'h02) {
			  data_pdu_length == 2;
			  data_payload.size() == 0;
			}
			else if(opcode == 8'h03) {
				data_pdu_length == 23;
			  data_payload.size() == 0;
			}
			else if(opcode == 8'h04) {
			  data_pdu_length == 13;
			  data_payload.size() == 0;
			}
			else if(opcode == 8'h05) {
			  data_pdu_length == 1;
			  data_payload.size() == 0;
			}
			else if(opcode == 8'h06) {
			  data_pdu_length == 1;
			  data_payload.size() == 0;
			}
			else if(opcode == 8'h07) {
			  data_pdu_length == 2;
			  data_payload.size() == 0;
			}
			else if(opcode == 8'h08) {
			  data_pdu_length == 9;
			  data_payload.size() == 0;
			}
			else if(opcode == 8'h09) {
			  data_pdu_length == 9;
			  data_payload.size() == 0;
			}
			else if(opcode == 8'h0A) {
			  data_pdu_length == 1;
			  data_payload.size() == 0;
			}
			else if(opcode == 8'h0B) {
			  data_pdu_length == 1;
			  data_payload.size() == 0;
		 	}
		 	else if(opcode == 8'h0C) {
		  	data_pdu_length == 6;
		   	data_payload.size() == 0;
			}
		 	else if(opcode == 8'h0D) {
		   	data_pdu_length == 2;
		   	data_payload.size() == 0;
		 	}
		 	else if(opcode == 8'h0E) {
		   	data_pdu_length == 9;
		   	data_payload.size() == 0;
		 	}
		 	else if(opcode == 8'h0F) {
		   	data_pdu_length == 24;
		   	data_payload.size() == 0;
		 	}
		 	else if(opcode == 8'h10) {
		   	data_pdu_length == 24;
		   	data_payload.size() == 0;
		 	}
		 	else if(opcode == 8'h11) {
		   	data_pdu_length == 3;
		   	data_payload.size() == 0;
		 	}
		 	else if(opcode == 8'h12) {
		   	data_pdu_length == 1;
		   	data_payload.size() == 0;
		 	}
		 	else if(opcode == 8'h13) {
		   	data_pdu_length == 1;
		   	data_payload.size() == 0;
		 	}
		}
		else {
			if (enc) {
				data_pdu_length inside {[1:31]};
			  data_payload.size() == (data_pdu_length-4);
			}
			else {
				data_pdu_length inside {[1:27]};
      	data_payload.size() == (data_pdu_length);
			}
		}
	}

	extern function new(string name = "rx_pkt_seq_item");
	extern function void do_print(ovm_printer printer);
	extern function void do_pack(ovm_packer packer);
	extern function void do_unpack(ovm_packer packer);
endclass: rx_pkt_seq_item

function rx_pkt_seq_item::new(string name = "rx_pkt_seq_item");
	super.new(name);
endfunction

function void rx_pkt_seq_item::do_pack(ovm_packer packer);
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
   if ((adv_type == 1) || (adv_type == 3))
    begin
     source_device_addr = {<< {source_device_addr}};
     packer.pack_field(source_device_addr,48);
     source_device_addr = {<< {source_device_addr}};     
     target_device_addr = {<< {target_device_addr}};
     packer.pack_field(target_device_addr,48);
     target_device_addr = {<< {target_device_addr}};     
    end
   else if(adv_type == 0 || adv_type == 2 || adv_type == 6)
    begin
     source_device_addr = {<< {source_device_addr}};
     packer.pack_field(source_device_addr,48);
     source_device_addr = {<< {source_device_addr}};     
     foreach(frm_adv_data[i])
      begin
       frm_adv_data[i] = {<< {frm_adv_data[i]}};
       packer.pack_field(frm_adv_data[i],8);
       frm_adv_data[i] = {<< {frm_adv_data[i]}};       
      end
    end
   else if(adv_type == 4)
    begin
     source_device_addr = {<< {source_device_addr}};
     packer.pack_field(source_device_addr,48);
     source_device_addr = {<< {source_device_addr}};     
     foreach(frm_scan_rsp_data[i])
      begin
       frm_scan_rsp_data[i] = {<< {frm_scan_rsp_data[i]}};
       packer.pack_field(frm_scan_rsp_data[i],8);
       frm_scan_rsp_data[i] = {<< {frm_scan_rsp_data[i]}};       
      end
    end
   else if(adv_type == 5)
    begin
     source_device_addr = {<< {source_device_addr}};
     packer.pack_field(source_device_addr,48);
     source_device_addr = {<< {source_device_addr}};     
     target_device_addr = {<< {target_device_addr}};
     packer.pack_field(target_device_addr,48);
     target_device_addr = {<< {target_device_addr}};     
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
   packer.pack_field(crc,24);
  end
 else
  begin
   packer.pack_field(preamble,8);
   access_address = {<< {access_address}};
   packer.pack_field(access_address,32);
   access_address = {<< {access_address}};

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
	if (enc)
    packer.pack_field(mic,32);
  packer.pack_field(crc,24);
  end
 
endfunction

// Unpack method by sundeep 
function void rx_pkt_seq_item::do_unpack(ovm_packer packer);
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

		if((frm_adv_pdu_type == 1) || (frm_adv_pdu_type == 3))
		begin
			target_device_addr = packer.unpack_field(48);
			target_device_addr = {<< {target_device_addr}};     
			source_device_addr = packer.unpack_field(48);
			source_device_addr = {<< {source_device_addr}};     
		end
		else if(frm_adv_pdu_type == 0 || frm_adv_pdu_type == 2 || frm_adv_pdu_type == 6)
		begin
			target_device_addr = packer.unpack_field(48);
			target_device_addr = {<< {target_device_addr}};     
			frm_adv_data = new [frm_adv_pdu_length - 6];
			foreach(frm_adv_data[i])
			begin
				frm_adv_data[i] = packer.unpack_field(8);
				frm_adv_data[i] = {<< {frm_adv_data[i]}};
			end
		end
		else if(frm_adv_pdu_type == 4)
		begin
			target_device_addr = packer.unpack_field(48);
			target_device_addr = {<< {target_device_addr}};     
			frm_scan_rsp_data = new [frm_adv_pdu_length - 6];
			foreach(frm_scan_rsp_data[i])
			begin
				frm_scan_rsp_data[i] = packer.unpack_field(8);
				frm_scan_rsp_data[i] = {<< {frm_scan_rsp_data[i]}};
			end
		end
		else if(frm_adv_pdu_type == 5)
		begin
			target_device_addr = packer.unpack_field(48);
			target_device_addr = {<< {target_device_addr}};     
			source_device_addr = packer.unpack_field(48);
			source_device_addr = {<< {source_device_addr}};     
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
		adv_type = adv_pdu_type'(frm_adv_pdu_type);
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
			else
				$display("INVALID OPCODE");
		end
		else
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
			mic = packer.unpack_field(32);
		crc = packer.unpack_field(24);
	end
endfunction

function void rx_pkt_seq_item::do_print(ovm_printer printer);
  super.do_print(printer);

  printer.print_field("pkt_type", this.pkt_type, 1, OVM_BIN);
  if(pkt_type == ADV)
  begin
	  printer.print_field("adv_pdu_type", this.frm_adv_pdu_type, 4, OVM_BIN);
	  printer.print_field("txadd", this.frm_adv_pdu_tx_add, 1, OVM_BIN);
	  printer.print_field("rxadd", this.frm_adv_pdu_rx_add, 1, OVM_BIN);
	  printer.print_field("length", this.frm_adv_pdu_length, 6, OVM_UNSIGNED); 
	  printer.print_field("source_device_addr", this.source_device_addr, 48, OVM_HEX);
	  printer.print_field("target_device_addr", this.target_device_addr, 48, OVM_HEX);
	
		if (adv_type == CONNECT_REQ)
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
	
		if ((adv_type == ADV_IND) || (adv_type == ADV_NONCONN_IND) || (adv_type == ADV_SCAN_IND))
		begin
			foreach (frm_adv_data[i])
			  printer.print_field($sformatf("adv_data[%0d]",i), this.frm_adv_data[i], 8, OVM_HEX);
		end
		if (adv_type == SCAN_RSP)
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

	if (llid != 2'b11)
	begin
	foreach (data_payload[i])
  printer.print_field($sformatf("data_payload[%0d]",i), this.data_payload[i], 8, OVM_HEX);
	end
	else
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
	  printer.print_field("MIC", this.mic, 32, OVM_HEX);
  end
  printer.print_field("CRC", this.crc, 24, OVM_HEX);
endfunction
