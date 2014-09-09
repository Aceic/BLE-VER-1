  package intr_pkg;
  
   import ovm_pkg::*;
   import ovm_container_pkg::*;
   import uvm_reg_pkg::*;
   `include "uvm_reg_macros.svh"
   import ble_reg_bank_pkg::*;
   `include "ovm_macros.svh"

  // import tx_agent_pkg::*;

   `include "intr_seq_item.sv"

   `include "bt_intr_seq_item.sv"   //temp file...need to remove after integration

   `include "intr_config.sv"
   `include "intr_driver.sv"
   `include "intr_sequencer.sv"
   `include "intr_monitor.sv"
   `include "intr_agent.sv" 

  endpackage
