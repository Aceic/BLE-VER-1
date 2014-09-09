

class ble_vip_env extends ovm_agent;

  `ovm_component_utils(ble_vip_env)                                                   // Factory registration of VIP env testbench

  ble_controller_env ctrl_env;                                                       // Instance of controller environment
  ahb_bfm            ahb_slv;                                                         // Instance of AHB bfm
  //VIP_Scoreboard         vip_sb;                                                         // Instance of VIP scoreboard

  ble_vip_config    vip_cfg;
   
  ovm_object temp;

  extern function new(string name = "ble_vip_env", ovm_component parent = null);
  extern function void build();
  extern function void connect();

endclass: ble_vip_env


  function ble_vip_env:: new(string name = "ble_vip_env", ovm_component parent = null);
   super.new(name, parent);
  endfunction 


  function void ble_vip_env:: build();
  
   `ovm_info(get_name(),"BUILD PHASE",OVM_LOW)

    if(!(get_config_object("vip_cfg",temp)))
        `ovm_fatal("BLE_VIP_ENV","can not get config object")
    if(!$cast(vip_cfg,temp))
       `ovm_error("BLE_VIP_ENV","invalid cast....") 
 
   
   ctrl_env = ble_controller_env::type_id::create("ctrl_env", this);                
   ahb_slv   = ahb_bfm::type_id::create("ahb_slv", this);
  
  // vip_sb   = scoreboard::type_id::create("vip_sb", this);

  //setting config for controller  
   set_config_object("ctrl_env*","controller_config",vip_cfg.ctrl_cfg,0);      
  //setting config for AHB slave 
   set_config_object("ahb_slv*","ahb_slave_agent_config",vip_cfg.slv_cfg,0);

  endfunction: build

  function void ble_vip_env:: connect();
   
   `ovm_info(get_name(),"CONNECT PHASE",OVM_LOW)

  endfunction:connect

