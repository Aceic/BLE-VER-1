
class seq_item extends ovm_sequence_item;

 `ovm_object_utils(seq_item)


  // Common BT packet Variables

	rand bit [7:0]  preamble;             // STARTING  OF PACKET
	rand bit [31:0] access_address;       // 
  
  // Adv_Header_field.
  typedef enum {ADV,DATA} bt_pkt_type;
  rand bt_pkt_type    pkt_type;     // data or adv packet.
  typedef enum  {ADV_IND,ADV_DIRECT_IND,ADV_NONCONN_IND,SCAN_REQ,SCAN_RSP,CONNECT_REQ,ADV_SCAN_IND} adv_pdu_type;



 	rand adv_pdu_type   adv_type;     // pdu type



 	rand bit[3:0]   adv_pdu;     // pdu type

  typedef enum {SMALL,MEDIUM,LARGE} len;
  rand len length;
  
  rand bit        adv_pdu_txadd;               // Indicates whether source device addr is public or random
	rand bit        adv_pdu_rxadd;               // Indicates whether target device addr is public or random
  rand bit [5:0]  adv_pdu_length;  // length field for ADV packets is 6 bits

  rand bit [47:0] source_device_addr;   // To store source device address
	rand bit [47:0] target_device_addr;   // To store target device address
	rand bit [1:0]  rand_dev_addr_type;   // For random device address which can be of static type (00)
                                        //  Non resolvable private address type(01) or 
				                                //  Resolvable private address type (10)
	rand bit [31:0] adv_init_AA;       // 
  rand bit [23:0] crc_init;         // CRC initial value for conn_req pkt
  rand bit [7:0]  winsize;
  rand bit [16:0] winoffset;
  rand bit [16:0] interval;
  rand bit [16:0] latency;
  rand bit [16:0] timeout;
  rand bit [39:0] chm;
  rand bit [4:0]  hop;
  rand bit [2:0]  sca;

  // CONN State PKT Details	( bt_pkt_type == DATA )

  //	enum {ll_DATA,ll_CONTROL} type_of_data;

	typedef enum {LL_CONNECTION_UPDATE_REQ,LL_CHANNEL_MAP_REQ,LL_TERMINATE_IND,LL_ENC_REQ,LL_ENC_RSP,LL_START_ENC_REQ,
                LL_START_ENC_RSP,LL_UNKNOWN_RSP,LL_FEATURE_REQ,LL_FEATURE_RSP,LL_PAUSE_ENC_REQ,LL_PAUSE_ENC_RSP,
                LL_VERSION_IND,LL_REJECT_IND,LL_SLAVE_FEATURE_REQ,LL_CONNECTION_PARAM_REQ,LL_CONNECTION_PARAM_RSP,
                LL_REJECT_IND_EXT,LL_PING_REQ,LL_PING_RSP} type_of_control_pkt;
 
        
	rand bit [7:0]  control_pdu_opcode; // Indicates opcode for above "type_of_control_pkt" enum.
                                     // control_pdu_opcode[0x14]-control_pdu_opcode[0xFF] = RFU
  rand bit [1:0]  data_pdu_llid;               // used to indicate start/fragment/empty packet.

  rand bit [31:0] conn_data[32];
	rand bit [7:0]  ctrl_data[];


  // Controller calculated variables    ( These variables are not touched by AHB/Host, These are used in sequece_item for comparing in SB )
	
	    

       bit [23:0] crc;              // CRC var for formation of packet
//rand bit [31:0] MIC;
	rand bit        data_pdu_sn;
	rand bit        data_pdu_nesn;
	rand bit        data_pdu_md;
	rand bit [4:0]  data_pdu_length;    // length field for Data packets is 5 bits. 
                                      // So, it can accommodate 31 bytes in data packet. out of which, 
                                      // 4 bytes goes for MIC, So that is the reason max payload in Data packets is 27 bytes. 
  rand bit [7:0]  adv_payload[];
  rand int        no_of_adv;          // Indicates how many times ADV has to be repeated
	rand bit [7:0]  scan_rsp_data[8];
  // NON-CONN State PKT Details   ( bt_pkt_type == ADV )
  //constraint pt{pkt_type==0;}
        
	//   bit [1:0] RFU = 2'b00;


  // Constraint on Advertising address channel PDU
  constraint ADV_PDU_TYPE{(adv_type== ADV_IND)       -> adv_pdu == 4'b0000;    // Adv undirected
			                    (adv_type==ADV_DIRECT_IND) -> adv_pdu == 4'b0001;    // Adv directed high/low duty cycle
			     			          (adv_type==ADV_NONCONN_IND)-> adv_pdu == 4'b0010;  // Non connectable undirect
                          (adv_type==SCAN_REQ)       -> adv_pdu == 4'b0011;    // Scan req pdu
                          (adv_type == SCAN_RSP)     -> adv_pdu == 4'b0100;    // Scan rsp
			              		  (adv_type==CONNECT_REQ)    ->	adv_pdu == 4'b0101;   // Conn req
		                      (adv_type==ADV_SCAN_IND)   -> adv_pdu == 4'b0110;   // Scannable undirected.
			                     }
			   

	
  // Constraint on Random device address
  constraint dev_addr{ rand_dev_addr_type!=2'b11; solve rand_dev_addr_type before source_device_addr;
                if(adv_pdu_txadd==1) 
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

	constraint ctrl_opcode{control_pdu_opcode inside {[0:19]};}
  constraint LLID{data_pdu_llid != 2'b00;}
  constraint ACCESS_ADDRESS{ if(pkt_type == 1) 
                  {foreach(adv_init_AA[i])
		                if(i<27)
		               ((adv_init_AA[i] && adv_init_AA[i+1] && adv_init_AA[i+2] && adv_init_AA[i+3] && adv_init_AA[i+4] && adv_init_AA[i+5]) != 1);
                    //No 6 consecutive ones

		               foreach(adv_init_AA[j])
		               if(j<27)
		               ((adv_init_AA[j] || adv_init_AA[j+1] || adv_init_AA[j+2] || adv_init_AA[j+3] || adv_init_AA[j+4] || adv_init_AA[j+5]) != 0); 
                    //No 6 consecutive zeroes
	                  adv_init_AA != 32'h8E89BED6;          // Shall not be Advertising packet's Access address.
    		            adv_init_AA[31:24] != adv_init_AA[23:16]; adv_init_AA[23:16] != adv_init_AA[15:8]; adv_init_AA[15:8] != adv_init_AA[7:0];                       // All 4 octects shall not be same  ;
                     // abc(adv_init_AA)==1;             // Shall not differ by Advertising channel Access address by 1 bit.
                    $countones(adv_init_AA ^ (32'h8E89BED6))>1;
      	            ($countones(adv_init_AA ^ (adv_init_AA*2)) -(1&adv_init_AA[0]))<25; // Shall not have more than 24 transitions
                    ($countones(adv_init_AA[31:26] ^ (adv_init_AA[31:26]*2))-(1&adv_init_AA[26]))>1; 
                    //Shall have a minimum of 2 transitions in most significant six bits
                    }
                else   
    	              {
		                  adv_init_AA == 32'h8E89BED6;
		                } 
	          }	
 constraint CRC{if(pkt_type == 1)
                  crc_init == 24'h555555;}

 constraint PREAMBLE{if(adv_init_AA[0]==1)
                 preamble==8'b01010101;
		            else
		             preamble==8'b10101010;
                }


constraint LENGTH {solve length before adv_pdu_length;
                     (length== SMALL) ->  (adv_pdu_length inside {[6:10]});
                     (length== MEDIUM) -> (adv_pdu_length inside {[11:25]});
                     (length== LARGE) ->  (adv_pdu_length inside {[26:37]});

                    }


 constraint adv_length{//solve adv_type before adv_payload; //solve adv_type before adv_pdu_length;// solve adv_pdu_length before adv_payload;
                     if(adv_type==1)
                        {
			                    adv_pdu_length == 12;
			                    adv_payload.size()==12; 
			              		 }
		                 else 
		                     {
			                    adv_pdu_length inside {[6:37]};
			                    adv_payload.size()==adv_pdu_length;
		                			}
                       }


	 
 extern function new(string name = "seq_item");
 //extern function void do_print(ovm_printer printer);
 //extern function void do_pack(ovm_packer packer);
 //extern function void do_unpack(ovm_packer packer);
endclass: seq_item

function seq_item::new(string name = "seq_item");
 super.new(name);
endfunction

/*function void seq_item::do_pack(ovm_packer packer);
 super.do_pack(packer);
 packer.pack_field(preamble,8);
 packer.pack_field(adv_init_AA, 32);
 packer.pack_field(adv_pdu_type,4);
 packer.pack_field(RFU,2);
 packer.pack_field(adv_pdu_txadd,1);
 packer.pack_field(adv_pdu_rxadd,1);
 packer.pack_field(adv_pdu_length,6);
 packer.pack_field(RFU,2);
 foreach(adv_payload[i])// for(int i=0;i<adv_payload.size();i++)
 begin
 packer.pack_field(adv_payload[i],8);
 end
 packer.pack_field(crc_init,24);
endfunction
*/
//function void seq_item::do_unpack(ovm_packer packer);
 
 
//endfunction
/*function void seq_item::do_print(ovm_printer printer);
  super.do_print(printer);

  printer.print_field("adv_pdu_type", this.adv_pdu_type, 4, OVM_BIN);
  printer.print_field("source_device_addr", this.source_device_addr, 48, OVM_HEX);
  printer.print_field("target_device_addr", this.target_device_addr, 48, OVM_HEX);
  printer.print_field("adv_pdu_txadd", this.adv_pdu_txadd, 1, OVM_BIN);
  printer.print_field("adv_pdu_rxadd", this.adv_pdu_rxadd, 1, OVM_BIN);
  printer.print_field("no_of_adv", this.no_of_adv, 32, OVM_UNSIGNED);
  printer.print_field("adv_type", this.adv_type, 32, OVM_UNSIGNED);
  printer.print_field("pkt_type", this.pkt_type, 1, OVM_BIN);

/*printer.print_field("adv_data[0]", this.adv_data[0], 32, OVM_HEX);
  printer.print_field("adv_data[1]", this.adv_data[1], 32, OVM_HEX);
  printer.print_field("adv_data[2]", this.adv_data[2], 32, OVM_HEX);
  printer.print_field("adv_data[3]", this.adv_data[3], 32, OVM_HEX);
  printer.print_field("adv_data[4]", this.adv_data[4], 32, OVM_HEX);
  printer.print_field("adv_data[5]", this.adv_data[5], 32, OVM_HEX);
  printer.print_field("adv_data[6]", this.adv_data[6], 32, OVM_HEX);
  printer.print_field("adv_data[7]", this.adv_data[7], 32, OVM_HEX);
  printer.print_field("scan_rsp_data[0]", this.scan_rsp_data[0], 32, OVM_HEX);
  printer.print_field("scan_rsp_data[1]", this.scan_rsp_data[1], 32, OVM_HEX);
  printer.print_field("scan_rsp_data[2]", this.scan_rsp_data[2], 32, OVM_HEX);
  printer.print_field("scan_rsp_data[3]", this.scan_rsp_data[3], 32, OVM_HEX);
  printer.print_field("scan_rsp_data[4]", this.scan_rsp_data[4], 32, OVM_HEX);
  printer.print_field("scan_rsp_data[5]", this.scan_rsp_data[5], 32, OVM_HEX);
  printer.print_field("scan_rsp_data[6]", this.scan_rsp_data[6], 32, OVM_HEX);
  printer.print_field("scan_rsp_data[7]", this.scan_rsp_data[7], 32, OVM_HEX);  
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

/*  printer.print_field("control_pdu_opcode", this.control_pdu_opcode, 16, OVM_HEX);
  printer.print_field("data_pdu_llid", this.data_pdu_llid, 2, OVM_BIN);
  printer.print_field("adv_init_AA", this.adv_init_AA, 32, OVM_HEX);
  printer.print_field("preamble", this.preamble, 8, OVM_BIN);
  printer.print_field("crc_init", this.crc_init, 24, OVM_HEX);
  printer.print_field("MIC", this.MIC, 32, OVM_HEX);
  printer.print_field("data_pdu_sn", this.data_pdu_sn, 1, OVM_BIN); 
  printer.print_field("data_pdu_nesn", this.data_pdu_nesn, 1, OVM_BIN); 
  printer.print_field("data_pdu_md", this.data_pdu_md, 1, OVM_BIN); 
  printer.print_field("adv_pdu_length", this.adv_pdu_length, 6, OVM_UNSIGNED); 
  printer.print_field("data_pdu_length", this.data_pdu_length, 5, OVM_UNSIGNED);


endfunction*/
