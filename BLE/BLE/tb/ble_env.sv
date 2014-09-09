class ble_env extends ovm_env;
  `ovm_component_utils(ble_env)

  ble_ovc ovc[];
  ble_env_config env_cfg;
  ble_vip_env vip_env[];

  Ble_Scoreboard sb_top;

  //ahb_bfm bfm;
  ble_virtual_sequencer v_sequencer;

  extern function new(string name = "ble_env", ovm_component parent);
  extern function void build();
  extern function void connect();
  extern task run();
 
endclass

  
  function ble_env::new(string name = "ble_env", ovm_component parent);
    super.new(name,parent);
  endfunction;

  function void ble_env::build();
      ovm_object tmp;
      if(!(get_config_object("config",tmp)))
        `ovm_fatal("BLE_ENV","can not get config object")
      if(!$cast(env_cfg,tmp))
       `ovm_error("BLE_ENV","invalid cast....") 
 
            ovc = new[env_cfg.no_of_ovc];
          foreach(ovc[i])
            ovc[i] = ble_ovc::type_id::create($sformatf("ovc[%0d]",i),this);

          foreach(env_cfg.ovc_cfg[i])
          set_config_object($sformatf("ovc[%0d]*",i),"ovc_config",env_cfg.ovc_cfg[i],0);
     
          vip_env = new[env_cfg.no_of_vip];
          foreach(vip_env[i])
            vip_env[i] = ble_vip_env::type_id::create($sformatf("vip_env[%0d]",i),this);
    
          foreach(env_cfg.vip_cfg[i])
           begin
            set_config_object($sformatf("vip_env[%0d]*",i),"vip_cfg",env_cfg.vip_cfg[i],0);
           end

          sb_top = Ble_Scoreboard::type_id::create("sb_top", this);
                
          v_sequencer = ble_virtual_sequencer::type_id::create("v_sequencer",this);
          v_sequencer.m_seqrh = new[env_cfg.no_of_ovc];
    //  end
  endfunction

  function void ble_env::connect();
       foreach(v_sequencer.m_seqrh[i]) begin 
        v_sequencer.m_seqrh[i] = ovc[i].ahb_agent.m_sequencer;
        end 
   vip_env[0].ctrl_env.tx_agent_h.m_monitor.ap_tx.connect(sb_top.tx_fifo_0.analysis_export);	
   vip_env[1].ctrl_env.rx_agent_h.rx_mon.rx_mon_port.connect(sb_top.rx_fifo_1.analysis_export);
   vip_env[0].ctrl_env.rx_agent_h.rx_mon.rx_mon_port.connect(sb_top.rx_fifo_0.analysis_export);
   vip_env[1].ctrl_env.tx_agent_h.m_monitor.ap_tx.connect(sb_top.tx_fifo_1.analysis_export);	
    
  endfunction

  task ble_env::run();
    ovm_top.print_topology();
  endtask
