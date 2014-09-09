`define BUS_WIDTH 32

`define IDLE    2'b00
`define BUSY    2'b01
`define NONSEQ  2'b10
`define SEQ     2'b11

`define SINGLE  3'b000
`define INCR    3'b001
`define WRAP4   3'b010
`define INCR4   3'b011
`define WRAP8   3'b100
`define INCR8   3'b101
`define WRAP16  3'b110
`define INCR16  3'b111

`define OKAY    2'b00
`define ERROR   2'b01
`define RETRY   2'b10
`define SPLIT   2'b11

`define CONN_UNDIRECTED     4'b0000
`define CONN_DIRECTED_HDUTY 4'b0001
`define NON_CONN_UNDIRECTED 4'b0010
`define SCAN_UNDIRECTED     4'b0110
`define CONN_DIRECTED_LDUTY 4'b0111
`define SCAN_RSP_PDU        4'b0100

`define public 1'b0
`define random 1'b1

`define OFF 1'b0
`define ON  1'b1

//typedef enum  { OK,ERROR,RETRY,SPLIT } response;
typedef enum  {ADV_IND,ADV_DIRECT_IND_HIGH_DUTY_CYCLE,ADV_SCAN_IND,ADV_NONCONN_IND,ADV_DIRECT_IND_LOW_DUTY_CYCLE} adv_pdu_type;


   `define  adv_tx_data_base_addr     32'h200
   `define  adv_rx_data_base_addr     32'h300
   `define  tx_data_base_addr         32'h400
   `define  rx_data_base_addr         32'h500
   `define  scan_resp_data_base_addr  32'h600
   `define  whitelist_fifo_base_addr  32'h700
   `define  nonhost_fifo_base_addr    32'h800
   `define  encryption_fifo_base_addr 32'h900
   `define  decryption_fifo_base_addr 32'ha00
   `define  duplicate_fifo_base_addr  32'hb00
                                                            
   `define  adv_tx_data_depth         32'h256
   `define  adv_rx_data_depth         32'h256
   `define  tx_data_depth             32'h256
   `define  rx_data_depth             32'h256
   `define  scan_resp_data_depth      32'h256
   `define  whitelist_fifo_depth      32'h256
   `define  nonhost_fifo_depth        32'h256
   `define  encryption_fifo_depth     32'h256
   `define  decryption_fifo_depth     32'h256
   `define  duplicate_fifo_depth      32'h256
                                              
