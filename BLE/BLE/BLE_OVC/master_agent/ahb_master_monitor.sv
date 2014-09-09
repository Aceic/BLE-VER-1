class ahb_master_monitor extends ovm_monitor;
  `ovm_component_utils(ahb_master_monitor)

  virtual ahb_if vif;      //interface
  ahb_agent_config m_cfg;  //config handle

  ahb_seq_item xtn;
  ovm_analysis_port #(ahb_seq_item) monitor_port;

  extern function new(string name ="ahb_master_monitor",ovm_component parent);
  extern function void build();

  extern function void connect();
  extern task run();
  
  extern task collect();
  extern task collect_data(ahb_seq_item xtn);
  extern task collect_rdata(ahb_seq_item xtn);

endclass : ahb_master_monitor

//*************************************************************************

  function ahb_master_monitor::new(string name ="ahb_master_monitor",ovm_component parent);
    super.new(name,parent);
    monitor_port = new("monitor_port",this);
  endfunction

//*************************************************************************

  function void ahb_master_monitor::build();
      ovm_object tmp;
     if(!(get_config_object("ahb_agent_config",tmp)))
      `ovm_fatal("ahb_master_monitor","can not get config object....") 
     if(!$cast(m_cfg,tmp))
      `ovm_fatal("ahb_master_monitor","invalid cast....") 
  endfunction

//*************************************************************************

  function void ahb_master_monitor::connect();
    vif = m_cfg.vif;
  endfunction

//*************************************************************************

  task ahb_master_monitor::run();
    forever
      begin
        collect();
      end
  endtask

//*************************************************************************


 
	task ahb_master_monitor::collect();
					xtn = ahb_seq_item::type_id::create("xtn");
	        begin
						//wait(vif.mmon_cb.hsel && vif.mmon_cb.hready);
						wait(vif.mmon_cb.htrans == `NONSEQ)
						xtn.haddr_q.push_back(vif.mmon_cb.haddr);
            xtn.haddr = vif.mmon_cb.haddr;
						xtn.hburst = vif.mmon_cb.hburst;
						xtn.hsize = vif.mmon_cb.hsize;
						xtn.hwrite = vif.mmon_cb.hwrite;
					
						@(vif.mmon_cb);
						if(xtn.hwrite == 1)
						collect_data(xtn);
						else
						collect_rdata(xtn);
          end    		
		endtask	

//****************** collecting wr_addr control signals ****************************	
		
task ahb_master_monitor::collect_data(ahb_seq_item xtn);
		begin
				if(vif.mmon_cb.hburst == `SINGLE)
					begin
						xtn.hwdata.push_back(vif.mmon_cb.hwdata);
						xtn.hresp.push_back(vif.mmon_cb.hresp); 
					end
				else
					begin
					//	if(!vif.mmon_cb.hgrant)
					//		xtn = null;
					//	else
						begin
							forever
							begin
								wait(vif.mmon_cb.hgrant && vif.mmon_cb.hready);
								xtn.hwdata.push_back(vif.mmon_cb.hwdata);
								xtn.hresp.push_back(vif.mmon_cb.hresp); 
								xtn.hwrite = 1;
								if(vif.mmon_cb.htrans == `SEQ)
								begin
									xtn.haddr_q.push_back(vif.mmon_cb.haddr);
								end
								else
								break;
							@(vif.mmon_cb);
							end
						end
					end
		//		if(xtn != null)	begin
				ovm_report_info("AHB_MONITOR",$sformatf("printing from master monitor........\n %s",xtn.sprint),OVM_HIGH);
				monitor_port.write(xtn);
		//		end
	end	
endtask

//****************** collecting rdata signals ****************************	
		
task ahb_master_monitor::collect_rdata(ahb_seq_item xtn);
		begin
				if(vif.mmon_cb.hburst == `SINGLE)
					begin
						xtn.hrdata.push_back(vif.mmon_cb.hrdata);
						xtn.hresp.push_back(vif.mmon_cb.hresp); 
					end
				else
					begin
					//	if(!vif.mmon_cb.hgrant)
					//		xtn = null;
					//	else
						begin
							forever
							begin
								wait(vif.mmon_cb.hgrant && vif.mmon_cb.hready);
								xtn.hrdata.push_back(vif.mmon_cb.hrdata);
								xtn.hwrite = 0;
								xtn.hresp.push_back(vif.mmon_cb.hresp); 
		
							if(vif.mmon_cb.htrans == `SEQ)
								begin
									xtn.haddr_q.push_back(vif.mmon_cb.haddr);
								end
								else
								break;
							@(vif.mmon_cb);
							end
						end
					end
				if(xtn != null)
				begin
				ovm_report_info("AHB_MONITOR",$sformatf("printing from master monitor........\n %s",xtn.sprint()),OVM_HIGH);
				monitor_port.write(xtn);
				
						
				end
	end	
endtask
			 


