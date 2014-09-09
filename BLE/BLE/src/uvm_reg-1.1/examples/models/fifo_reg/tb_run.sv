// 
// -------------------------------------------------------------
//    Copyright 2004-2011 Synopsys, Inc.
//    Copyright 2010 Mentor Graphics Corporation
//    Copyright 2010-2011 Cadence Design Systems, Inc.
//    All Rights Reserved Worldwide
// 
//    Licensed under the Apache License, Version 2.0 (the
//    "License"); you may not use this file except in
//    compliance with the License.  You may obtain a copy of
//    the License at
// 
//        http://www.apache.org/licenses/LICENSE-2.0
// 
//    Unless required by applicable law or agreed to in
//    writing, software distributed under the License is
//    distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//    CONDITIONS OF ANY KIND, either express or implied.  See
//    the License for the specific language governing
//    permissions and limitations under the License.
// -------------------------------------------------------------
// 

`include "apb.sv"
`include "dut.sv"
`include "ovm_macros.svh"
`include "uvm_reg_macros.svh"

module dut_top;
   bit clk = 0;
   bit rst = 0;

   apb_if apb0(clk);
   dut dut0(apb0, rst);

   always #10 clk = ~clk;
endmodule: dut_top


module tb_top;

import ovm_pkg::*;
import uvm_reg_pkg::*;
import apb_pkg::*;

`include "reg_model.sv"
`include "tb_env.sv"

class dut_reset_seq extends ovm_sequence;

   function new(string name = "dut_reset_seq");
      super.new(name);
   endfunction

   `ovm_object_utils(dut_reset_seq)
   
   virtual task body();
      dut_top.rst = 1;
      repeat (5) @(negedge dut_top.clk);
      dut_top.rst = 0;
   endtask
endclass


class my_test extends tb_env;

   function new(string name = "my_test", ovm_component parent = null);
      super.new(name, parent);
   endfunction

   virtual task run();
      uvm_status_e   status;
      uvm_reg_data_t data;
      uvm_reg_data_t expected[];
      int max;
      fifo_reg FIFO;

      ovm_test_done.raise_objection(this);

      `ovm_info("Test", "Resetting DUT and Register Model...", OVM_LOW)
      begin
         dut_reset_seq rst_seq;
         rst_seq = dut_reset_seq::type_id::create("rst_seq", this);
         rst_seq.start(null);
      end
      regmodel.reset();

      FIFO = regmodel.FIFO;
      max = FIFO.capacity();

      FIFO.set_compare(UVM_CHECK);

      `ovm_info("FIFO Example", 
        $sformatf("Initializing FIFO reg of max size %0d with set()...",max), OVM_LOW)

      expected = new[max];

      // SET - preload regmodel; remodel now has full FIFO; DUT still empty
      foreach (expected[i]) begin
        data = $urandom;
        expected[i] = data;
        FIFO.set(data);
      end

      `ovm_info("FIFO Example", 
        $sformatf("Updating DUT FIFO reg with mirror using update()..."), OVM_LOW)

      // UPDATE - write regmodel contents to DUT; DUT now has full FIFO
      FIFO.update(status);
      if (status == UVM_NOT_OK) begin
        `ovm_fatal("FIFO Update Error", "Received status UVM_NOT_OK updating DUT")
      end

      `ovm_info("FIFO Example", 
        $sformatf(" Read back DUT FIFO reg into mirror using read()..."), OVM_LOW)

      // READ - read contents of DUT back to regmodel; DUT is empty now, regmodel FULL
      foreach (expected[i]) begin
        FIFO.read(status, data);
        if (status == UVM_NOT_OK) begin
          `ovm_fatal("FIFO Read Error", "Received status UVM_NOT_OK updating Regmodel")
        end
      end

      ovm_test_done.drop_objection(this);
   endtask
endclass


initial begin
   my_test test;
   ovm_report_server svr;
   ovm_container #(apb_vif) vif = new;

   test = new("test");
   svr = _global_reporter.get_report_server();
   svr.set_max_quit_count(10);
   
   //ovm_config_db#(apb_vif)::set(test, "apb", "vif", $root.dut_top.apb0);
   vif.val = $root.dut_top.apb0;
   set_config_object("test.apb","vif",vif,0);



   run_test();
end

endmodule
