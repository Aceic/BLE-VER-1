

module top;
 `include "ovm_macros.svh"
 `include "uvm_reg_macros.svh" 
  import uvm_reg_pkg::*;
  import ovm_pkg::*;
  import ovm_container_pkg::*;
  import ble_reg_bank_pkg::*;
  import ble_test_pkg::*; 
 
  bit clk,ppi_clk;
  bit reset;

  ahb_if m_AHB0(clk,reset);
  ahb_if m_AHB1(clk,reset);
  
//instantiate interrupt interface
  intr_if int_if0();
  intr_if int_if1();
 
// Instantiate PPI Interface

   pp_intf ppi0(ppi_clk,reset);
   pp_intf ppi1(ppi_clk,reset);

//clock module
  clk_agent CLK(clk,ppi_clk);

//channel interface
  channel_if ch_if();

//channel model
  channel_module CHM(ppi0, ppi1, ch_if);

//	ahb_if S_AHB(clk,reset);
  //ahb_memory_slave MEMORY_MODEL (.AHB(AHB));
	
  initial
    begin
      ovm_container #(virtual ahb_if)::set_value_in_global_config("ahb_mvif0",m_AHB0);
      ovm_container #(virtual ahb_if)::set_value_in_global_config("ahb_mvif1",m_AHB1);
      ovm_container #(virtual ahb_if)::set_value_in_global_config("ahb_svif0",m_AHB0);
      ovm_container #(virtual ahb_if)::set_value_in_global_config("ahb_svif1",m_AHB1);

      ovm_container #(virtual pp_intf)::set_value_in_global_config("pp_intf0", ppi0);
      ovm_container #(virtual pp_intf)::set_value_in_global_config("pp_intf1", ppi1);
      
      ovm_container #(virtual intr_if)::set_value_in_global_config("int_if0",int_if0);
      ovm_container #(virtual intr_if)::set_value_in_global_config("int_if1",int_if1);
        
      run_test();
      $finish;
    end

 
   initial
    begin
        reset = 1'b0;
   			
			 #10 reset = 1'b1;
   // #10 @(posedge clk) reset = 1'b1;

    end

endmodule
