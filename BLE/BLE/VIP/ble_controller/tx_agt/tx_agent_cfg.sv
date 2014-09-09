class tx_agent_cfg extends ovm_object;

   `ovm_object_utils(tx_agent_cfg)

   extern function new(string name = "tx_agent_cfg");

    virtual pp_intf vif;
    
   // Class fields


   ovm_active_passive_enum active = OVM_ACTIVE;

endclass


function tx_agent_cfg::new(string name = "tx_agent_cfg");

   super.new(name);

endfunction



