class ahb_master_agent extends ovm_agent;
  `ovm_component_utils(ahb_master_agent)

   ahb_master_driver      m_driver;
   ahb_master_monitor     m_monitor;
   ahb_master_sequencer   m_sequencer;

   ahb_agent_config       m_cfg;

  
//standard OVM methods
  
  extern function new(string name = "ahb_master_agent", ovm_component parent = null);
  extern function void build();
  extern function void connect();

endclass : ahb_master_agent

//*****************************************************************************
  
  function ahb_master_agent::new(string name = "ahb_master_agent", ovm_component parent = null);
    super.new(name,parent);
  endfunction

//*****************************************************************************

  function void ahb_master_agent::build();

 //get method for configuration 
    ovm_object tmp;
    if(!(get_config_object("ahb_agent_config",tmp)))
       `ovm_fatal("ahb_master_agent","can not get config object...") 
    if(!$cast(m_cfg,tmp))
       `ovm_fatal("ahb_master_agent","invalid cast....") 
    //m_cfg = ahb_agent_config::get_config(this);

 //monitor is always present
    m_monitor = ahb_master_monitor::type_id::create("m_monitor",this);

 //create drive and sequencer is agent is active
 //  if(m_cfg.is_active == OVM_ACTIVE)
    begin
      m_driver = ahb_master_driver::type_id::create("m_drver",this);
      m_sequencer = ahb_master_sequencer::type_id::create("m_sequencer",this);
    end
  
  endfunction : build

//****************************************************************************

  function void ahb_master_agent::connect();
  
    if(m_cfg.is_active == OVM_ACTIVE)
      m_driver.seq_item_port.connect(m_sequencer.seq_item_export);
    
  endfunction : connect


//****************************************************************************
