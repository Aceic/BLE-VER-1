class ahb_slave_agent_config extends ovm_object;
  `ovm_object_utils(ahb_slave_agent_config)

  virtual ahb_if vif;
  ble_ral_block ble_ral;
  ovm_active_passive_enum is_active = OVM_ACTIVE;

  function new(string name = "ahb_slave_agent_config");
      super.new(name);
  endfunction

endclass : ahb_slave_agent_config
