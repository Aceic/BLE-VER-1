class ble_vbase_seq extends ovm_sequence #(ovm_sequence_item);
	`ovm_object_utils(ble_vbase_seq)

//	ahb_master_sequencer m_seqrh[];

  ovm_object tmp;
	ble_env_config cfg;
	ble_virtual_sequencer v_sequencer;

	//handles for all the sequences
   adv_random_vseq adv_rand_seq;
   scan_req_vseq scan_seq;
   conn_req_vseq conn_seq;
  //scan_param_set_seq scan_set_seq;
   



function new(string name = "ble_vbase_seq");
	super.new(name);
endfunction	

	task body();
     begin
    	if(!$cast(v_sequencer,m_sequencer))
			`ovm_fatal("VSEQ BODY","Error in $Cast of virtual sequencer")
 
		  if(!(m_sequencer.get_config_object("config",tmp)))
	    	`ovm_fatal("V_SEQUENCNE","cannot get() cfg in virtual sequence. Have you set() it?")
	   if(!$cast(cfg,tmp))
    	`ovm_fatal("V_SEQUENCNE","cannot get() m_cfg from ovm_config_db. Have you set() it?")
	//	  m_seqrh = new[cfg.no_of_ovc];
      
	//	adv_set_seq = new[cfg.no_of_ovc];	

		//	v_sequencer.m_seqrh = new[cfg.no_of_ovc];
			
	//	m_seqrh = new[v_sequencer.m_seqrh.size()];
//		foreach(m_seqrh[i]) 
 //     m_seqrh[i] = v_sequencer.m_seqrh[i];
  end
 	endtask

endclass


//***************************************************************************************************


class adv_scan_vseq extends ble_vbase_seq;
`ovm_object_utils(adv_scan_vseq)

  extern function new(string name = "adv_scan_vseq");
  extern task body();

endclass


  function adv_scan_vseq::new(string name = "adv_scan_vseq");
    super.new(name);
  endfunction

  task adv_scan_vseq::body();
      super.body();
        begin
          adv_rand_seq = adv_random_vseq::type_id::create("adv_rand_seq");
          scan_seq = scan_req_vseq::type_id::create("scan_seq");
          fork
            adv_rand_seq.start(v_sequencer.m_seqrh[0]);
            scan_seq.start(v_sequencer.m_seqrh[1]);
          join
        end
  endtask


//***************************************************************************************************


class adv_conn_vseq extends ble_vbase_seq;
`ovm_object_utils(adv_conn_vseq)
  
  string pdu_type;
  bit [5:0] length;
  
  adv_set_config_seq adv_set_config_seq_h;

  extern function new(string name = "adv_conn_vseq");
  extern task body();

endclass


  function adv_conn_vseq::new(string name = "adv_conn_vseq");
    super.new(name);
  endfunction

  task adv_conn_vseq::body();
      super.body();
         begin
          adv_rand_seq = adv_random_vseq::type_id::create("adv_rand_seq");
          conn_seq = conn_req_vseq::type_id::create("conn_seq");
          fork         
          adv_rand_seq.start(v_sequencer.m_seqrh[0]);
          conn_seq.start(v_sequencer.m_seqrh[1]);
          join 
       end
  endtask


//***************************************************************************************************

//***************************************************************************************************
//                              ADV - SCAN virtual sequence
//***************************************************************************************************

/*

class adv_random_vseq extends ble_vbase_seq;
`ovm_object_utils(adv_random_vseq)

 adv_param_set_seq        adv_param_seq; 
 advertising_add_set_seq  adv_address_set;
 scan_resp_param_set_seq  scan_resp_set;
 interrupt_enable         ie_enable,ie_enable_s ;
 adv_enable               advertising_enable;
// adv_directed_pdu_seq     adv_param_seq; 
 scan_enable_param_set_seq scan_req_set;
 scan_enable             scanning_enable;
 adv_received_interrupt_seq adv_rcvd_int;

  ral_reset_seq reset_seq;

  extern function new(string name = "adv_random_vseq");
  extern task body();

endclass



  function adv_random_vseq::new(string name = "adv_random_vseq");
    super.new(name);
  endfunction

  task adv_random_vseq::body();
      super.body();
   //   repeat(5)
       begin
       fork
        begin
          adv_param_seq = adv_param_set_seq::type_id::create("adv_param_seq");
          adv_param_seq.status_randomize = 1;
          //adv_param_seq = adv_directed_pdu_seq::type_id::create("adv_param_seq");
          //adv_param_seq.local_adv_type =ADV_DIRECT_IND_HIGH_DUTY_CYCLE;
          adv_address_set = advertising_add_set_seq::type_id::create("adv_address_set");
          adv_address_set.status_randomize = 1;
          scan_resp_set = scan_resp_param_set_seq::type_id::create("scan_resp_set");
          scan_resp_set.status_randomize = 1;
          ie_enable= interrupt_enable::type_id::create("ie_enable");
          advertising_enable = adv_enable::type_id::create("advertising_enable");
            if((v_sequencer.m_seqrh[0])!=null) begin
            adv_param_seq.start(v_sequencer.m_seqrh[0]);               // setting adv parameters
            adv_address_set.start(v_sequencer.m_seqrh[0]);             // setting adv address values
            scan_resp_set.start(v_sequencer.m_seqrh[0]);               // setting up scan resp data (if required)
            ie_enable.start(v_sequencer.m_seqrh[0]);                   // enabling interrrupt
            advertising_enable.start(v_sequencer.m_seqrh[0]);          // configuring command reg to start advertising
            end
            else
            begin
             `ovm_fatal(get_type_name(),"Local Sequencer[0] of Vseqr is not found ")            
            end
       end
       begin
          scan_req_set = scan_enable_param_set_seq::type_id::create("scan_req_set");
          scan_req_set.status_randomize = 1;
          scanning_enable = scan_enable::type_id::create("scanning_enable");
          adv_rcvd_int = adv_received_interrupt_seq::type_id::create("adv_rcvd_int");
          ie_enable_s= interrupt_enable::type_id::create("ie_enable_s");
            if((v_sequencer.m_seqrh[1])!=null) begin
            scan_req_set.start(v_sequencer.m_seqrh[1]);               // setting scan parameters to put rx in scan state
            ie_enable_s.start(v_sequencer.m_seqrh[1]);
            scanning_enable.start(v_sequencer.m_seqrh[1]);            // configuring command reg to start scanning
            //   adv_rcvd_int.start(v_sequencer.m_seqrh[1]);               // waiting for pkt recvd interrrupt to be high and clear
            end 
            else begin`ovm_fatal(get_type_name(),"Local Sequencer[1] of Vseqr is not found ") end

       end
      join
        reset_seq = ral_reset_seq::type_id::create("reset_seq");
        reset_seq.start(v_sequencer.m_seqrh[0]);
        reset_seq.start(v_sequencer.m_seqrh[1]);
      end
  endtask


//***************************************************************************************************
//                              ADV - SCAN virtual sequence
//***************************************************************************************************

class adv_conn_vseq extends ble_vbase_seq;
  `ovm_object_utils(adv_conn_vseq)

  function new (string name = "adv_conn_vseq");
    super.new(name);
  endfunction

  
 adv_param_set_seq        adv_param_seq; 
 advertising_add_set_seq  adv_address_set;
 scan_resp_param_set_seq  scan_resp_set;
 interrupt_enable         ie_enable,ie_enable_s ;
 adv_enable               advertising_enable;
// adv_directed_pdu_seq     adv_param_seq; 
 scan_enable_param_set_seq scan_req_set;
 scan_enable             scanning_enable;
 adv_received_interrupt_seq adv_rcvd_int;

 create_conn_param_set_seq  conn_param_seq;
 conn_timing_param_set_seq  conn_timing_seq;
 data_channel_set_seq       data_channel_seq;
 connection_add_set_seq     conn_addr_seq;
 init_enable                init_en;                           

 extern task body();

endclass

 task adv_conn_vseq::body();
      super.body();
       fork
        begin
          adv_param_seq = adv_param_set_seq::type_id::create("adv_param_seq");
          adv_param_seq.status_randomize = 1;
          adv_address_set = advertising_add_set_seq::type_id::create("adv_address_set");
          adv_address_set.status_randomize = 1;
          scan_resp_set = scan_resp_param_set_seq::type_id::create("scan_resp_set");
          scan_resp_set.status_randomize = 1;
          ie_enable= interrupt_enable::type_id::create("ie_enable");
          advertising_enable = adv_enable::type_id::create("advertising_enable");
          adv_param_seq.start(v_sequencer.m_seqrh[0]);               // setting adv parameters
          adv_address_set.start(v_sequencer.m_seqrh[0]);             // setting adv address values
          scan_resp_set.start(v_sequencer.m_seqrh[0]);               // setting up scan resp data (if required)
          ie_enable.start(v_sequencer.m_seqrh[0]);                   // enabling interrrupt
          advertising_enable.start(v_sequencer.m_seqrh[0]);          // configuring command reg to start advertising
       end
       begin
         conn_param_seq   = create_conn_param_set_seq::type_id::create("conn_param_seq");
         conn_param_seq.status_randomize = 1;
         conn_timing_seq  = conn_timing_param_set_seq::type_id::create("conn_timing_seq");
         conn_timing_seq.status_randomize = 1;
         data_channel_seq = data_channel_set_seq::type_id::create("data_channel_seq");     
         data_channel_seq.status_randomize = 1;
         conn_addr_seq    = connection_add_set_seq::type_id::create("conn_addr_seq");   
         conn_addr_seq.status_randomize = 1;
         init_en         = init_enable::type_id::create("init_en");                           
         conn_param_seq.start(v_sequencer.m_seqrh[1]);    
         conn_timing_seq.start(v_sequencer.m_seqrh[1]);    
         data_channel_seq.start(v_sequencer.m_seqrh[1]);    
         conn_addr_seq.start(v_sequencer.m_seqrh[1]);    
         init_en.start(v_sequencer.m_seqrh[1]);    
 
       end
      join
  endtask
*/
