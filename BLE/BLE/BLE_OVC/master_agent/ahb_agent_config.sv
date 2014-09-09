class ahb_agent_config extends ovm_object;
  `ovm_object_utils(ahb_agent_config)

  virtual ahb_if vif;
 	virtual ahb_if slave; 
  ovm_active_passive_enum is_active = OVM_ACTIVE;

  function new(string name = "ahb_agent_config");
      super.new(name);
  endfunction

endclass : ahb_agent_config
