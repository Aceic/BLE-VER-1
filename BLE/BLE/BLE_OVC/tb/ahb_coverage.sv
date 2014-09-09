class ahb_coverage extends ovm_component;
	`ovm_component_utils (ahb_coverage)

	tlm_analysis_fifo #(ahb_seq_item) ahb_fifo;

	ahb_seq_item ahb_xtn;

	bit [7:0] cp_ahb_addr;
	bit [2:0] cp_ahb_burst;
	bit [1:0] cp_ahb_resp[$];
	bit [2:0] cp_ahb_size;
	bit [1:0] cp_ahb_trans;
	bit 			cp_ahb_write;
	bit [31:0] cp_ahb_wdata[$];
	bit [31:0] cp_ahb_rdata[$];

	covergroup AHB_cov with function sample (int i);
  option.per_instance=1;

		CP_AHB_ADDR: coverpoint cp_ahb_addr {
			bins bin_addr_00 = {8'h00};
			bins bin_addr_04 = {8'h04};
			bins bin_addr_08 = {8'h08};
			bins bin_addr_0C = {8'h0C};
			bins bin_addr_10 = {8'h10};
			bins bin_addr_14 = {8'h14};
			bins bin_addr_18 = {8'h18};
			bins bin_addr_28 = {8'h28};
			bins bin_addr_2C = {8'h2C};
			bins bin_addr_30 = {8'h30};
			bins bin_addr_34 = {8'h34};
			bins bin_addr_38 = {8'h38};
			bins bin_addr_3C = {8'h3C};
			bins bin_addr_40 = {8'h40};
			bins bin_addr_44 = {8'h44};
			bins bin_addr_48 = {8'h48};
			bins bin_addr_4C = {8'h4C};
			bins bin_addr_50 = {8'h50};
			bins bin_addr_54 = {8'h54};
			bins bin_addr_58 = {8'h58};
			bins bin_addr_5C = {8'h5C};
			bins bin_addr_60 = {8'h60};
			bins bin_addr_64 = {8'h64};
			bins bin_addr_68 = {8'h68};
			bins bin_addr_6C = {8'h6C};
			bins bin_addr_70 = {8'h70};
			bins bin_addr_74 = {8'h74};
			bins bin_addr_78 = {8'h78};
			bins bin_addr_7C = {8'h7C};
			bins bin_addr_80 = {8'h80};
			bins bin_addr_84 = {8'h84};
			bins bin_addr_88 = {8'h88};
			bins bin_addr_8C = {8'h8C};
			bins bin_addr_90 = {8'h90};
			bins bin_addr_94 = {8'h94};
			bins bin_addr_98 = {8'h98};
			bins bin_addr_9C = {8'h9C};
			bins bin_addr_A0 = {8'hA0};
			bins bin_addr_A4 = {8'hA4};
		}
		CP_AHB_BURST: coverpoint cp_ahb_burst {
			bins bin_burst_single = {0};
			ignore_bins bin_burst = {[1:7]};
		}
		CP_AHB_RESP: coverpoint cp_ahb_resp[i] {
			bins bin_okay = {2'b0};
			bins bin_error = {2'b1};
		}
		CP_AHB_SIZE: coverpoint cp_ahb_size {
			bins bin_byte = {3'h0};
			bins bin_word = {3'h2};
		}
		CP_AHB_TRANS: coverpoint cp_ahb_trans {
			bins bin_idle = {2'b00};
			bins bin_non_seq = {2'b10};
		}
		CP_AHB_WRITE: coverpoint cp_ahb_write {
			bins bin_write = {0};
			bins bin_read = {1};
		}
		CP_AHB_WDATA: coverpoint cp_ahb_wdata[i] {
			bins bin_min = {[0:32'h0000_00ff]};
			bins bin_mid1 = {[32'h0000_0100:32'h0000_ffff]};
			bins bin_mid2 = {[32'h0001_0000:32'h00ff_ffff]};
			bins bin_max = {[32'h0100_0000:32'hffff_ffff]};
		}
		CP_AHB_RDATA: coverpoint cp_ahb_rdata[i] {
			bins bin_min = {[0:32'h0000_00ff]};
			bins bin_mid1 = {[32'h0000_0100:32'h0000_ffff]};
			bins bin_mid2 = {[32'h0001_0000:32'h00ff_ffff]};
			bins bin_max = {[32'h0100_0000:32'hffff_ffff]};
		}
	
		CP_CROSS_AHB_WRITE: cross CP_AHB_ADDR,CP_AHB_BURST,CP_AHB_RESP,CP_AHB_SIZE,CP_AHB_TRANS,CP_AHB_WRITE,CP_AHB_WDATA;
		CP_CROSS_AHB_READ: cross CP_AHB_ADDR,CP_AHB_BURST,CP_AHB_RESP,CP_AHB_SIZE,CP_AHB_TRANS,CP_AHB_WRITE,CP_AHB_RDATA;
	endgroup

	function new (string name="ahb_coverage",ovm_component parent);
		super.new (name,parent);
		ahb_fifo = new("ahb_fifo",this);
		AHB_cov = new;
	endfunction

	task run();
    forever
     begin
  		ahb_fifo.get(ahb_xtn);
	  	`ovm_info ("SCOREBOARD",$sformatf("ahb sequence item from BLE_OVC:\n%s",ahb_xtn.sprint()),OVM_LOW);
		  cp_ahb_addr = ahb_xtn.haddr;
  		cp_ahb_burst = ahb_xtn.hburst;
	  	cp_ahb_resp = ahb_xtn.hresp;
		  cp_ahb_size = ahb_xtn.hsize;
  		cp_ahb_trans = ahb_xtn.htrans;
  		cp_ahb_write = ahb_xtn.hwrite;
  		cp_ahb_wdata = ahb_xtn.hwdata;
  		cp_ahb_rdata = ahb_xtn.hrdata;
 
      foreach(cp_ahb_resp[i])
        AHB_cov.sample(i);
  		foreach (cp_ahb_wdata[i])
  			AHB_cov.sample(i);
  		foreach (cp_ahb_rdata[i])
  			AHB_cov.sample(i);
    end
	endtask
endclass
