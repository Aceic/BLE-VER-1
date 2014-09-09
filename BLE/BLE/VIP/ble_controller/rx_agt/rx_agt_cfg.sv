class rx_agt_cfg extends ovm_object;
	localparam string s_my_config_id = "rx_agt_cfg";
	localparam string s_no_config_id = "no config";
	localparam string s_my_config_type_error_id = "config type error";

	`ovm_object_utils (rx_agt_cfg)
	virtual pp_intf vif;
	bit [31:0] access_addr = 'h8E89BED6;
	bit [6:0] white_init = {1'b1,6'd37};
	bit [23:0] crc_init = 'h555555;

	function new (string name="rx_agt_cfg");
		super.new (name);
	endfunction : new

	extern static function rx_agt_cfg get_config (ovm_component c);
endclass : rx_agt_cfg

function rx_agt_cfg rx_agt_cfg::get_config (ovm_component c);
	ovm_object o;
  rx_agt_cfg t;

  if(!c.get_config_object (s_my_config_id , o , 0))
	begin
		c.ovm_report_error (s_no_config_id,
												$sformatf("no config associated with %s",s_my_config_id),
												OVM_NONE,`ovm_file,`ovm_line);
    return null;
  end

  if(!$cast (t,o))
	begin
		c.ovm_report_error (s_my_config_type_error_id,
                        $sformatf("config %s associated with config %s is not of type my_config",o.sprint(),s_my_config_id ),
                        OVM_NONE,`ovm_file,`ovm_line );
  end

  return t;
endfunction : get_config
