


class intr_sequencer extends ovm_sequencer #(intr_seq_item);

 `ovm_component_utils(intr_sequencer)                                       // Factory registration of interrupt sequencer

 extern function new(string name = "intr_sequencer", ovm_component parent = null);

endclass: intr_sequencer

function intr_sequencer:: new(string name = "intr_sequencer", ovm_component parent = null);
 super.new(name, parent);
endfunction

