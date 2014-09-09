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
`include "apb.sv"
`include "tb_top.sv"

program test;

import ovm_pkg::*;
import uvm_reg_pkg::*;
import apb_pkg::*;

`include "regmodel.sv"
`include "tb_env.sv"

initial
begin
   static tb_env env = new("env");

   begin
     ovm_report_server svr;
     svr = _global_reporter.get_report_server();
     svr.set_max_quit_count(10);
   end

   begin
     string seq_name;
     if ($value$plusargs("OVM_SEQUENCE=%s",seq_name)) begin
       uvm_reg_sequence seq;
       seq = uvm_utils #(uvm_reg_sequence)::create_type_by_name(seq_name,"tb");
       if (seq == null) 
         ovm_report_fatal("NO_SEQUENCE",
           "This env requires you to specify the sequence to run using OVM_SEQUENCE=<name>");
       env.seq = seq;
     end
   end

   begin
     //uvm_config_db#(apb_vif)::set(env, "apb", "vif", $root.tb_top.apb0);
     ovm_container #(apb_vif) vif = new;
     vif.val = $root.tb_top.apb0;
     set_config_object("env.apb","vif",vif,0);
   end

   run_test();
end

endprogram
