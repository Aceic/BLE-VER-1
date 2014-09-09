

class ble_virtual_sequencer extends ovm_sequencer #(ovm_sequence_item);
	`ovm_component_utils(ble_virtual_sequencer)

	ahb_master_sequencer m_seqrh[];

	function new(string name = "ble_virtual_sequencer" , ovm_component parent);
		super.new(name,parent);
	endfunction

endclass 
