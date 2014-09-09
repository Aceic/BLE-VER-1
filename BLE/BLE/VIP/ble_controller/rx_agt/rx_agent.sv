class rx_agent extends ovm_agent;
	`ovm_component_utils (rx_agent)	
	rx_monitor rx_mon;
	
	function new (string name="rx_agent",ovm_component parent);
		super.new (name,parent);
	endfunction : new
	
	function void build();
		super.build();
		rx_mon = rx_monitor::type_id::create("rx_mon",this);
	endfunction : build
endclass : rx_agent
