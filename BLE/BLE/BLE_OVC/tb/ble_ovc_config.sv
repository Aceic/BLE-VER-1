class ble_ovc_config extends ovm_object;
  `ovm_object_utils(ble_ovc_config)

  bit has_ahb_agent = 1;
  ahb_agent_config ahb_agent_cfgh;
	virtual ahb_if vif;
  ble_ral_block ble_ral;
  extern function new(string name = "ble_ovc_config");


endclass



 function ble_ovc_config::new(string name = "ble_ovc_config");
    super.new(name);
  endfunction 

  
