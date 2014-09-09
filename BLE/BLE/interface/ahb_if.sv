
interface ahb_if(input bit hclk,hresetn);
  
  `include "tb_defs.sv"

  logic                       hbusreq;
  logic                       hlock;
  logic                       hgrant = 1;
  logic                       hsel ;

  logic   [15:0]              hsplit;
  logic   [1:0]               htrans;
  logic   [`BUS_WIDTH-1 : 0]  haddr;
  logic                       hwrite;
  logic   [2:0]               hsize; // = 2;
  logic   [2:0]               hburst; //= `SINGLE;
  logic   [3:0]               hprot;
  logic   [`BUS_WIDTH-1 : 0]  hwdata;

  logic                       hready;
  logic   [1:0]               hresp; // = `OKAY ;
  logic   [`BUS_WIDTH-1 : 0]  hrdata;

  logic   [3:0]               hmaster;
  logic                       hmastlock;



//******************** master driver clcoking block *****************************


clocking mdr_cb @(posedge hclk);
		default input #0 output #1;

  input hgrant;
  input hready;
  input hresp;
  input hrdata;

  output hbusreq;
  output hlock;
  output htrans;
  output haddr;
  output hwrite;
  output hsize;
  output hburst;
  output hprot;
  output hwdata;
  

endclocking


//*************** master monitor clocking block ********************************

clocking mmon_cb @(posedge hclk);
    default input #0 output #1;

  input hgrant;
  input hready;
  input hresp;
  input hrdata;

  input hbusreq;
  input hlock;
  input htrans;
  input haddr;
  input hsize;
  input hwrite;
  input hburst;
  input hprot;
  input hwdata;

endclocking



//****************************************************************************

modport MDR_MP (clocking mdr_cb,input hresetn);
modport MMON_MP (clocking mmon_cb,input hresetn);

endinterface
