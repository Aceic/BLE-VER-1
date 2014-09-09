package rx_agent_pkg;

   import ovm_pkg::*;
   import ovm_container_pkg::*;
   import uvm_reg_pkg::*;
   `include "uvm_reg_macros.svh"
   import ble_reg_bank_pkg::*;
   `include "ovm_macros.svh"
    import tx_agent_pkg::*;
    import intr_pkg::*;
   // Including all the BT TX agent files here
            `include "engines.sv"
       `include "rx_agt_cfg.sv"

      `include "rx_monitor.sv"
      `include "rx_agent.sv"


endpackage: rx_agent_pkg


