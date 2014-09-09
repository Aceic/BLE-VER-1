
class ble_ovc extends ovm_env;
  `ovm_component_utils(ble_ovc);

  ahb_master_agent ahb_agent;
  ble_ovc_config ovc_cfg;
  ahb_coverage ahb_cov;
//	ahb_bfm bfm;
  ahb_adapter ahb_adp;
 // ble_virtual_sequencer v_sequencer;

  extern function new(string name = "ble_ovc", ovm_component parent);
  extern function void build();
  extern function void connect();
  extern task run();
 
endclass

  
  function ble_ovc::new(string name = "ble_ovc", ovm_component parent);
    super.new(name,parent);
  endfunction;

  function void ble_ovc::build();
      ovm_object tmp;
      if(!(get_config_object("ovc_config",tmp)))
        `ovm_fatal("BLE_OVC","can not get config object")
      if(!$cast(ovc_cfg,tmp))
       `ovm_error("ahb_master_monitor","invalid cast....") 
 
    // if(env_cfg.has_ahb_agent)
    //    begin
         // set_config_object("ahb_agent*","ahb_agent_config",env_cfg.agent_cfg,0);
          set_config_object("*","ahb_agent_config",ovc_cfg.ahb_agent_cfgh,0);
          ahb_agent = ahb_master_agent::type_id::create("ahb_agent",this);
      
          ahb_cov = ahb_coverage::type_id::create("ahb_cov",this);    
				//	bfm = ahb_bfm::type_id::create("bfm",this);

         // v_sequencer = ble_virtual_sequencer::type_id::create("v_sequencer",this);
         // v_sequencer.m_seqrh = new[1];
    //  end
        ahb_adp = ahb_adapter::type_id::create("ahb_adp"); 
  endfunction

  function void ble_ovc::connect();
        
      //  v_sequencer.m_seqrh = ahb_agent.m_sequencer;

  //    if(env_cfg.ble_ral.get_parent() == null)
   //   begin
       ovc_cfg.ble_ral.ble_rm.bt_map.set_sequencer(ahb_agent.m_sequencer, ahb_adp); 
  //    end
       ovc_cfg.ble_ral.ble_rm.bt_map.set_auto_predict(1);
    
       ahb_agent.m_monitor.monitor_port.connect(ahb_cov.ahb_fifo.analysis_export);
  endfunction

  task ble_ovc::run();
   // ovm_top.print_topology();
  endtask
