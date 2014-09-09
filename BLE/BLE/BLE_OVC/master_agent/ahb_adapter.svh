//
// -------------------------------------------------------------
//    Copyright 2004-2008 Synopsys, Inc.
//    Copyright 2010 Mentor Graphics Corp.
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
class ahb_adapter extends uvm_reg_adapter;

  `uvm_object_utils(ahb_adapter)

   function new(string name = "ahb_adapter");
      super.new(name);
 //     supports_byte_enable = 0;
 //     provides_responses = 1;
   endfunction

  virtual function uvm_sequence_item reg2bus(const ref uvm_reg_bus_op rw);
    ahb_seq_item ahb = ahb_seq_item::type_id::create("apb");
    ahb.hwrite = (rw.kind == UVM_READ) ? 0 : 1;
    ahb.haddr_q[0] = rw.addr;
    ahb.haddr = rw.addr;
    ahb.hwdata[0] = rw.data;
    ahb.hburst = `SINGLE;
    ahb.hsize = 2;
    return ahb;
  endfunction: reg2bus

  virtual function void bus2reg(uvm_sequence_item bus_item,
                                ref uvm_reg_bus_op rw);
    ahb_seq_item ahb;
    if (!$cast(ahb, bus_item)) begin
      `uvm_fatal("NOT_AHB_TYPE","Provided bus_item is not of the correct type")
      return;
    end
    rw.kind = ahb.hwrite ? UVM_WRITE : UVM_READ;
    rw.addr = ahb.haddr_q[0];
    rw.data = ahb.hwdata[0];
    rw.status = UVM_IS_OK;
  endfunction: bus2reg

endclass: ahb_adapter

