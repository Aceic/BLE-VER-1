//
// -------------------------------------------------------------
//    Copyright 2011 Mentor Graphics Corporation
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

`ifndef UVM_REG_PKG__SV
`define UVM_REG_PKG__SV

`include "ovm_macros.svh"
`include "uvm_reg_macros.svh"

package uvm_reg_pkg;

import ovm_pkg::*;

parameter string uvm_reg_mgc_copyright  = "(C) 2007-2011 Mentor Graphics Corporation";
parameter string uvm_reg_cdn_copyright  = "(C) 2007-2011 Cadence Design Systems, Inc.";
parameter string uvm_reg_snps_copyright = "(C) 2006-2011 Synopsys, Inc.";
parameter string uvm_reg_cy_copyright   = "(C) 2011      Cypress Semiconductor Corp.";
parameter string uvm_reg_revision = `UVM_REG_VERSION_STRING;

function string uvm_reg_revision_string();
  return uvm_reg_revision;
endfunction


function void uvm_report_info(string id, string msg, int ver=OVM_INFO, string file="", int line=0);
  ovm_report_info(id,msg,ver,file,line);
endfunction

function void uvm_report_warning(string id, string msg, int ver=OVM_MEDIUM, string file="", int line=0);
  ovm_report_warning(id,msg,ver,file,line);
endfunction

function void uvm_report_error(string id, string msg, int ver=OVM_MEDIUM, string file="", int line=0);
  ovm_report_error(id,msg,ver,file,line);
endfunction

function void uvm_report_fatal(string id, string msg, int ver=OVM_NONE, string file="", int line=0);
  ovm_report_fatal(id,msg,ver,file,line);
endfunction

typedef ovm_component uvm_component;
typedef ovm_object uvm_object;
typedef ovm_root uvm_root;
typedef ovm_printer uvm_printer;
typedef ovm_comparer uvm_comparer;
typedef ovm_packer uvm_packer;
typedef ovm_recorder uvm_recorder;
typedef ovm_sequence_base uvm_sequence_base;
typedef ovm_object_wrapper uvm_object_wrapper;
typedef ovm_sequence_item uvm_sequence_item;
typedef ovm_sequencer_base uvm_sequencer_base;
typedef ovm_void uvm_phase;
typedef ovm_report_object uvm_report_object;
typedef ovm_bitstream_t uvm_bitstream_t;
typedef ovm_event_pool uvm_event_pool;
typedef ovm_event uvm_event;

typedef class uvm_tlm_gp;

parameter int UVM_ALL_ON = OVM_ALL_ON;
parameter int UVM_DEFAULT = OVM_DEFAULT;
parameter int UVM_NONE = OVM_NONE;
parameter int UVM_LOW = OVM_LOW;
parameter int UVM_MEDIUM = OVM_MEDIUM;
parameter int UVM_HIGH = OVM_HIGH;
parameter int UVM_FULL = OVM_FULL;
parameter int UVM_DEBUG = OVM_DEBUG;
parameter int UVM_UNSIGNED = OVM_UNSIGNED;


class uvm_pool #(type KEY=int,T=int) extends ovm_pool #(KEY,T);
  `uvm_object_param_utils(uvm_pool #(KEY,T))
  function new(string name="");
    super.new(name);
  endfunction
endclass


class uvm_object_string_pool #(type T=int) extends ovm_object_string_pool #(T);
  `uvm_object_param_utils(uvm_object_string_pool #(T))
  function new(string name="");
    super.new(name);
  endfunction
endclass

class uvm_queue #(type T=int) extends ovm_queue #(T);
  `uvm_object_param_utils(uvm_queue #(T))
  function new(string name="");
    super.new(name);
  endfunction
endclass

class uvm_sequence #(type REQ=uvm_sequence_item,RSP=REQ) extends ovm_sequence #(REQ,RSP);
  `uvm_object_param_utils(uvm_sequence #(REQ,RSP))
  function new(string name="");
    super.new(name);
  endfunction
endclass

// the UVM sequencer is factory registered
class uvm_sequencer #(type REQ=uvm_sequence_item,RSP=REQ) extends ovm_sequencer #(REQ,RSP);
  `uvm_component_param_utils(uvm_sequencer #(REQ,RSP))
  function new(string name,uvm_component parent=null);
    super.new(name,parent);
  endfunction
endclass

class uvm_driver #(type REQ=uvm_sequence_item,RSP=REQ) extends ovm_driver #(REQ,RSP);
  function new(string name,uvm_component parent=null);
    super.new(name,parent);
  endfunction
endclass

`ifndef UVM_REG_SEQR
`define UVM_REG_SEQR ovm_sequencer
`endif



// works only for T = integral -- it's all the UVM reg package uses
class uvm_resource_db #(type T=int);

  static function bit read_by_name(string scope, string name, inout T val, input uvm_object accessor=null);
    uvm_root top = uvm_root::get();
    uvm_bitstream_t i;
    if (top.get_config_int({scope,".",name},i)) begin
      val = i;
      return 1;
    end
    return 0;
  endfunction

  static function void set(input string scope, input string name,
                           T val, input uvm_object accessor = null);
    set_config_int("*", {scope, ".", name}, val);
  endfunction


  static function uvm_object get_by_name(string scope,
                                     string name,
                                     bit rpterr=1);

    uvm_root top = uvm_root::get();
    uvm_bitstream_t i;
    if (top.get_config_int({scope,".",name}, i)) begin
      // return any non-null object (reg layer only calls get_by_name to
      // check for existence of setting; does not use resource API)
      return uvm_root::get();
    end
    else 
      if (rpterr)
        `uvm_error("get_by_name", {"No set with scope '",scope,"' and field name '",name,"' found"})
    return null;
  endfunction


endclass


class uvm_analysis_port #(type T=int) extends ovm_analysis_port #(T);
  function new(string name,uvm_component parent=null);
    super.new(name,parent);
  endfunction
endclass

class uvm_analysis_imp #(type T=int,IMP=ovm_component) extends ovm_analysis_imp #(T,IMP);
  function new(string name,IMP parent=null);
    super.new(name,parent);
  endfunction
endclass


function bit uvm_report_enabled (int verbosity,
                                 ovm_severity severity=OVM_INFO, string id="");
  return ovm_top.ovm_report_enabled(verbosity,severity,id);
endfunction


`include "base/uvm_callback.svh"
`include "dpi/uvm_dpi.svh"
`include "tlm2/tlm2_generic_payload.svh"
`include "reg/uvm_reg_model.svh"

endpackage

`endif // UVM_REG_PKG__SV

