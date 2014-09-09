class ovc_vbase_seq extends ovm_sequence #(ovm_sequence_item);
	`ovm_object_utils(ovc_vbase_seq)

//	ahb_master_sequencer m_seqrh[];

  ovm_object tmp;
	ovm_sequencer_base sequencer;


	//handles for all the sequences
   adv_param_set_seq adv_set_seq[];
  //scan_param_set_seq scan_set_seq;
   



function new(string name = "ovc_vbase_seq");
	super.new(name);
endfunction	

	task body();
     begin
    	if(!$cast(sequencer,m_sequencer))
			`ovm_fatal("VSEQ BODY","Error in $Cast of virtual sequencer")
 
		
		//	sequencer.m_seqrh = new[cfg.no_of_ovc];
			
	//	m_seqrh = new[sequencer.m_seqrh.size()];
//		foreach(m_seqrh[i]) 
 //     m_seqrh[i] = sequencer.m_seqrh[i];
  end
 	endtask

endclass


//***************************************************************************************************

/*
class adv_scan_vseq extends ovc_vbase_seq;
`ovm_object_utils(adv_scan_vseq)

  extern function new(string name = "adv_scan_vseq");
  extern task body();

endclass


  function adv_scan_vseq::new(string name = "adv_scan_vseq");
    super.new(name);
  endfunction

  task adv_scan_vseq::body();
      super.body();
          foreach(sequencer.m_seqrh[i])
        begin
          adv_set_seq[i] = adv_param_set_seq::type_id::create($sformatf("adv_set_seq[%0d]",i));
          adv_set_seq[i].start(sequencer);
        end
  endtask


//***************************************************************************************************


class adv_config_vseq extends ovc_vbase_seq;
`ovm_object_utils(adv_config_vseq)
  
  string pdu_type;
  bit [5:0] length;
  
  adv_set_config_seq adv_set_config_seq_h;

  extern function new(string name = "adv_config_vseq");
  extern task body();

endclass


  function adv_config_vseq::new(string name = "adv_config_vseq");
    super.new(name);
  endfunction

  task adv_config_vseq::body();
      super.body();
          foreach(sequencer.m_seqrh[i])
        begin
          adv_set_config_seq_h = adv_set_config_seq::type_id::create("adv_set_seq");
          adv_set_config_seq_h.local_pdu_type = pdu_type;
          adv_set_config_seq_h.local_length = length;
          adv_set_config_seq_h.start(sequencer.m_seqrh[i]);
        end
  endtask
*/

//***************************************************************************************************


//***************************************************************************************************
//                              ADV random virtual sequence
//***************************************************************************************************

class adv_random_vseq extends ovc_vbase_seq;
  `ovm_object_utils(adv_random_vseq)

  function new (string name = "adv_random_vseq");
    super.new(name);
  endfunction

  
 adv_param_set_seq        adv_param_seq; 
 advertising_add_set_seq  adv_address_set;
 scan_resp_param_set_seq  scan_resp_set;
 interrupt_enable         ie_enable ;
 adv_enable               advertising_enable;
// adv_directed_pdu_seq     adv_param_seq; 
                       

 extern task body();

endclass

 task adv_random_vseq::body();
      super.body();
        begin
          adv_param_seq = adv_param_set_seq::type_id::create("adv_param_seq");
          adv_param_seq.status_randomize = 1;
          adv_address_set = advertising_add_set_seq::type_id::create("adv_address_set");
          adv_address_set.status_randomize = 1;
          scan_resp_set = scan_resp_param_set_seq::type_id::create("scan_resp_set");
          scan_resp_set.status_randomize = 1;
          ie_enable= interrupt_enable::type_id::create("ie_enable");
          advertising_enable = adv_enable::type_id::create("advertising_enable");
          adv_param_seq.start(sequencer);               // setting adv parameters
          adv_address_set.start(sequencer);             // setting adv address values
          scan_resp_set.start(sequencer);               // setting up scan resp data (if required)
          ie_enable.start(sequencer);                   // enabling interrrupt
          advertising_enable.start(sequencer);          // configuring command reg to start advertising
       end
    endtask


//***************************************************************************************************
//                               SCAN virtual sequence
//***************************************************************************************************



class scan_req_vseq extends ovc_vbase_seq;
`ovm_object_utils(scan_req_vseq)

 interrupt_enable         ie_enable_s ;
 scan_enable_param_set_seq scan_req_set;
 scan_enable             scanning_enable;
 adv_received_interrupt_seq adv_rcvd_int;

  ral_reset_seq reset_seq;

  extern function new(string name = "scan_req_vseq");
  extern task body();

endclass



  function scan_req_vseq::new(string name = "scan_req_vseq");
    super.new(name);
  endfunction

  task scan_req_vseq::body();
      super.body();
   //   repeat(5)
       begin
          scan_req_set = scan_enable_param_set_seq::type_id::create("scan_req_set");
          scan_req_set.status_randomize = 1;
          scanning_enable = scan_enable::type_id::create("scanning_enable");
          adv_rcvd_int = adv_received_interrupt_seq::type_id::create("adv_rcvd_int");
          ie_enable_s= interrupt_enable::type_id::create("ie_enable_s");
          
        if((sequencer)!=null)
          begin
            scan_req_set.start(sequencer);               // setting scan parameters to put rx in scan state
            ie_enable_s.start(sequencer);
            scanning_enable.start(sequencer);            // configuring command reg to start scanning
            //   adv_rcvd_int.start(sequencer);               // waiting for pkt recvd interrrupt to be high and clear
          end 
        else 
          begin
            `ovm_fatal(get_type_name(),"Local Sequencer[1] of Vseqr is not found ")
          end

       end
  endtask


//***************************************************************************************************
//                              conn virtual sequence
//***************************************************************************************************

class conn_req_vseq extends ovc_vbase_seq;
  `ovm_object_utils(conn_req_vseq)

  function new (string name = "conn_req_vseq");
    super.new(name);
  endfunction

  

 create_conn_param_set_seq  conn_param_seq;
 conn_timing_param_set_seq  conn_timing_seq;
 data_channel_set_seq       data_channel_seq;
 connection_add_set_seq     conn_addr_seq;
 init_enable                init_en;                           

 extern task body();

endclass

 task conn_req_vseq::body();
      super.body();
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
         conn_param_seq.start(sequencer);    
         conn_timing_seq.start(sequencer);    
         data_channel_seq.start(sequencer);    
         conn_addr_seq.start(sequencer);    
         init_en.start(sequencer);    
 
       end
  endtask

