class ahb_master_driver extends ovm_driver #(ahb_seq_item);
  `ovm_component_utils(ahb_master_driver)

  virtual ahb_if vif;      // interface
  ahb_agent_config m_cfg;  // configuration object

  ahb_seq_item xtn;

  int length,length1;

   extern function new(string name ="ahb_master_driver",ovm_component parent);
   extern function void build();
   extern function void connect();
   extern task run();
  
   extern task send_to_slave(ahb_seq_item xtn);
   extern task drive_data(ahb_seq_item xtn);
	 extern task drive_raddr(ahb_seq_item xtn);
   extern task reset();  
   extern task address_phase(ahb_seq_item xtn);
   extern task okay_resp(ahb_seq_item xtn,input int i);
   extern task err_resp(ahb_seq_item xtn,input int i);
   extern task retry_resp(ahb_seq_item xtn,input int i);
   extern task split_resp(ahb_seq_item xtn,input int i);
 	 extern task okay_rresp(ahb_seq_item xtn,input int i);
   extern task err_rresp(ahb_seq_item xtn,input int i);
   extern task retry_rresp(ahb_seq_item xtn,input int i);
   extern task split_rresp(ahb_seq_item xtn,input int i);



endclass : ahb_master_driver


//***********************************************************************************************

  function ahb_master_driver::new(string name ="ahb_master_driver",ovm_component parent);
    super.new(name,parent);
  endfunction

//***********************************************************************************************

  function void ahb_master_driver::build();
      ovm_object tmp;
      if(!(get_config_object("ahb_agent_config",tmp)))
         `ovm_fatal("ahb_master_monitor","can not get config object....")  
      if(!$cast(m_cfg,tmp))
         `ovm_fatal("ahb_master_monitor","invalid cast....")  
  endfunction

//***********************************************************************************************

  function void ahb_master_driver::connect();
    vif = m_cfg.vif;
  endfunction

//***********************************************************************************************

  task ahb_master_driver::run();
    forever
      begin
        fork
         begin
          @(vif.mdr_cb);
              begin :drive
								
                seq_item_port.get_next_item(req);
								length = req.haddr_q.size();
								
                ovm_report_info("AHB_DRIVER",$sformatf("printing form AHB_DRIVER ....\n %s",req.sprint()),OVM_HIGH);
                send_to_slave(req);	
                seq_item_port.item_done();
             end
          end

         begin
             wait(!vif.hresetn);
             reset();
             @(vif.mdr_cb);
         end
        join_any
        disable fork;
     end
  endtask



//***********************************************************************************************

task ahb_master_driver::reset();
   vif.mdr_cb.haddr	<=	0;
   vif.mdr_cb.hwrite	<=	0;
   vif.mdr_cb.hsize	<=	0;
   vif.mdr_cb.hburst	<=	0;
   vif.mdr_cb.hprot	<=	1;
   //vif.mdr_cb.hmastlock	<=	0;
   vif.mdr_cb.htrans	<=	`IDLE;
   vif.mdr_cb.hwdata	<=	0;

endtask

//************************************************************************************************

task ahb_master_driver::send_to_slave(ahb_seq_item xtn);
      //@(vif.mdr_cb)
      vif.mdr_cb.hbusreq <= 1;
      wait(vif.mdr_cb.hgrant);
       address_phase(xtn);
        if(xtn.hwrite)
            drive_data(xtn);
        else
      		begin 
			    	 drive_raddr(xtn);
					end
    
endtask

//************************************************************************************************

task ahb_master_driver::address_phase(ahb_seq_item xtn);
    //@(vif.mdr_cb);
    vif.mdr_cb.haddr <= xtn.haddr_q[0];
    vif.hsel <= 1;
    vif.mdr_cb.hwrite <= xtn.hwrite;
    //vif.mdr_cb.hwrite <= 1;
    vif.mdr_cb.hsize <= xtn.hsize;
    vif.mdr_cb.hburst <= xtn.hburst;
    vif.mdr_cb.htrans <= xtn.htrans;// `NONSEQ;
    vif.mdr_cb.htrans <= `NONSEQ;
		
 endtask

//************************************************************************************************


task ahb_master_driver::drive_data(ahb_seq_item xtn);
       @(vif.mdr_cb);
       for(int i=0;i<length;i++)
       begin
        case(vif.hresp)
          `OKAY  : okay_resp(xtn,i);
          `ERROR : begin
                    err_resp(xtn,i);
                    break;
                   end
          `RETRY : begin
                    retry_resp(xtn,i);
                    @(vif.mdr_cb);
                     i = -1;
                    length = xtn.hwdata.size();
                        case(length)
                        	'd4 : xtn.hburst = `INCR4;
                        	'd8 : xtn.hburst = `INCR8;
                        	'd16: xtn.hburst = `INCR16;
                        	default : xtn.hburst = `INCR;
                        endcase
                        @(vif.mdr_cb);
                        wait(vif.hready && vif.hgrant);
                        address_phase(xtn);
                        //drive_data(xtn);
                        @(vif.mdr_cb);
                    end
          `SPLIT : begin
                    split_resp(xtn,i);
                    @(vif.mdr_cb);
                    i= -1;
                    length =xtn.hwdata.size();
                         case(length)
                        'd4 : xtn.hburst = `INCR4;
                        'd8 : xtn.hburst = `INCR8;
                        'd16: xtn.hburst = `INCR16;
                        default : xtn.hburst = `INCR;
                    endcase
 												@(vif.mdr_cb);
                        wait(vif.hready && vif.hgrant);
                        address_phase(xtn);
                        //drive_data(xtn);
                        @(vif.mdr_cb);

                    end
         endcase
    end
      // @(vif.mdr_cb);

 			 vif.mdr_cb.hbusreq <= 0;
    
      
endtask



//************************************************************************************************

task ahb_master_driver::okay_resp(ahb_seq_item xtn,input int i);
        wait(vif.mdr_cb.hready);

				vif.mdr_cb.hwdata <= xtn.hwdata[i];
      	
      if(xtn.haddr_q.size > (i+1))
      	begin	
          vif.mdr_cb.haddr <= xtn.haddr_q[i+1];
    		end
		  	else 
      		begin
        		vif.mdr_cb.haddr <= 0;
        		//vif.mdr_cb.hwrite <= 1;
        		vif.mdr_cb.hwrite <= 0;
        		vif.mdr_cb.hburst <= xtn.hburst;
      		end
      	if((xtn.hburst == 0) || (i == (length-1)))
       		 vif.mdr_cb.htrans <= `IDLE;
      	else
      		vif.mdr_cb.htrans <= `SEQ;
       		wait(vif.mdr_cb.hready && vif.mdr_cb.hgrant);
       		@(vif.mdr_cb);
endtask

//************************************************************************************************

task ahb_master_driver::err_resp(ahb_seq_item xtn,input int i);
    vif.mdr_cb.haddr  <=  0;
    vif.mdr_cb.hwrite <=  0;
    vif.mdr_cb.hsize  <=  0;
    vif.mdr_cb.hburst <=  0;
    vif.mdr_cb.htrans <=  `IDLE;
    vif.mdr_cb.hwdata <=  0;
    //xtn = null;
    @(vif.mdr_cb);

endtask
//************************************************************************************************

task ahb_master_driver::retry_resp(ahb_seq_item xtn,input int i);
       vif.mdr_cb.htrans  <= `IDLE;
       for(int k=0;k<(i-1);k++)
          begin
          xtn.haddr_q.delete(0);
          xtn.hwdata.delete(0);
          end   
      
    endtask

//************************************************************************************************

task ahb_master_driver::split_resp(ahb_seq_item xtn,input int i);
       vif.mdr_cb.htrans  <= `IDLE;
  for(int k=0;k<(i-1);k++)
          begin
          xtn.haddr_q.delete(0);
          xtn.hwdata.delete(0);
          end   
      
endtask

//**********************************************************************************************

task ahb_master_driver::drive_raddr(ahb_seq_item xtn);
	@(vif.mdr_cb);
	for(int i=0;i<length;i++)
		begin
			 case(vif.hresp)
          `OKAY  : okay_rresp(xtn,i);
          `ERROR : begin
                    err_rresp(xtn,i);
                    break;
                   end
          `RETRY : begin
                    retry_rresp(xtn,i);
                    @(vif.mdr_cb);
                     i = 0;
                    length = xtn.haddr_q.size();
                        case(length)
                        	'd4 : xtn.hburst = `INCR4;
                        	'd8 : xtn.hburst = `INCR8;
                        	'd16: xtn.hburst = `INCR16;
                        	default : xtn.hburst = `INCR;
                        endcase
                        @(vif.mdr_cb);
                        wait(vif.hready && vif.hgrant);
                        address_phase(xtn);
                        //drive_data(xtn);
                        @(vif.mdr_cb);
                    end
          `SPLIT : begin
                    split_rresp(xtn,i);
                    @(vif.mdr_cb);
                    i= -1;
                    length =xtn.haddr_q.size();
                         case(length)
                        'd4 : xtn.hburst = `INCR4;
                        'd8 : xtn.hburst = `INCR8;
                        'd16: xtn.hburst = `INCR16;
                        default : xtn.hburst = `INCR;
                    endcase
 												@(vif.mdr_cb);
                        wait(vif.hready && vif.hgrant);
                        address_phase(xtn);
                        //drive_data(xtn);
                        @(vif.mdr_cb);

                    end
         endcase
    end
       @(vif.mdr_cb);

 			 vif.mdr_cb.hbusreq <= 0;
      endtask

//************************************************************************************************

task ahb_master_driver::okay_rresp(ahb_seq_item xtn,input int i);
      
      	if(xtn.haddr_q.size > (i+1))
      		vif.mdr_cb.haddr <= xtn.haddr_q[i+1];
      	else 
      		begin
        		vif.mdr_cb.haddr <= 0;
        		vif.mdr_cb.hwrite <= 0;
        		vif.mdr_cb.hburst <= xtn.hburst;
      		end
      	if((xtn.hburst == 0) || (i == (length-1)))
       		 vif.mdr_cb.htrans <= `IDLE;
      	else
      		vif.mdr_cb.htrans <= `SEQ;
       		wait(vif.mdr_cb.hready);
       		@(vif.mdr_cb);
endtask

//************************************************************************************************

task ahb_master_driver::err_rresp(ahb_seq_item xtn,input int i);
    vif.mdr_cb.haddr  <=  0;
    vif.mdr_cb.hwrite <=  0;
    vif.mdr_cb.hsize  <=  0;
    vif.mdr_cb.hburst <=  0;
    vif.mdr_cb.htrans <=  `IDLE;
    
    //xtn = null;
    @(vif.mdr_cb);

endtask
//************************************************************************************************

task ahb_master_driver::retry_rresp(ahb_seq_item xtn,input int i);
       vif.mdr_cb.htrans  <= `IDLE;
       for(int k=0;k<(i-1);k++)
          begin
          xtn.haddr_q.delete(0);
          end   
      
    endtask

//************************************************************************************************

task ahb_master_driver::split_rresp(ahb_seq_item xtn,input int i);
       vif.mdr_cb.htrans  <= `IDLE;
  for(int k=0;k<(i-1);k++)
          begin
          xtn.haddr_q.delete(0);
         
          end   
      
endtask

//**********************************************************************************************

