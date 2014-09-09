package ble_test_pkg;
	import ovm_pkg::*;
  import ovm_container_pkg::*;

`include "ovm_macros.svh"
`include "uvm_reg_macros.svh"
  import uvm_reg_pkg::*;

//`include "tb_defs.sv"

// `include "ahb_all_reg.sv"
// `include "ahb_reg_block.sv"
// `include "ble_reg_bank_pkg.sv"
  import ble_reg_bank_pkg::*;
//`ifndef ble_ral_block_included
//`define ble_ral_block_included
//`include "ble_ral_block.sv"
//`endif
  import ahb_slv_pkg::*;
//  import intr_pkg::*;
//   import tx_agent_pkg::*;
//   import rx_agent_pkg::*;
  import ovc_pkg::*;
  import  ctrl_pkg::*; 

  import vip_pkg::*;





`include "ble_env_config.sv"
`include "ble_virtual_sequencer.sv"

`include "ble_virtual_seq.sv"



//`include "ble_env_config.sv"
//`include "ahb_virtual_seqs.sv"
`include "scoreboard.sv"
`include "ble_env.sv"


//`include "ahb_base_test.sv"
`include "ble_base_test.sv"
//`include "test_pkg.sv"
endpackage
