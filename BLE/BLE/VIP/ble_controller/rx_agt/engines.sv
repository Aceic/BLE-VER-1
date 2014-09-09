class pkt_engines extends ovm_component;
	`ovm_component_utils (pkt_engines)

	bit [6:0] reg_whitner;
	bit [23:0] reg_crc;

	function new (string name="pkt_engines", ovm_component parent);
		super.new (name,parent);
	endfunction : new

//whitner
	task whitner_initial_value (bit [6:0] initial_value);
		reg_whitner = initial_value;
	endtask : whitner_initial_value

	task whiten_data (bit in,output bit out);
out = in;
//		bit b3, b0;
//
//		b3 = reg_whitner[3];
//		b0 = reg_whitner[0];
//		reg_whitner [6:3] = {b0,reg_whitner[6:4]};
//		reg_whitner [2:0] = {(b3^b0),reg_whitner[2:1]};
//		out = b0^in;
	endtask : whiten_data

//crc_calculator
	task crc_initial_value (bit [23:0] initial_value);
		reg_crc = initial_value;
	endtask : crc_initial_value

	task crc_output_value (output bit [23:0] out);
		out = reg_crc;
	endtask : crc_output_value

	task crc_calculate_data (bit in);
		bit out,b23,b9,b8,b5,b3,b2,b0;

		b0 = reg_crc[0];
		b2 = reg_crc[2];
		b3 = reg_crc[3];
		b5 = reg_crc[5];
		b8 = reg_crc[8];
		b9 = reg_crc[9];
		b23 = reg_crc[23];

		out = b23^in;
		reg_crc[23:10] = {reg_crc[22:10],(b9^out)};
		reg_crc[9]     = b8^out;
		reg_crc[8:6]	 = {reg_crc[7:6],(b5^out)};
		reg_crc[5:4]	 = {reg_crc[4],(b3^out)};
		reg_crc[3] 		 = b2^out;
		reg_crc[2:1] 	 = {reg_crc[1],(b0^out)};
		reg_crc[0] 		 = out;
	endtask : crc_calculate_data

endclass : pkt_engines
