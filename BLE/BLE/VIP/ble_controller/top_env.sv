class top_env extends ovm_env;
   
   `ovm_component_utils(top_env)

   ble_controller_env env_h;

   extern function new(string name = "top_env", ovm_component parent);
   extern function void build();

endclass

function top_env::new(string name = "top_env", ovm_component parent);
   
   super.new(name,parent);

endfunction


function void top_env::build();

   env_h = ble_controller_env::type_id::create("env_h",this);


endfunction

