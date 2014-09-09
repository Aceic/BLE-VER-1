class tx_sequencer extends ovm_sequencer #(ovm_sequence_item);

   `ovm_component_utils(tx_sequencer)

   extern function new(string name = "tx_sequencer", ovm_component parent);

endclass

function tx_sequencer::new(string name = "tx_sequencer", ovm_component parent);

   super.new(name,parent);

endfunction



