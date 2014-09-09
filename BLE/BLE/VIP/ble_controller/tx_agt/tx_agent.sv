class tx_agent extends ovm_agent;

   `ovm_component_utils(tx_agent)

   tx_agent_cfg m_cfg;

   // OVM component members

   tx_monitor m_monitor;
   tx_driver m_driver;
   tx_sequencer m_sequencer;


   // Standard UVM methods


   extern function new(string name = "tx_agent", ovm_component parent);
   extern function void build();
   extern function void connect();


endclass: tx_agent


function tx_agent::new(string name = "tx_agent", ovm_component parent);

   super.new(name,parent);

endfunction

function void tx_agent::build();

//  Get Tx Agent config 
//  m_cfg = tx_agent_cfg::type_id::create(  

   m_monitor = tx_monitor::type_id::create("m_monitor",this);

   m_driver = tx_driver::type_id::create("m_driver",this);
   m_sequencer = tx_sequencer::type_id::create("m_sequencer",this);

endfunction

function void tx_agent::connect();

   // Check for OVM_ACTIVE in cfg file, after getting its object

   m_driver.seq_item_port.connect(m_sequencer.seq_item_export);

endfunction


