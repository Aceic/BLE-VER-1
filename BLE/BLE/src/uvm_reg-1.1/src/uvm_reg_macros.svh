//----------------------------------------------------------------------
//   Copyright 2011 Mentor Graphics Corporation
//   All Rights Reserved Worldwide
//
//   Licensed under the Apache License, Version 2.0 (the
//   "License"); you may not use this file except in
//   compliance with the License.  You may obtain a copy of
//   the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in
//   writing, software distributed under the License is
//   distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//   CONDITIONS OF ANY KIND, either express or implied.  See
//   the License for the specific language governing
//   permissions and limitations under the License.
//----------------------------------------------------------------------

`ifndef UVM_REG_MACROS__SVH
`define UVM_REG_MACROS__SVH


// Version numbers to be used in creating version strings for printing
// or programmatic tesing against version numbers
`define UVM_REG_NAME UVM_REG
`define UVM_REG_MAJOR_REV 1
`define UVM_REG_MINOR_REV 1
`define UVM_REG_FIX_REV 


// Whole version identifiers that can be used in `ifdefs and `ifndefs
// to do conditional compilation
`define UVM_REG_VERSION_1_1
`define UVM_REG_MAJOR_VERSION_1_1
`define UVM_REG_MAJOR_REV_1
`define UVM_REG_MINOR_REV_1

// When there is a FIX_REV, print as "M.mf"
// When there is NO FIX_REV, print as "M.m".
// Fix rev kind of string:
//`define UVM_REG_VERSION_STRING `"`UVM_NAME``-```UVM_MAJOR_REV``.```UVM_MINOR_REV`UVM_FIX_REV`"
// No fix rev kind of string:
`define UVM_REG_VERSION_STRING `"`UVM_NAME``-```UVM_MAJOR_REV``.```UVM_MINOR_REV```"


`include "macros/uvm_callback_defines.svh"

`include "macros/uvm_reg_defines.svh"

`define UVM_DA_TO_QUEUE(Q,DA) Q=DA;


`define uvm_info(A,B,C)    `ovm_info(A,B,C)
`define uvm_warning(A,B)   `ovm_warning(A,B)
`define uvm_error(A,B)     `ovm_error(A,B)
`define uvm_fatal(A,B)     `ovm_fatal(A,B)
`define uvm_file           `ovm_file
`define uvm_line           `ovm_line

`define uvm_object_utils(TYPE)    `ovm_object_utils(TYPE)
`define uvm_object_param_utils(TYPE)    `ovm_object_param_utils(TYPE)
`define uvm_component_utils(TYPE) `ovm_component_utils(TYPE)
`define uvm_component_param_utils(TYPE) `ovm_component_param_utils(TYPE)

`define uvm_object_utils_begin(TYPE)    `ovm_object_utils_begin(TYPE)
`define uvm_object_utils_end `ovm_object_utils_end
`define uvm_component__begin(TYPE)    `ovm_component_utils_begin(TYPE)
`define uvm_component_utils_end `ovm_component_utils_end

`define uvm_field_int(A,B) `ovm_field_int(A,B)
`define uvm_field_enum(A,B,C) `ovm_field_enum(A,B,C)
`define uvm_field_array_int(A,B) `ovm_field_array_int(A,B)
`define uvm_field_queue_string(A,B) `ovm_field_queue_string(A,B)

`endif // UVM_REG_MACROS__SVH
