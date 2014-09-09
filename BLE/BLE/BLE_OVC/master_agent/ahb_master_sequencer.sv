class ahb_master_sequencer extends ovm_sequencer #(ahb_seq_item);
  `ovm_component_utils(ahb_master_sequencer)

  function new(string name = "ahb_master_sequencer" ,ovm_component parent);
    super.new(name,parent);
  endfunction

endclass
  
