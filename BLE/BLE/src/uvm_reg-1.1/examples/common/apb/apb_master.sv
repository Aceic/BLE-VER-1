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


`ifndef APB_MASTER__SV
`define APB_MASTER__SV

typedef class apb_master;
class apb_master_cbs extends ovm_callback;
    virtual task trans_received (apb_master xactor , apb_rw cycle);endtask
    virtual task trans_executed (apb_master xactor , apb_rw cycle);endtask
endclass

class apb_master extends ovm_driver#(apb_rw);

    `ovm_component_utils(apb_master)
  
   event trig;
   apb_vif sigs;
   apb_config cfg;

   function new(string name,ovm_component parent = null);
      super.new(name,parent);
   endfunction
   
   virtual function void build();
      apb_agent agent;
      if ($cast(agent, get_parent()) && agent != null) begin
         sigs = agent.vif;
      end
      else begin
         ovm_object obj;
         ovm_container #(apb_vif) wrapper;
         if (!get_config_object("vif", obj,0)) begin
           if (!$cast(wrapper,obj)) begin
             `ovm_fatal("APB/DRV/NOVIF", "No virtual interface specified for this driver instance")
             return;
           end
           sigs = wrapper.val;
         end
         /*
         if (!ovm_config_db#(apb_vif)::get(this, "", "vif", sigs)) begin
            `ovm_fatal("APB/DRV/NOVIF", "No virtual interface specified for this driver instance")
         end
         */
      end
   endfunction

   virtual protected task run();
      super.run();

      this.sigs.mck.psel    <= '0;
      this.sigs.mck.penable <= '0;

      forever begin
         apb_rw tr;
         @ (this.sigs.mck);

         seq_item_port.get_next_item(tr);

         // TODO: QUESTA issue with hier ref to sequence via modport; need workaround?
`ifdef VCS
         if (!this.sigs.mck.at_posedge.triggered)
`endif

`ifdef INCA
          // FIXME      if (!this.sigs.mck.at_posedge.triggered) // this is wrong and has to be reviewed
`endif
`ifdef QUESTA	
	   if (!this.sigs.mck.triggered)
`endif
	    @ (this.sigs.mck);
         
         this.trans_received(tr);
         `ovm_do_callbacks(apb_master_cbs,apb_master,trans_received(this,tr))
         
	 case (tr.kind)
          apb_rw::READ:  this.read(tr.addr, tr.data);  
          apb_rw::WRITE: this.write(tr.addr, tr.data);
         endcase
         
         this.trans_executed(tr);
         `ovm_do_callbacks(apb_master_cbs,apb_master,trans_executed(this,tr))

         seq_item_port.item_done();
	 ->trig ;
      end
   endtask

   virtual protected task read(input  bit   [31:0] addr,
                               output logic [31:0] data);

      this.sigs.mck.paddr   <= addr;
      this.sigs.mck.pwrite  <= '0;
      this.sigs.mck.psel    <= '1;
      @ (this.sigs.mck);
      this.sigs.mck.penable <= '1;
      @ (this.sigs.mck);
      data = this.sigs.mck.prdata;
      this.sigs.mck.psel    <= '0;
      this.sigs.mck.penable <= '0;
   endtask: read

   virtual protected task write(input bit [31:0] addr,
                                input bit [31:0] data);
      this.sigs.mck.paddr   <= addr;
      this.sigs.mck.pwdata  <= data;
      this.sigs.mck.pwrite  <= '1;
      this.sigs.mck.psel    <= '1;
      @ (this.sigs.mck);
      this.sigs.mck.penable <= '1;
      @ (this.sigs.mck);
      this.sigs.mck.psel    <= '0;
      this.sigs.mck.penable <= '0;
   endtask: write

   virtual protected task trans_received(apb_rw tr);
   endtask
 
   virtual protected task trans_executed(apb_rw tr);
   endtask
endclass: apb_master

`endif


