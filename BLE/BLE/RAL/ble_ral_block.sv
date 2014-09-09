//`ifndef ble_ral_block
//`define ble_ral_block


class ble_ral_block extends ovm_object;
  `ovm_object_utils(ble_ral_block)

  uvm_status_e status;

`include "tb_defs.sv"
 bit  adv_tx_data_full;    
 bit  adv_rx_data_full;    
 bit  tx_data_full;        
 bit  rx_data_full;        
 bit  scan_resp_data_full; 
 bit  whitelist_fifo_full; 
 bit  nonhost_fifo_full;  
 bit  encryption_fifo_full;
 bit  decryption_fifo_full;
 bit  duplicate_fifo_full; 


 bit  adv_tx_data_empty;    
 bit  adv_rx_data_empty;    
 bit  tx_data_empty;        
 bit  rx_data_empty;        
 bit  scan_resp_data_empty; 
 bit  whitelist_fifo_empty; 
 bit  nonhost_fifo_empty;  
 bit  encryption_fifo_empty;
 bit  decryption_fifo_empty;
 bit  duplicate_fifo_empty; 

 
     

   ble_reg_block ble_rm;    // Register Block  (address range 32'h00 to 32'he4) 


// Each memory is declared in form of Associative Array
 
   ovm_pool #(bit [31:0],bit [7:0])  adv_tx_data     ;      // `adv_tx_data_base_addr    = 'h200
   ovm_pool #(bit [31:0],bit [7:0])  adv_rx_data     ;      // `adv_rx_data_base_addr    = 'h300
   ovm_pool #(bit [31:0],bit [7:0])  tx_data         ;      // `tx_data_base_addr        = 'h400
   ovm_pool #(bit [31:0],bit [7:0])  rx_data         ;      // `rx_data_base_addr        = 'h500
   ovm_pool #(bit [31:0],bit [7:0])  scan_resp_data  ;      // `scan_resp_data_base_addr = 'h600
   ovm_pool #(bit [31:0],bit [7:0])  whitelist_fifo  ;      // `whitelist_fif0           = 'h700
   ovm_pool #(bit [31:0],bit [7:0])  nonhost_fifo    ;      // `nonhost_fifo             = 'h800
   ovm_pool #(bit [31:0],bit [7:0])  encryption_fifo ;      // `encryption_fifo          = 'h900
   ovm_pool #(bit [31:0],bit [7:0])  decryption_fifo ;      // `decryption_fifo          = 'ha00
   ovm_pool #(bit [31:0],bit [7:0])  duplicate_fifo  ;      // `duplicate_fifo           = 'hb00
                                                            
                                                            // Note :: Each memory has 256 byte depth

   
//Constructor including ble_reg_block build method

  function new(string name = "ble_ral_block");
      super.new(name);
      adv_tx_data     = new("adv_tx_data");
      adv_rx_data     = new("adv_rx_data");
      tx_data         = new("tx_data");        
      rx_data         = new("rx_data");
      scan_resp_data  = new("scan_resp_data");
      whitelist_fifo  = new("whitelist_fifo");
      nonhost_fifo    = new("nonhost_fifo");
      encryption_fifo = new("encryption_fifo");
      decryption_fifo = new("decryption_fifo");
      duplicate_fifo  = new("duplicate_fifo");
     // ble_rm = ble_reg_block::type_id::create("ble_rm");
     // ble_rm.build();
  
   endfunction
   

   task reset();
        ble_rm.reset();
        adv_tx_data.delete_all();    
        adv_rx_data.delete_all();    
        tx_data.delete_all();        
        rx_data.delete_all();        
        scan_resp_data.delete_all(); 
        whitelist_fifo.delete_all(); 
        nonhost_fifo.delete_all();   
        encryption_fifo.delete_all();
        decryption_fifo.delete_all();
        duplicate_fifo.delete_all(); 
   endtask

//API for register write

   task reg_write(input bit[31:0] addr, input bit[31:0] wdata);
       uvm_reg reg_ptr;  
       reg_ptr=ble_rm.bt_map.get_reg_by_offset(addr);
       if(reg_ptr==null)
         `ovm_error("BLE_RAL_BLK",$psprintf("Register does not exist for the address =%0d",addr))  
       else
         reg_ptr.write(status,wdata,UVM_FRONTDOOR,.parent(null)); 
   endtask: reg_write


//API for Register Read
   task reg_read(input bit[31:0] addr, output bit[31:0] rdata);
       uvm_reg reg_ptr;  
       reg_ptr=ble_rm.bt_map.get_reg_by_offset(addr);
       if(reg_ptr==null)
         `ovm_error("BLE_RAL_BLK",$psprintf("Register does not exist for the address =%0d",addr))  
       else
         //reg_ptr.read(status,rdata,UVM_FRONTDOOR,.parent(null)); 
         rdata = reg_ptr.get(); 
   endtask: reg_read

//API for register set

   task reg_set(input bit[31:0] addr, input bit[31:0] wdata);
       uvm_reg reg_ptr;  
       reg_ptr=ble_rm.bt_map.get_reg_by_offset(addr);
       if(reg_ptr==null)
         `ovm_error("BLE_RAL_BLK",$psprintf("Register does not exist for the address =%0d",addr))  
       else
         reg_ptr.set(wdata); 
   endtask: reg_set


//API for Register get

   task reg_get(input bit[31:0] addr, output bit[31:0] rdata);
       uvm_reg reg_ptr;  
       reg_ptr=ble_rm.bt_map.get_reg_by_offset(addr);
       if(reg_ptr==null)
         `ovm_error("BLE_RAL_BLK",$psprintf("Register does not exist for the address =%0d",addr))  
       else
         rdata = reg_ptr.get(); 
   endtask: reg_get




// API for memory write
    task mem_write(input bit[31:0] addr, input bit[7:0] wdata[$]);
       case(addr) inside
       [`adv_tx_data_base_addr : `adv_tx_data_base_addr + `adv_tx_data_depth - 1]  :
                           begin
                               foreach(wdata[i]) 
                                   begin
                                      if((addr + i) == (`adv_tx_data_base_addr + `adv_tx_data_depth))
                                         begin
                                          adv_tx_data_full = 1;          //setting flag high
                                          ovm_report_info("BLE_RAL","adv_tx_data memory is full",OVM_MEDIUM);
                                         end
                                      else
                                         adv_tx_data.add(addr+i,wdata[i]);
                                    end
                           end 
   
      [`adv_rx_data_base_addr : (`adv_rx_data_base_addr + `adv_rx_data_depth - 1)]  : 
                           begin
                                foreach(wdata[i]) 
                                   begin
                                      if((addr + i) == (`adv_rx_data_base_addr + `adv_rx_data_depth))
                                         begin
                                          adv_rx_data_full = 1;          //setting flag high
                                          ovm_report_info("BLE_RAL","adv_rx_data memory is full",OVM_MEDIUM);
                                         end
                                      else                                                                                                                                                                    adv_rx_data.add(addr+i,wdata[i]);
                                    end 
                            end 
       [`tx_data_base_addr : (`tx_data_base_addr + `tx_data_depth - 1)]               :
                           begin
                                foreach(wdata[i]) 
                                  begin
                                    if((addr + i) == (`tx_data_base_addr + `tx_data_depth))
                                       begin
                                          tx_data_full = 1;          //setting flag high
                                          ovm_report_info("BLE_RAL","tx_data memory is full",OVM_MEDIUM);
                                         end
                                    else
                                       tx_data.add(addr+i,wdata[i]);
                                  end
                           end 
       [`rx_data_base_addr : (`rx_data_base_addr + `rx_data_depth - 1)]               :
                            begin
                                 foreach(wdata[i]) 
                                   begin
                                     if((addr + i) == (`rx_data_base_addr + `rx_data_depth))
                                      begin
                                          rx_data_full = 1;          //setting flag high
                                          ovm_report_info("BLE_RAL","rx_data memory is full",OVM_MEDIUM);
                                         end
                                    else
                                        rx_data.add(addr+i,wdata[i]);
                                   end
                           end 
       [`scan_resp_data_base_addr : (`scan_resp_data_base_addr + `scan_resp_data_depth-1)]  :
                           begin
                                 foreach(wdata[i]) 
                                    begin
                                     if((addr + i) == (`scan_resp_data_base_addr + `scan_resp_data_depth))
                                        begin
                                          scan_resp_data_full = 1;          //setting flag high
                                          ovm_report_info("BLE_RAL","scan_resp_data memory is full",OVM_MEDIUM);
                                         end
                                     else
                                        scan_resp_data.add(addr+i,wdata[i]);
                                    end
                           end 
       [`whitelist_fifo_base_addr : (`whitelist_fifo_base_addr + `whitelist_fifo_depth - 1)]  :
                           begin
                                foreach(wdata[i]) 
                                    begin
                                      if((addr + i) == (`whitelist_fifo_base_addr + `whitelist_fifo_depth))
                                        begin
                                          whitelist_fifo_full = 1;          //setting flag high
                                          ovm_report_info("BLE_RAL","whitelist_fifo memory is full",OVM_MEDIUM);
                                         end
                                      else
                                        whitelist_fifo.add(addr+i,wdata[i]);
                                    end
                           end 

       [`nonhost_fifo_base_addr : (`nonhost_fifo_base_addr + `nonhost_fifo_depth - 1)]  :
                          begin
                                foreach(wdata[i]) 
                                   begin
                                      if((addr + i) == (`nonhost_fifo_base_addr + `nonhost_fifo_depth))
                                         begin
                                          nonhost_fifo_full = 1;          //setting flag high
                                          ovm_report_info("BLE_RAL","nonhost_fifo memory is full",OVM_MEDIUM);
                                         end
                                      else
                                         nonhost_fifo.add(addr+i,wdata[i]);
                                   end
                           end 
       [`encryption_fifo_base_addr : (`encryption_fifo_base_addr + `encryption_fifo_depth - 1)]  :
                         begin
                                foreach(wdata[i]) 
                                    begin
                                       if((addr + i) == (`encryption_fifo_base_addr + `encryption_fifo_depth))
                                          begin
                                          encryption_fifo_full = 1;          //setting flag high
                                          ovm_report_info("BLE_RAL","encryption_fifo memory is full",OVM_MEDIUM);
                                         end
                                       else
                                          encryption_fifo.add(addr+i,wdata[i]);
                                     end
                         end 
       [`decryption_fifo_base_addr : (`decryption_fifo_base_addr + `decryption_fifo_depth - 1)]  :
                         begin
                               foreach(wdata[i]) 
                                   begin
                                      if((addr + i) == (`decryption_fifo_base_addr + `decryption_fifo_depth))
                                          begin
                                          decryption_fifo_full = 1;          //setting flag high
                                          ovm_report_info("BLE_RAL","decryption_fifo memory is full",OVM_MEDIUM);
                                         end
                                      else
                                          decryption_fifo.add(addr+i,wdata[i]);
                                   end
                          end 
      [`duplicate_fifo_base_addr : (`duplicate_fifo_base_addr + `duplicate_fifo_depth - 1)]  : 
                        begin
                              foreach(wdata[i]) 
                                  begin
                                     if((addr + i) == (`duplicate_fifo_base_addr + `duplicate_fifo_depth))
                                          begin
                                          duplicate_fifo_full = 1;          //setting flag high
                                          ovm_report_info("BLE_RAL","duplicate_fifo memory is full",OVM_MEDIUM);
                                         end
                                     else
                                          duplicate_fifo.add(addr+i,wdata[i]);
                                   end
                        end 
                  default   : begin
                                   ovm_report_info("MEM_WRITE_BLOCK", "invalid addresss for memory",OVM_MEDIUM);
                              end

            endcase
   endtask: mem_write




//API for memory read with empty logic included

   task mem_read(input bit[31:0] addr, input int len=0 ,output bit[7:0] rdata[$]);
       case(addr) inside
            [`adv_tx_data_base_addr : `adv_tx_data_base_addr + `adv_tx_data_depth - 1]: 
                      begin
                        if(adv_tx_data.num() == len) 
                          begin
                            for(int i=0;i<len;i++) 
                              begin
                                rdata[i]=adv_tx_data.get(addr+i);
                              end
                          end
                        else
                             begin
                                adv_tx_data_empty = 1;          //setting flag high
                                ovm_report_info("BLE_RAL","adv_tx_data memory is empty",OVM_MEDIUM);
                             end
                       end
     [`adv_rx_data_base_addr : (`adv_rx_data_base_addr + `adv_rx_data_depth - 1)]: 
                      begin
                        if(adv_rx_data.num() == len) 
                          begin
                            for(int i=0;i<len;i++) 
                              begin
                                rdata[i]=adv_rx_data.get(addr+i);
                              end
                          end
                        else
                            begin
                                adv_rx_data_empty = 1;          //setting flag high
                                ovm_report_info("BLE_RAL","adv_rx_data memory is empty",OVM_MEDIUM);
                             end

                       end
 [`tx_data_base_addr : (`tx_data_base_addr + `tx_data_depth - 1)]: 
                      begin
                        if(tx_data.num() == len) 
                          begin
                            for(int i=0;i<len;i++) 
                              begin
                                rdata[i]=tx_data.get(addr+i);
                              end
                          end
                        else
                            begin
                                tx_data_empty = 1;          //setting flag high
                                ovm_report_info("BLE_RAL","tx_data memory is empty",OVM_MEDIUM);
                             end
 
                       end
 [`rx_data_base_addr : (`rx_data_base_addr + `rx_data_depth - 1)]: 
                      begin
                        if(rx_data.num() == len) 
                          begin
                            for(int i=0;i<len;i++) 
                              begin
                                rdata[i]=rx_data.get(addr+i);
                              end
                          end
                        else
                            begin
                                rx_data_empty = 1;          //setting flag high
                                ovm_report_info("BLE_RAL","rx_data memory is empty",OVM_MEDIUM);
                             end
                       end
 [`scan_resp_data_base_addr : (`scan_resp_data_base_addr + `scan_resp_data_depth - 1)]: 
                      begin
                        if(scan_resp_data.num() == len) 
                          begin
                            for(int i=0;i<len;i++) 
                              begin
                                rdata[i]=scan_resp_data.get(addr+i);
                              end
                          end
                        else
                            begin
                                scan_resp_data_empty = 1;          //setting flag high
                                ovm_report_info("BLE_RAL","scan_resp_data memory is empty",OVM_MEDIUM);
                             end
 
                       end
 [`whitelist_fifo_base_addr : (`whitelist_fifo_base_addr + `whitelist_fifo_depth - 1)]: 
                      begin
                        if(whitelist_fifo.num() == len) 
                          begin
                            for(int i=0;i<len;i++) 
                              begin
                                rdata[i]=whitelist_fifo.get(addr+i);
                              end
                          end
                        else
                            begin
                                whitelist_fifo_empty = 1;          //setting flag high
                                ovm_report_info("BLE_RAL","whitelist_fifo memory is empty",OVM_MEDIUM);
                             end
                       end
 [`nonhost_fifo_base_addr : (`nonhost_fifo_base_addr + `nonhost_fifo_depth - 1)]: 
                      begin
                        if(nonhost_fifo.num() == len) 
                          begin
                            for(int i=0;i<len;i++) 
                              begin
                                rdata[i]=nonhost_fifo.get(addr+i);
                              end
                          end
                        else
                            begin
                                nonhost_fifo_empty = 1;          //setting flag high
                                ovm_report_info("BLE_RAL","nonhost_fifo memory is empty",OVM_MEDIUM);
                             end
                       end
 [`encryption_fifo_base_addr : (`encryption_fifo_base_addr + `encryption_fifo_depth - 1)]: 
                      begin
                        if(encryption_fifo.num() == len) 
                          begin
                            for(int i=0;i<len;i++) 
                              begin
                                rdata[i]=encryption_fifo.get(addr+i);
                              end
                          end
                        else
                            begin
                                encryption_fifo_empty = 1;          //setting flag high
                                ovm_report_info("BLE_RAL","encryption_fifo memory is empty",OVM_MEDIUM);
                             end
                       end
 [`decryption_fifo_base_addr : (`decryption_fifo_base_addr + `decryption_fifo_depth - 1)]: 
                      begin
                        if(decryption_fifo.num() == len) 
                          begin
                            for(int i=0;i<len;i++) 
                              begin
                                rdata[i]=decryption_fifo.get(addr+i);
                              end
                          end
                        else
                            begin
                                decryption_fifo_empty = 1;          //setting flag high
                                ovm_report_info("BLE_RAL","decryption_fifo memory is empty",OVM_MEDIUM);
                             end
                       end
 [`duplicate_fifo_base_addr : (`duplicate_fifo_base_addr + `duplicate_fifo_depth - 1)]: 
                      begin
                        if(duplicate_fifo.num() == len) 
                          begin
                            for(int i=0;i<len;i++) 
                              begin
                                rdata[i]=duplicate_fifo.get(addr+i);
                              end
                          end
                        else
                            begin
                                duplicate_fifo_empty = 1;          //setting flag high
                                ovm_report_info("BLE_RAL","duplicate_fifo memory is empty",OVM_MEDIUM);
                             end
                       end
      default :  begin
                       ovm_report_info("MEM_READ_BLOCK", "invalid addresss for memory",OVM_MEDIUM);
                 end

       endcase
   endtask: mem_read
endclass
//`endif

