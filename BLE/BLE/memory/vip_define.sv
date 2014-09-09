

// State register
`define rstatus_register             32'h0100_0000

// ADV state register 

`define radv_types	                   32'h0100_0100
`define radv_source_device_addr_lsb	   32'h0100_0104
`define radv_source_device_addr_msb	   32'h0100_0108
`define radv_target_device_addr_lsb	   32'h0100_010c 
`define radv_target_device_addr_msb	   32'h0100_0110 
`define radv_TX_Data	                 32'h0100_0114 
`define radv_RX_Data	                 32'h0100_0118  
`define radv_interval	                 32'h0100_011c  
`define radv_interval_min	             32'h0100_0120 
`define radv_interval_max	             32'h0100_0124 
`define radv_channel_map_lsb	         32'h0100_0128 
`define radv_channel_map_msb	         32'h0100_012c 
`define rwhitelist_reg	               32'h0100_0130 
`define radv_rx_type	                 32'h0100_0134 

