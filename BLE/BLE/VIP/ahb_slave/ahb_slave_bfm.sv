class ahb_bfm extends ovm_component;
  `ovm_component_utils(ahb_bfm)

  virtual ahb_if vif;
  ble_ral_block ble_ral;
  ahb_slave_agent_config s_cfg;


	// Handle - where the env passes a reference
	// ble_ral_block ble_ral;

   logic[31:0] address;
   logic[31:0] w_data,r_data;
   logic       direction;
   logic       addr_valid;
 	 logic[2:0]  hsize;
	extern function new(string name ="ahb_bfm",ovm_component parent);
  extern function void build();
  extern function void connect();
  extern task run();
  extern virtual protected task get_and_drive();
	extern task collect_data();
  extern virtual protected task reset_signals();
    


endclass : ahb_bfm


//***********************************************************************************************

  function ahb_bfm::new(string name ="ahb_bfm",ovm_component parent);
    super.new(name,parent);
  endfunction

//***********************************************************************************************

   function void ahb_bfm::build();
      ovm_object tmp;
      assert(get_config_object("ahb_slave_agent_config",tmp));
      if(!$cast(s_cfg,tmp))
         `ovm_error("ahb_master_monitor","invalid cast....")  
  endfunction


//***********************************************************************************************

  function void ahb_bfm::connect();
    vif = s_cfg.vif;
    ble_ral = s_cfg.ble_ral;
  endfunction


 // UVM run() phase
  task ahb_bfm::run();
    fork
      get_and_drive();
      reset_signals();
    join_any
  endtask : run

   // Reset all signals    ######################################################################
  task ahb_bfm::reset_signals();
    forever begin
      @(negedge vif.hresetn);
      `uvm_info(get_type_name(), "Reset observed", UVM_MEDIUM)
      vif.hready   <= 0;
      vif.hresp		 <= 0;
			vif.hrdata   <= 0;
    end
  endtask : reset_signals

	// Continuosly detects transfers   ###########################################################
  task ahb_bfm::get_and_drive();
    @(posedge vif.hresetn);
    `uvm_info(get_type_name(), "Reset dropped", UVM_MEDIUM)
		 forever begin
     @(posedge vif.hclk); 
			vif.hready <=1;
      vif.hresp  <=0;
			 
				if(vif.htrans[1] == 'b1 )   // Checking for Tranfer type //NON_SEQ 
      	begin
					// Begin transaction recording
      		address = vif.haddr;
      		direction = vif.hwrite;
					hsize   = vif.hsize;
		   		addr_valid = 1;
					vif.hresp <= 0;
					vif.hready <=1;
					@(posedge vif.hclk);
					collect_data();						// Calling collect data task // If Transfer type is sequential 
        end
        else begin
				addr_valid = 0;   	end
		end
  endtask : get_and_drive
	
	// Collect data task     #####################################################################
	task ahb_bfm::collect_data();

	// FOR SINGLE BURST  //

	if (vif.hburst == 3'b000) begin				// check for Burst type // single
			if(direction == 1) begin					// check fo Write/Read
        w_data = vif.hwdata;						
			
  	// Writing a data into Ral model
        case(hsize)
        	3'd0:ble_ral.reg_set(address,{w_data[7:0]});
        	3'd1:ble_ral.reg_set(address,{w_data[15:8],w_data[7:0]});
        	3'd2:ble_ral.reg_set(address,{w_data[31:24],w_data[23:16],w_data[15:8],w_data[7:0]});
        endcase
      end
			else begin
				// reading a data from RAL model
        case(hsize)
        	3'd0:ble_ral.reg_get(address,{r_data[7:0]}); 
        	3'd1:ble_ral.reg_get(address,{r_data[15:8],r_data[7:0]}); 
        	3'd2:ble_ral.reg_get(address,{r_data[31:24],r_data[23:16],r_data[15:8],r_data[7:0]}); 
        endcase
				vif.hrdata <= r_data;
        
			end

	end

	// FOR MULTIPLE TANSACTION 

	else begin
		forever  begin // Control phase pipelined
         		if(direction == 1) begin			// check fo Write/Read
            	w_data = vif.hwdata;			
							
							// Writing a data into Ral model
							case(hsize)
        				3'd0:ble_ral.mem_write(address,{w_data[7:0]});
        				3'd1:ble_ral.mem_write(address,{w_data[15:8],w_data[7:0]});
        				3'd2:ble_ral.mem_write(address,{w_data[31:24],w_data[23:16],w_data[15:8],w_data[7:0]});
        			endcase
							if(vif.htrans == 'b11)		begin    // If Transfer type is sequential 
			 				address = vif.haddr;
      				direction = vif.hwrite;	
 							end
						else 
							break;	
							@(posedge vif.hclk);
						  			
						end
						else begin			
						// reading a data from RAL model
			//			case(hsize)
      //  			3'd0:ble_ral.mem_read(address,0,{r_data[7:0]}); 
      //  			3'd1:ble_ral.mem_read(address,1,{r_data[15:8],r_data[7:0]}); 
      //  			3'd2:ble_ral.mem_read(address,2,{r_data[31:24],r_data[23:16],r_data[15:8],r_data[7:0]}); 
      //  		endcase
						vif.hrdata <= r_data;
						if(vif.htrans == 'b11)		begin    // If Transfer type is sequential
			 			address = vif.haddr;
      			direction = vif.hwrite;		
 						end
						else
						break;
						@(posedge vif.hclk);	  			
					end
				end
		end
		endtask : collect_data
     


