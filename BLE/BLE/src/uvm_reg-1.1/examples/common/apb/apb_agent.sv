// 
// -------------------------------------------------------------
//    Copyright 2004-2011 Synopsys, Inc.
//    Copyright 2010-2011 Mentor Graphics Corporation
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


`ifndef APB_AGENT__SV
`define APB_AGENT__SV


typedef class apb_agent;


class apb_agent extends ovm_agent;

   apb_sequencer sqr;
   apb_master    drv;
   apb_monitor   mon;

   apb_vif       vif;

   `ovm_component_utils_begin(apb_agent)
      `ovm_field_object(sqr, OVM_ALL_ON)
      `ovm_field_object(drv, OVM_ALL_ON)
      `ovm_field_object(mon, OVM_ALL_ON)
   `ovm_component_utils_end
   
   function new(string name, ovm_component parent = null);
      super.new(name, parent);
   endfunction

   virtual function void build();
      sqr = apb_sequencer::type_id::create("sqr", this);
      drv = apb_master::type_id::create("drv", this);
      mon = apb_monitor::type_id::create("mon", this);
      
      begin
         ovm_object obj;
         ovm_container #(apb_vif) wrapper;
         if (get_config_object("vif", obj,0)) begin
           if (!$cast(wrapper,obj)) begin
             `ovm_fatal("APB/DRV/NOVIF", "config object with name 'vif' not of type ovm_container#(apb_vif)")
             return;
           end
         end
         else begin
           `ovm_fatal("APB/DRV/NOVIF", "No virtual interface specified for this monitor instance")
           return;
         end
         vif = wrapper.val;
       end

      /*
      if (!ovm_config_db#(apb_vif)::get(this, "", "vif", vif)) begin
         `ovm_fatal("APB/AGT/NOVIF", "No virtual interface specified for this agent instance")
      end
      */
   endfunction

   virtual function void connect();
      drv.seq_item_port.connect(sqr.seq_item_export);
   endfunction
endclass: apb_agent

`endif


