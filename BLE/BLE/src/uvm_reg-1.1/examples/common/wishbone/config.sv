// 
// -------------------------------------------------------------
//    Copyright 2004-2011 Synopsys, Inc.
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


class wb_cfg extends ovm_object;
   typedef enum {BYTE, WORD, DWORD, QWORD} sizes_e;
   rand sizes_e m_port_size;
   rand sizes_e m_granularity;

   typedef enum {CLASSIC, REGISTERED} cycle_types_e;
   rand cycle_types_e m_cycles;

   rand integer m_max_n_wss;

   `ovm_object_utils_begin(wb_cfg)
      `ovm_field_enum(sizes_e, m_port_size, OVM_ALL_ON)
      `ovm_field_enum(sizes_e, m_granularity, OVM_ALL_ON)
      `ovm_field_enum(cycle_types_e, m_cycles, OVM_ALL_ON)
      `ovm_field_int(m_max_n_wss, OVM_ALL_ON)
   `ovm_object_utils_end


   constraint wb_cfg_valid {
      m_granularity <= m_port_size;
      m_max_n_wss >= 0;
   }

   constraint reasonable_max_n_wss {
      m_max_n_wss == 10;
   }

   constraint supported {
      m_port_size   == DWORD;
      m_granularity == BYTE; 
      m_cycles      == CLASSIC;
   }

endclass: wb_cfg
