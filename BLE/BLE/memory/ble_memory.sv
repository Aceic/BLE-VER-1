`include "define.sv"
module ahb_memory_slave (ahb_if AHB);

int memory[logic[31:0]];


// Internal signals
reg [31:0] haddr1;
reg [31:0] status_register;
wire status_reg_sel;

// Address decoder
assign status_reg_sel = AHB.hclk & AHB.hsel & (AHB.haddr == `status_register);


// Status Register
always @(posedge AHB.hclk or posedge AHB.hresetn)
  begin
    if(AHB.hresetn == 0) begin
      status_register <= 0;
      AHB.hrdata <= 0;
      AHB.hready <= 0;
      AHB.hresp <= 0;
      haddr1 <=0;end
    else if (status_reg_sel & AHB.hwrite)
      haddr1 <= AHB.haddr;
   if(haddr1 == `status_register )
      status_register =  AHB.hwdata;
      
          $display("mem_data = %h  ------- intf_data = %h ------- address= %h",status_register,AHB.hwdata,haddr1);

 
  end

endmodule: ahb_memory_slave
     /*         AHB.hready <= 1;
          AHB.hresp <= 'b00;
        
          if(haddr1 == `status_register) begin // state register.
          memory[`status_register] = AHB.hwdata;
          $display("mem_data = %h  ------- intf_data = %h",memory[`status_register],AHB.hwdata);
          end

          else if(haddr2 inside {[32'h0100_0100:32'h0100_0134]}) begin // ADV state register 
          
          case(haddr2)
          `adv_types	                :  memory[`adv_types	               ] = AHB.hwdata;
          `adv_source_device_addr_lsb :  memory[`adv_source_device_addr_lsb] = AHB.hwdata;
          `adv_source_device_addr_msb :  memory[`adv_source_device_addr_msb] = AHB.hwdata;
          `adv_target_device_addr_lsb :  memory[`adv_target_device_addr_lsb] = AHB.hwdata;
          `adv_target_device_addr_msb :  memory[`adv_target_device_addr_msb] = AHB.hwdata;
          `adv_TX_Data	              :  memory[`adv_TX_Data	             ] = AHB.hwdata;
          `adv_RX_Data	              :  memory[`adv_RX_Data	             ] = AHB.hwdata;
          `adv_interval	              :  memory[`adv_interval	             ] = AHB.hwdata;
          `adv_interval_min	          :  memory[`adv_interval_min	         ] = AHB.hwdata;
          `adv_interval_max	          :  memory[`adv_interval_max	         ] = AHB.hwdata;
          `adv_channel_map_lsb	      :  memory[`adv_channel_map_lsb	     ] = AHB.hwdata;
          `adv_channel_map_msb	      :  memory[`adv_channel_map_msb	     ] = AHB.hwdata;
          `whitelist_reg	            :  memory[`whitelist_reg	           ] = AHB.hwdata; 
          `adv_rx_type	              :  memory[`adv_rx_type	             ] = AHB.hwdata;
          endcase
          AHB.hready <= 1;
          AHB.hresp  <= 'd0;
          end
          else begin
          AHB.hresp  <= 'd1; // Outside valid write address range
          end
      end
    end

end     

endmodule: ahb_memory_slave*/
