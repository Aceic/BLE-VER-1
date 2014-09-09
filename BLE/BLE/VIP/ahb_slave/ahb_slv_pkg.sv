  package ahb_slv_pkg;
  
   import ovm_pkg::*;
   import ovm_container_pkg::*;
   import uvm_reg_pkg::*;
   `include "uvm_reg_macros.svh"
   import ble_reg_bank_pkg::*;
   `include "ovm_macros.svh"


   `include "ahb_slave_agent_config.sv"
   `include "ahb_slave_bfm.sv"

  endpackage
