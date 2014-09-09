class controller_config extends ovm_object;

   `ovm_object_utils(controller_config)

   virtual pp_intf vif;   
   ble_ral_block ble_ral;
  
  intr_config int_cfg;
  tx_agent_cfg tx_cfg;
   rx_agt_cfg rx_cfg;




   function new(string name = "controller_config");
      super.new(name);
   endfunction

endclass
