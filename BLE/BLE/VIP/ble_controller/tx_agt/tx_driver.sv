class tx_driver extends ovm_driver #(ovm_sequence_item);

   `ovm_component_utils(tx_driver)
   
   tx_agent_cfg tx_cfg;

// temp variables
   int i,count;

   virtual pp_intf vif;

   bit frame_packet[];

   // Standard OVM methods

   extern function new(string name = "tx_driver", ovm_component parent=null);
   extern function void build();
   extern function void connect();
    extern task run;
   extern task perform_dut(pkt_seq_item txn);


endclass: tx_driver


function tx_driver::new(string name = "tx_driver", ovm_component parent = null);

	super.new(name,parent);

endfunction

function void tx_driver::build();
   ovm_object temp;
   if(!get_config_object("tx_cfg",temp,0))
    `ovm_fatal("TX_DRIVER","Cannot get config handle..")
 
   if(!$cast(tx_cfg,temp))
    `ovm_fatal("TX_DRIVER","Casting failed")

  
  // vif = ovm_container #(virtual ppi_interface)::get_value_from_config(this, "vif");
endfunction

function  void tx_driver::connect();
  vif = tx_cfg.vif;
endfunction

task tx_driver::run;
   
   pkt_seq_item txn1;
   vif.channel_index = 'd37;

   forever
      begin
         seq_item_port.get_next_item(req);
         perform_dut(req);
	 seq_item_port.item_done();
      end

endtask

task tx_driver::perform_dut(pkt_seq_item txn);


   txn.pack(frame_packet);
  // txn.frame_pkt = frame_packet;
 /* txn.randomize();
  txn.pack(txn.frame_pkt);
*/

   $display("started driving packet");
   `ovm_info("BLE_Driver:run",$sformatf("Interface value before vif.BLE_TX = %b ",vif.BLE_TX),OVM_LOW);
   `ovm_info("BLE_Driver:run",$sformatf("Interface value before vif.BLE_RX = %b ",vif.BLE_RX),OVM_LOW);
   `ovm_info("BLE_Driver:run",$sformatf("Interface value before vif.TX_FLAG = %b ",vif.TX_FLAG),OVM_LOW);
   @(posedge vif.clock);
   `ovm_info("BLE_Driver:run",$sformatf("Interface value after vif.clock vif.TX_FLAG = %b ",vif.TX_FLAG),OVM_LOW);
   @(posedge vif.BLE_TX);
   `ovm_info("BLE_Driver:run",$sformatf("Interface value after vif.BLE_TX = %b ",vif.BLE_TX),OVM_LOW);
   `ovm_info("BLE_Driver:run",$sformatf("Interface value after vif.BLE_RX = %b ",vif.BLE_RX),OVM_LOW);
   for(i=0;i<frame_packet.size();i++)
   begin

      @(posedge vif.clock);
      vif.PPI <= frame_packet[i] ;
      count = count+1;
      $display("Count value is %d",count);

   end
   @(posedge vif.BLE_TX);
   count = 0;

endtask
