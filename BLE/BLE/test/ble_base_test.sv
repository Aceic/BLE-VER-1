
class ble_base_test extends ovm_test;
  `ovm_component_utils(ble_base_test)

  ble_env env;                                  //env handle
  ble_env_config env_cfg;                       //env cofig handle

 

  ble_vip_config vip_cfg[];                   //vip config handle
  controller_config ctrl_cfg[];
  intr_config  int_cfg[];        //interrupt agent config handle
  ble_ovc_config ovc_cfg[];   //ovc config handle
  ahb_agent_config ahb_agent_cfgh[];     //ahb agent config handle
  ahb_slave_agent_config ahb_slv_cfg[];
  tx_agent_cfg tx_cfg[];
  rx_agt_cfg rx_cfg[];

  ble_virtual_sequencer v_sequencer;            //virtual sequencer handle
 
  ble_ral_block ble_ral[];    //ble_ral block handle
  ble_reg_block ble_rm[];    // ble_reg_block handle
  ble_ral_block vip_ral[];  //ble_ral block handle for vip
  ble_reg_block vip_rm[];   // ble_reg_block handle for vip
  
  int no_of_vip = 0;
  int no_of_ovc = 0;
  
  extern function new(string name = "ble_base_test",ovm_component parent);
  extern function void build();

endclass : ble_base_test
   

  function ble_base_test::new(string name = "ble_base_test", ovm_component parent);
      super.new(name,parent);
  endfunction

  function void ble_base_test::build();
    no_of_ovc = 2;
    no_of_vip = 2;
    ble_ral = new[no_of_ovc];
    ble_rm  = new[no_of_ovc];
    vip_ral = new[no_of_vip];
    vip_rm  = new[no_of_vip];

    foreach(ble_ral[i])
      begin
        ble_ral[i] = new();
        ble_rm[i] = ble_reg_block::type_id::create($sformatf("ble_rm[%0d]",i));
        ble_rm[i].build();
        ble_ral[i].ble_rm = ble_rm[i];
      end

    foreach(vip_ral[i])
      begin
        vip_ral[i] = new();
        vip_rm[i] = ble_reg_block::type_id::create($sformatf("vip_rm[%0d]",i));
        vip_rm[i].build();
        vip_ral[i].ble_rm = vip_rm[i];
      end


    ovc_cfg = new[no_of_ovc];
    vip_cfg = new[no_of_vip];
    
    ahb_agent_cfgh = new[no_of_ovc];
    ahb_slv_cfg = new[no_of_vip];
    ctrl_cfg = new[no_of_vip];
    tx_cfg = new[no_of_vip];
    rx_cfg = new[no_of_vip];
    int_cfg = new[no_of_vip];

      env = ble_env::type_id::create("env",this);       //create environment handle
      env_cfg = ble_env_config::type_id::create("env_cfg"); //create environtment config class
      env_cfg.no_of_ovc = no_of_ovc;
      env_cfg.no_of_vip = no_of_vip;
   // set_config_object("env.m_ble_ovc*","m_reg_model",m_ble_rm,0);
   // set_config_object("env.s_ble_ovc*","s_reg_model",s_ble_rm,0);
    
      env_cfg.vip_cfg = new[no_of_vip];
      env_cfg.ovc_cfg = new[no_of_ovc];
     
     foreach(ovc_cfg[i])
        begin
          ovc_cfg[i]      = ble_ovc_config::type_id::create($sformatf("ovc_cfg[%0d]",i));
          ahb_agent_cfgh[i]  = ahb_agent_config::type_id::create($sformatf("ahb_agent_cfgh[%0d]",i));
          ahb_agent_cfgh[i].vif = ovm_container #(virtual ahb_if)::get_value_from_config(this,$sformatf("ahb_mvif%0d",i));
          ovc_cfg[i].ahb_agent_cfgh = ahb_agent_cfgh[i];
          ovc_cfg[i].ble_ral = ble_ral[i];
          env_cfg.ovc_cfg[i] = ovc_cfg[i];
        end
      
      foreach(vip_cfg[i])
        begin
           ahb_slv_cfg[i] = ahb_slave_agent_config::type_id::create($sformatf("slv_cfg[%0d]",i));
           ahb_slv_cfg[i].vif = ovm_container #(virtual ahb_if)::get_value_from_config(this,$sformatf("ahb_svif%0d",i));
           ahb_slv_cfg[i].ble_ral = vip_ral[i];

           int_cfg[i] = intr_config::type_id::create($sformatf("int_cfg[%0d]",i));
           int_cfg[i].int_if = ovm_container #(virtual intr_if)::get_value_from_config(this,$sformatf("int_if%0d",i));
           int_cfg[i].pkt_if = ovm_container #(virtual pp_intf)::get_value_from_config(this,$sformatf("pp_intf%0d",i));
           int_cfg[i].ble_ral = vip_ral[i];

	   
           tx_cfg[i] = tx_agent_cfg::type_id::create($sformatf("tx_cfg[%0d]",i));
           rx_cfg[i] = rx_agt_cfg::type_id::create($sformatf("rx_cfg[%0d]",i));

           tx_cfg[i].vif = ovm_container #(virtual pp_intf)::get_value_from_config(this,$sformatf("pp_intf%0d",i));
           rx_cfg[i].vif = ovm_container #(virtual pp_intf)::get_value_from_config(this,$sformatf("pp_intf%0d",i));

	         ctrl_cfg[i] = controller_config::type_id::create($sformatf("ctrl_cfg[%0d]",i));
	         ctrl_cfg[i].int_cfg = int_cfg[i];
           ctrl_cfg[i].ble_ral = vip_ral[i];
	         ctrl_cfg[i].tx_cfg = tx_cfg[i];
	         ctrl_cfg[i].rx_cfg = rx_cfg[i];

           vip_cfg[i] = ble_vip_config::type_id::create($sformatf("vip_cfg[%0d]",i));
           vip_cfg[i].vif = ovm_container #(virtual pp_intf)::get_value_from_config(this,$sformatf("pp_intf%0d",i));

           vip_cfg[i].ble_ral = vip_ral[i];
           vip_cfg[i].slv_cfg = ahb_slv_cfg[i];
	         vip_cfg[i].ctrl_cfg = ctrl_cfg[i];
           
           env_cfg.vip_cfg[i] = vip_cfg[i];
        end
          

   // configure_ahb_agent(agent_cfg);
  
  
    set_config_object("*","config",env_cfg,0);
    endfunction

  
//******************************************************************************************************


class adv_scan_test extends ble_base_test;
`ovm_component_utils(adv_scan_test)

  adv_scan_vseq adv_scan_vseq_h;
 // adv_random_vseq adv_random_vseq_h;
  extern function new(string name = "adv_scan_test",ovm_component parent);
  extern function void build();
  extern task run();


endclass

  function adv_scan_test::new(string name = "adv_scan_test",ovm_component parent);
    super.new(name,parent);
  endfunction

  function void adv_scan_test::build();
    super.build();
   endfunction

  task adv_scan_test::run();
    adv_scan_vseq_h = adv_scan_vseq::type_id::create("adv_scan_vseq_h");
    //adv_random_vseq_h = adv_random_vseq::type_id::create("adv_random_vseq_h");
    ovm_test_done.raise_objection();
    adv_scan_vseq_h.start(env.v_sequencer);
  //  if(env.v_sequencer)
   //adv_random_vseq_h.start(env.v_sequencer);
    ovm_test_done.drop_objection();
    
   endtask

//******************************************************************************************************


class adv_conn_test extends ble_base_test;
`ovm_component_utils(adv_conn_test)

 // adv_scan_vseq adv_scan_vseq_h;
  adv_conn_vseq adv_conn_vseq_h;
  extern function new(string name = "adv_conn_test",ovm_component parent);
  extern function void build();
  extern task run();


endclass

  function adv_conn_test::new(string name = "adv_conn_test",ovm_component parent);
    super.new(name,parent);
  endfunction

  function void adv_conn_test::build();
    super.build();
   endfunction

  task adv_conn_test::run();
   // adv_scan_vseq_h = adv_scan_vseq::type_id::create("adv_scan_vseq_h");
    adv_conn_vseq_h = adv_conn_vseq::type_id::create("adv_conn_vseq_h");
    ovm_test_done.raise_objection();
    //adv_scan_vseq_h.start(env.v_sequencer);
    adv_conn_vseq_h.start(env.v_sequencer);
    #100;
    ovm_test_done.drop_objection();

   endtask
