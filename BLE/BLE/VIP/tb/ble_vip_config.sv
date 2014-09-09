class ble_vip_config extends ovm_object;

   `ovm_object_utils(ble_vip_config)

   virtual pp_intf vif;   

   ble_ral_block ble_ral;

   controller_config ctrl_cfg;
   ahb_slave_agent_config slv_cfg;

   function new(string name = "ble_vip_config");
      super.new(name);
   endfunction

endclass
