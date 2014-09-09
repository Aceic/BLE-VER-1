

class ble_env_config extends ovm_object;
	`ovm_object_utils(ble_env_config)

//	bit has_scoreboard = 1;
//	bit has_magent = 1;
//	bit has_sagent = 1;
//	bit has_virtual_sequencer = 1;
	int no_of_ovc = 1;
  int no_of_vip = 2;

	ble_ovc_config ovc_cfg[];
  ble_vip_config vip_cfg[];

	function new(string name = "ble_env_config");
		super.new(name);
	endfunction

endclass
