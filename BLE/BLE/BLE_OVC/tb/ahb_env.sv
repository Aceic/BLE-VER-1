class ahb_env extends ovm_env;
  `ovm_component_utils(ahb_env);

  ahb_master_agent ahb_agent;
  ahb_env_config env_cfg;
	ahb_bfm bfm;
  ahb_adapter ahb_adp;

  extern function new(string name = "ahb_env", ovm_component parent);
  extern function void build();
  extern function void connect();
  extern task run();
 
endclass

  
  function ahb_env::new(string name = "ahb_env", ovm_component parent);
    super.new(name,parent);
  endfunction;

  function void ahb_env::build();
      ovm_object tmp;
      assert(get_config_object("config",tmp));
      if(!$cast(env_cfg,tmp))
       `ovm_error("ahb_master_monitor","invalid cast....") 
 
    // if(env_cfg.has_ahb_agent)
    //    begin
         // set_config_object("ahb_agent*","ahb_agent_config",env_cfg.agent_cfg,0);
          set_config_object("*","ahb_agent_config",env_cfg.agent_cfg,0);
          ahb_agent = ahb_master_agent::type_id::create("ahb_agent",this);
					bfm = ahb_bfm::type_id::create("bfm",this);
    //  end
        ahb_adp = ahb_adapter::type_id::create("ahb_adp"); 
  endfunction

  function void ahb_env::connect();
      if(env_cfg.ahb_rm.get_parent() == null)
      begin
       env_cfg.ahb_rm.ahb_map.set_sequencer(ahb_agent.m_sequencer, ahb_adp); 
      end
       env_cfg.ahb_rm.ahb_map.set_auto_predict(1);
 
  endfunction

  task ahb_env::run();
    ovm_top.print_topology();
  endtask
