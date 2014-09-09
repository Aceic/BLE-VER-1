typedef class controller_config;
//typedef class rx_agent;

class rx_monitor extends ovm_monitor;
	`ovm_component_utils (rx_monitor)

	ovm_analysis_port #(pkt_seq_item) rx_mon_port;
	ovm_analysis_port #(bt_intr_seq_item) rx_intr_port;

	rx_agt_cfg rx_cfg_h;
	controller_config ctrl_cfg_h;
	virtual pp_intf vif;
	pkt_engines pkt_eng_h;
	pkt_seq_item xtn_h;
	bt_intr_seq_item intr_h;

	bit [7:0] preamble;
	bit [39:0] rcvd_pre_aa;
	int length;
	bit [23:0] crc_ref;
	bit packet [];

	struct {
		bit [7:0] preamble;
		bit [31:0] access_addr;
		bit [15:0] header;
		bit [295:0] payload;
		bit [23:0] crc;
	} fields;

	function new (string name="rx_montior",ovm_component parent);
		super.new (name,parent);
		pkt_eng_h = pkt_engines::type_id::create("pkt_eng_h",this);
		rx_mon_port = new ("rx_mon_port",this);
		rx_intr_port = new ("rx_intr_port",this);
	endfunction : new

	function void build();
//Sun		ctrl_cfg_h = ctrl_cfg::get_config(this);
		//rx_cfg_h = rx_agt_cfg::get_config(this);
		ovm_object temp1,temp2;

    if(!get_config_object("controller_config",temp1))
      `ovm_fatal("RX_MONITOR","can not get config object")
   if(!$cast(ctrl_cfg_h,temp1))
      `ovm_fatal("RX_MONITOR","can not cast config object handle")


     if(!get_config_object("rx_cfg",temp2,0))
    `ovm_fatal("RX_MONITOR","Cannot get config handle..")
 
   if(!$cast(rx_cfg_h,temp2))
    `ovm_fatal("RX_MONITOR","Casting failed")

    super.build();
	endfunction : build

	function void connect();
		vif = rx_cfg_h.vif;
		preamble = rx_cfg_h.access_addr[0] ? 'h55 : 'haa;
	endfunction : connect

	task run();
		forever
		begin
			string stat;
			bit in_data,dw_data;
			int indx = 0;

			wait (vif.BLE_RX);
			xtn_h = pkt_seq_item::type_id::create("xtn_h");
			intr_h = bt_intr_seq_item::type_id::create("intr_h");
			`ovm_info ("RX_MONITOR","RX Started receiving Packet",OVM_LOW);
//preamble & access_addr
			forever
			begin
				@(posedge vif.clock);
				rcvd_pre_aa = {vif.PPI,rcvd_pre_aa[39:1]};

				if (rcvd_pre_aa == {rx_cfg_h.access_addr,preamble})
					break;
			end

			ctrl_cfg_h.int_cfg.pkt_received = 1;
			fields.preamble = rcvd_pre_aa[7:0];
			fields.access_addr = rcvd_pre_aa[39:8];

			pkt_eng_h.whitner_initial_value (rx_cfg_h.white_init);
			pkt_eng_h.crc_initial_value (rx_cfg_h.crc_init);

//header
			repeat (16)
			begin
				@(posedge vif.clock);
				in_data = vif.PPI;
				pkt_eng_h.whiten_data (in_data,dw_data);
				pkt_eng_h.crc_calculate_data (dw_data);
				fields.header = {dw_data,fields.header[15:1]};
			end
			if (fields.access_addr == 'h8E89BED6)
			begin
				intr_h.data = 0;
				intr_h.adv_pdu_type = fields.header[3:0];
				intr_h.adv_length = fields.header[15:8];
			end
			else
			begin
				intr_h.data = 1;
				intr_h.llid = fields.header[1:0];
				intr_h.data_length = fields.header[15:8];
			end

			`ovm_info ("RX_MONITOR",$sformatf("Interrupt sequence item sending to Interrupt agent : \n%s",intr_h.sprint()),OVM_LOW);
			rx_intr_port.write (intr_h);

			check_header (fields.header,stat);
			if (stat == "ERROR")
			begin
				ctrl_cfg_h.int_cfg.pkt_dropped = 1;
				`ovm_info ("RX_MONITOR",$sformatf("pkt_dropped interrupt triggered at %t",$time),OVM_LOW);
				break;
			end

			length = fields.header[15:8];
			length = length * 8;

//payload
			repeat (length)
			begin
				@(posedge vif.clock);
				in_data = vif.PPI;
				pkt_eng_h.whiten_data (in_data,dw_data);
				pkt_eng_h.crc_calculate_data (dw_data);
				fields.payload = {dw_data,fields.payload[295:1]};
			end
			fields.payload = fields.payload >> (296 - length);

//crc_calculated
			pkt_eng_h.crc_output_value (crc_ref);

//crc_rx
			repeat (24)
			begin
				@(posedge vif.clock);
				in_data = vif.PPI;
				pkt_eng_h.whiten_data (in_data,dw_data);
				fields.crc = {fields.crc[22:0],dw_data};
			end

//crc_check
			if (crc_ref != fields.crc)
			begin
				ctrl_cfg_h.int_cfg.bad_pkt_received = 1;
				`ovm_error ("RX_MONITOR",$sformatf("\n\tCRC:CRC mismatch received CRC = %h, Calculated CRC = %h.",fields.crc,crc_ref))
				stat = "ERROR";
//				break;
			end

//unpack
			packet = new[length + 80];
			for (int i = 0; i < 8; i++)
			begin
				packet[indx] = fields.preamble[i];
				indx++;
			end
			for (int i = 0; i < 32; i++)
			begin
				packet[indx] = fields.access_addr[i];
				indx++;
			end
			for (int i = 0; i < 16; i++)
			begin
				packet[indx] = fields.header[i];
				indx++;
			end
			for (int i = 0; i < length; i++)
			begin
				packet[indx] = fields.payload[i];
				indx++;
			end
			for (int i = 23; i >= 0; i--)
			begin
				packet[indx] = fields.crc[i];
				indx++;
			end

//			$write ("Packet = ");
//			foreach (packet[j])
//				$write (packet[j]);
//

			xtn_h.enc = 0;
			xtn_h.unpack(packet);
			xtn_h.channel_index = rx_cfg_h.white_init; //whitening init value is channel index used
			rx_mon_port.write (xtn_h);
			`ovm_info ("RX_MONITOR",$sformatf("PPI sequence item received : \n%s",xtn_h.sprint()),OVM_HIGH);
			wait (!vif.BLE_RX);
		end
	endtask : run

	task check_header (bit [15:0] HD, output string stat);
		if (rx_cfg_h.access_addr == 'h8e89bed6)
		begin
			if ({HD[15:14],HD[5:4]} > 0)
			begin
				`ovm_error ("RX_MONITOR",$sformatf("\n\tHD error:RFU Values of the packet should be 0: HD = %b.",HD))
				stat = "ERROR";
				return;
			end
			else if ((HD[15:8] < 6) || (HD[15:8] > 37))
			begin
				`ovm_error ("RX_MONITOR",$sformatf("\n\tHD error:Length for ADV packet should be in range [6:37]: len = %b.",HD[15:8]))
				stat = "ERROR";
				return;
			end
		end
		else 
		begin
			if ({HD[15:13],HD[7:5]} > 0)
			begin
				`ovm_error ("RX_MONITOR",$sformatf("\n\tHD error:RFU Values of the packet should be 0: HD = %b.",HD))
				stat = "ERROR";
				return;
			end
			else if (HD[15:8] > 31)
			begin
				`ovm_error ("RX_MONITOR",$sformatf("\n\tHD error:Length for DATA packet should be in range [0:31]: len = %b.",HD[15:8]))
				stat = "ERROR";
				return;
			end
		end
	endtask : check_header
endclass : rx_monitor
