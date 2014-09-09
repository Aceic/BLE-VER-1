  package ctrl_pkg;
  
   import ovm_pkg::*;
   import ovm_container_pkg::*;
   import uvm_reg_pkg::*;
   `include "uvm_reg_macros.svh"
   import ble_reg_bank_pkg::*;
   `include "ovm_macros.svh"

 //  import intr_pkg::*;
    
   `include "intr_seq_item.sv"

   `include "bt_intr_seq_item.sv"   //temp file...need to remove after integration

   `include "intr_config.sv"
   `include "intr_driver.sv"
   `include "intr_sequencer.sv"
   `include "intr_monitor.sv"
   `include "intr_agent.sv" 

  

  // import tx_agent_pkg::*;

    
      `include "pkt_seq_item.sv"
      `include "tx_agent_cfg.sv"
      `include "tx_driver.sv"
      `include "tx_monitor.sv"
      `include "tx_sequencer.sv"
      `include "tx_agent.sv"


 //  import rx_agent_pkg::*;
      `include "engines.sv"
      `include "rx_agt_cfg.sv"

      `include "rx_monitor.sv"
      `include "rx_agent.sv"



   `include "reg_file.sv"
   
   `include "controller_config.sv"
   `include "ble_controller_env.sv"

  endpackage
