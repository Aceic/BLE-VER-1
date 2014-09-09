// 
// -------------------------------------------------------------
//    Copyright 2011 Synopsys, Inc.
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

`include "ovm_macros.svh"
`include "uvm_reg_macros.svh"
`include "tb_top.sv"
`include "apb.sv"

program test;

import ovm_pkg::*;
import uvm_reg_pkg::*;
import apb_pkg::*;

`include "tb_env.sv"
tb_env env = new("env");


class test_base extends ovm_test;
   tb_env env;

   `ovm_component_utils(test_base)
   
   function new(string name = "test_base", ovm_component parent = null);
      super.new(name, parent);
   endfunction

   function void end_of_elaboration();
      $cast(env, $root.test.env);
      env.regmodel.set_hdl_path_root("$root.tb_top.dut");
   endfunction

endclass


class test extends test_base;
   `ovm_component_utils(test)

   function new(string name = "test_base", ovm_component parent = null);
      super.new(name, parent);
   endfunction

   task run();
      uvm_status_e status;
      uvm_reg_data_t data;

      ovm_test_done.raise_objection(this);

      env.regmodel.reset();

      `ovm_info("TEST", "Checking initial values in blocks...", OVM_NONE)
      foreach (env.regmodel.blk[i]) begin
         env.regmodel.blk[i].mirror(status, UVM_CHECK);
      end

      `ovm_info("TEST", "Checking broadcast-write via front-door...", OVM_NONE)
      env.regmodel.all_blks.mode.write(status, 32'hFFFFFFFF);
      env.regmodel.all_blks.mode.write(status, 32'hFFFFFF55);
      foreach (env.regmodel.blk[i]) begin
         env.regmodel.blk[i].mirror(status, UVM_CHECK);
      end
      
      `ovm_info("TEST", "Checking broadcast-write via back-door...", OVM_NONE)
      env.regmodel.all_blks.mode.write(status, 32'h00000002, UVM_BACKDOOR);
      env.regmodel.all_blks.mode.write(status, 32'h000000AA, UVM_BACKDOOR);
      foreach (env.regmodel.blk[i]) begin
         env.regmodel.blk[i].mirror(status, UVM_CHECK);
      end
      
      ovm_test_done.drop_objection(this);
   endtask
endclass

initial begin

   //ovm_config_db#(apb_vif)::set(env, "apb", "vif", $root.tb_top.apb);
   ovm_container #(apb_vif) vif = new;
   vif.val = $root.tb_top.apb;
   set_config_object("env.apb","vif",vif,0);

   ovm_top.set_report_id_action("CFGNTS",OVM_NO_ACTION);


   run_test("test");
end

endprogram
