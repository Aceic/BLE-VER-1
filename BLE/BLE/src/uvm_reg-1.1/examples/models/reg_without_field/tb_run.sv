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

`include "ovm_macros.svh"
`include "uvm_reg_macros.svh"

program tb;

import ovm_pkg::*;
import uvm_reg_pkg::*;

`include "regmodel.sv"
`include "tb_env.sv"


class tb_test extends ovm_test;

   function new(string name = "tb_test", ovm_component parent = null);
      super.new(name, parent);
   endfunction

   
   virtual task run();
      tb_env env;
      uvm_status_e status;
      uvm_reg_data_t dat;

      ovm_test_done.raise_objection(this);
      
      if (!$cast(env, ovm_top.find("env")) || env == null) begin
         `ovm_fatal("test", "Cannot find tb_env");
      end

      env.regmodel.R.reset();
      env.regmodel.R.read(status, dat);
      if (dat != 8'h00) `ovm_error("Test", $sformatf("R is not as expected after reset: 'h%0h instead of 'h00", dat));
      env.regmodel.R.write(status, 8'hFF);
      env.regmodel.R.read(status, dat);
      if (dat != 8'hFF) `ovm_error("Test", $sformatf("R is not as expected after write: 'h%0h instead of 'hFF", dat));
      void'(env.regmodel.randomize() with { R.value == 8'hA5; });
      dat = env.regmodel.R.get();
      if (dat != 8'hA5) `ovm_error("Test", $sformatf("R is not as expected after randomize: 'h%0h instead of 'hA5", dat));
            
      ovm_test_done.drop_objection(this);
   endtask
endclass


initial begin
   static tb_env env = new("env");
   static tb_test test = new("test");
   
   ovm_report_server svr;
   svr = _global_reporter.get_report_server();
   svr.set_max_quit_count(10);
   run_test();
end

endprogram
