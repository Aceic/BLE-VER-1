class ble_test extends ovm_test;

   `ovm_component_utils(ble_test)

   top_env top_env_h;

   extern function new(string name = "ble_test", ovm_component parent = null);
   extern function void build();

endclass

function ble_test::new(string name = "ble_test", ovm_component parent = null);
   
   super.new(name,parent);

endfunction

function void ble_test::build();

   top_env_h = top_env::type_id::create("top_env_h",this);

endfunction
//************************************************************************************************************

class ex_test extends ble_test;

   `ovm_component_utils(ex_test)

   extern function new(string name = "ex_test", ovm_component parent = null);
   extern function void build();
   extern task run;

endclass

function ex_test::new(string name = "ex_test", ovm_component parent = null);
   
   super.new(name,parent);

endfunction

function void ex_test::build();
   
   super.build();

endfunction

task ex_test::run;

   #100000;
  // global_stop_request();

endtask



