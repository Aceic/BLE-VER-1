adv_config_vseq adv_config_vseq_h;

task adv_configure(input string pdu_type, input bit[5:0] length );
  adv_config_vseq_h = adv_config_vseq::type_id::create("adv_scan_vseq_h");
  adv_config_vseq_h.pdu_type = pdu_type;
  adv_config_vseq_h.length = length;
  adv_config_vseq_h.start(env.v_sequencer);
endtask
