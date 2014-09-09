class adv_config_test extends ble_base_test;
`ovm_component_utils(adv_config_test)

  adv_scan_vseq adv_scan_vseq_h;
  extern function new(string name = "adv_config_test",ovm_component parent);
  `include "api_task.sv"
  extern function void build();
  extern task run();


endclass

  function adv_config_test::new(string name = "adv_config_test",ovm_component parent);
    super.new(name,parent);
  endfunction

  function void adv_config_test::build();
    super.build();
   endfunction
 

  task adv_config_test::run();
  //  adv_scan_vseq_h = adv_scan_vseq::type_id::create("adv_scan_vseq_h");
    ovm_test_done.raise_objection();
  //  adv_scan_vseq_h.start(env.v_sequencer);
       adv_configure("ADV_IND",10);

    ovm_test_done.drop_objection();

   endtask
