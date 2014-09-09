// 
// -------------------------------------------------------------
//    Copyright 2004-2011 Synopsys, Inc.
//    Copyright 2010 Mentor Graphics Corporation
//    Copyright 2010 Cadence Design Systems, Inc.
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

program test;

import ovm_pkg::*;
import uvm_reg_pkg::*;
import apb_pkg::*;


`include "reg_model.sv"
`include "tb_env.sv"
`include "testlib.sv"


class dut_reset_seq extends ovm_sequence;

   function new(string name = "dut_reset_seq");
      super.new(name);
   endfunction

   `ovm_object_utils(dut_reset_seq)
   
   virtual task body();
      tb_top.rst = 1;
      repeat (5) @(negedge tb_top.clk);
      tb_top.rst = 0;
   endtask
endclass


initial
begin
   static tb_env env = new("env");

   ovm_container #(apb_vif) vif = new;
   vif.val = $root.tb_top.apb0;
   set_config_object("env.apb","vif",vif,0);

   run_test();

end

endprogram

