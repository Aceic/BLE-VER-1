class ahb_seq_item extends ovm_sequence_item;
  
  	
      rand bit [`BUS_WIDTH-1 : 0] haddr;
      rand bit                    hwrite;
      rand bit [2:0]              hsize;
      rand bit [2:0]              hburst;
   //  rand bit [3:0]  hprot;
      rand bit [`BUS_WIDTH-1 : 0] hwdata[$]; 
	  			 bit [`BUS_WIDTH-1 : 0] hrdata[$];
      rand bit [4:0]              unspecified_len;

				   bit [1:0]              hresp[$];			
			rand bit [1:0]              htrans;
   //   rand bit [47:0] device_addr;
   //   rand bit addr_type;    //public 0 
                             //random 1
   //   rand bit [1:0] random_type;  // For random device address which can be of static type (00)
                                   //  Non resolvable private address type(01) or 
				                           //  Resolvable private address type (10)


			rand int no_of_write;
			rand int no_of_read;

  			   bit [`BUS_WIDTH-1 :0] haddr_q[$];
  			   bit [`BUS_WIDTH-1 :0] aligned_addr;
				   int no_of_bytes;
				   bit [`BUS_WIDTH-1 :0] lwb;
  			   bit [`BUS_WIDTH-1 :0] uwb;
				   bit [`BUS_WIDTH-1 :0] temp_addr;
 

	function new(string name = "ahb_seq_item");
		super.new(name);
	endfunction

  extern function void post_randomize();
  extern function void addr_calc(input int length);
  extern function void wrap_calc(input int length);  

  
	constraint write {no_of_write inside {[1:10]};}
	constraint read {no_of_read inside {[1:10]};}


  constraint HSIZE_BEFORE_HADDR {solve hsize before haddr;solve hburst before haddr;solve unspecified_len before haddr;}
  constraint HSIZE {hsize < 'd3 ;}
  constraint ALIGNED_HADDR {haddr % (2**hsize) == 0;}
  
  constraint VALID_ADDR {(hburst == 0) -> ((1024 - (haddr%1024)) > 2**hsize);
                         (hburst == 1) -> ((1024 - (haddr%1024)) > (unspecified_len*(2**hsize)));
                         ((hburst == 2) || (hburst == 3))  -> ((1024 - (haddr%1024)) > (4*(2**hsize)));
                         ((hburst == 4) || (hburst == 5))  -> ((1024 - (haddr%1024)) > (8*(2**hsize)));
                         ((hburst == 6) || (hburst == 7))  -> ((1024 - (haddr%1024)) > (16*(2**hsize)));
                            }
  
/*  constraint dev_addr{ random_type!=2'b11; solve random_type before device_addr;
                if(addr_type==1) 
		{ 
		 if(random_type==2'b00) {
                  device_addr[47:46]==2'b11;
		  $countones(device_addr[45:0])!=46;           // Constraint for static random address.
		  $countones(device_addr[45:0])!=0;}
		 else if(random_type==2'b01) {
		  device_addr[47:46]==2'b00;                   // Constraint for Non resolvable private address. 
		  $countones(device_addr[45:0])!=46;
		  $countones(device_addr[45:0])!=0; }
		}
	       }	

*/

  constraint HWDATA { (haddr == 'h01000000) -> ((hwdata[0][3:0] inside {[0:5]}) && (hwdata[0][4] inside {[0:1]}));
                      //(haddr == `ADV_types) -> ((hwdata[0][3:0] == types; hwdata[0][4] == addr_type ; hwdata[0][5] == filter_policy); hwdata[0][11:6] == length);
                        }
  constraint VALID_DATA {(hburst == 0) -> hwdata.size == 1;
                         (hburst == 1) -> hwdata.size == unspecified_len;
                         ((hburst == 2) || (hburst == 3)) -> hwdata.size == 4;
                         ((hburst == 4) || (hburst == 5)) -> hwdata.size == 8;
                         ((hburst == 6) || (hburst == 7)) -> hwdata.size == 16;}

  

    `ovm_object_utils_begin(ahb_seq_item)
      `ovm_field_int(hwrite,OVM_ALL_ON)
      `ovm_field_int(haddr,OVM_ALL_ON)
      `ovm_field_int(hsize,OVM_ALL_ON)
      `ovm_field_int(hburst,OVM_ALL_ON)
      `ovm_field_queue_int(haddr_q,OVM_ALL_ON)
      `ovm_field_queue_int(hwdata,OVM_ALL_ON)
			`ovm_field_queue_int(hrdata,OVM_ALL_ON)
			`ovm_field_queue_int(hresp,OVM_ALL_ON) 
   `ovm_object_utils_end
endclass

//*********************** address calculation in post_randomize **************************************

function void ahb_seq_item::post_randomize();
  if(hburst == 0)
      haddr_q.push_back(haddr);
  else
    begin
    case(hburst)
       
        `INCR   : addr_calc(unspecified_len);
        `WRAP4  : wrap_calc(4);
        `INCR4  : addr_calc(4);
        `WRAP8  : wrap_calc(8);
        `INCR8  : addr_calc(8);
        `WRAP16 : wrap_calc(16);
        `INCR16 : addr_calc(16);
    endcase
    end
endfunction 

//*******************************************************************************

function  void ahb_seq_item::addr_calc(input int length);
			 // haddr_q.size = length;
      
				temp_addr = haddr;

        for(int i=0;i<length;i++)
        begin
            haddr_q.push_back(temp_addr);
            temp_addr = temp_addr + 2**hsize;
        end
endfunction

//******************************************************************************

function void ahb_seq_item::wrap_calc(input int length);
        
			  temp_addr = haddr;
      	no_of_bytes = (length)*(2**hsize);
				aligned_addr = int'((temp_addr)/(2**hsize))*(2**hsize);
				lwb = int'(temp_addr/no_of_bytes)*no_of_bytes;
        uwb = lwb + no_of_bytes;
  
      for(int i=0;i<length;i++)
        begin
          haddr_q.push_back(temp_addr);
          temp_addr = temp_addr + 2**hsize;
          if(temp_addr >= uwb)
          temp_addr = lwb;
        end
 endfunction



