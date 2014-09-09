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


`ifndef APB_MONITOR__SV
`define APB_MONITOR__SV


typedef class apb_monitor;

class apb_monitor_cbs extends ovm_callback;
  virtual function void trans_observed(apb_monitor xactor,apb_rw cycle);
  endfunction:trans_observed
endclass: apb_monitor_cbs


class apb_monitor extends ovm_monitor;
   virtual apb_if.passive sigs;

   ovm_analysis_port#(apb_rw) ap;
   apb_config cfg;

   `ovm_component_utils(apb_monitor)

   function new(string name, ovm_component parent = null);
      super.new(name, parent);
      ap = new("ap", this);
   endfunction: new

   virtual function void build();
      apb_agent agent;
      if ($cast(agent, get_parent()) && agent != null) begin
         sigs = agent.vif;
      end
      else begin
         virtual apb_if tmp;
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
         sigs = wrapper.val;
         /*
         if (!ovm_config_db#(apb_vif)::get(this, "", "vif", tmp)) begin
            `ovm_fatal("APB/MON/NOVIF", "No virtual interface specified for this monitor instance")
         end
         sigs = tmp;
         */
      end
   endfunction


   virtual protected task run();
      super.run();
      forever begin
         apb_rw tr;
         
         // Wait for a SETUP cycle
         do begin
            @ (this.sigs.pck);
         end
         while (this.sigs.pck.psel !== 1'b1 ||
                this.sigs.pck.penable !== 1'b0);

         tr = apb_rw::type_id::create("tr", this);
         
         tr.kind = (this.sigs.pck.pwrite) ? apb_rw::WRITE : apb_rw::READ;
         tr.addr = this.sigs.pck.paddr;

         @ (this.sigs.pck);
         if (this.sigs.pck.penable !== 1'b1) begin
            `ovm_error("APB", "APB protocol violation: SETUP cycle not followed by ENABLE cycle");
         end
         tr.data = (tr.kind == apb_rw::READ) ? this.sigs.pck.prdata :
                                               this.sigs.pck.pwdata;

         trans_observed(tr);
         `ovm_do_callbacks(apb_monitor_cbs,apb_monitor,trans_observed(this,tr))

         ap.write(tr);
      end
   endtask

   virtual protected task trans_observed(apb_rw tr);
   endtask

endclass: apb_monitor

`endif


