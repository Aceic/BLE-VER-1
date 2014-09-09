class Ble_Scoreboard extends ovm_scoreboard;

	`ovm_component_utils(Ble_Scoreboard);

	   pkt_seq_item pkt_trans;

	// Analysis port required to collect data from the VIP'S	
	tlm_analysis_fifo #(pkt_seq_item) tx_fifo_0;
	tlm_analysis_fifo #(pkt_seq_item) rx_fifo_0;

	tlm_analysis_fifo #(pkt_seq_item) tx_fifo_1;
	tlm_analysis_fifo #(pkt_seq_item) rx_fifo_1;

	pkt_seq_item tx_trans_0, pkt_tx0;
	pkt_seq_item rx_trans_0, pkt_rx0;
  pkt_seq_item tx_trans_1, pkt_tx1;
	pkt_seq_item rx_trans_1, pkt_rx1;

  typedef bit frame_pkt[];
	frame_pkt frame_tx0, frame_rx0, frame_tx1, frame_rx1;

	pkt_seq_item exp_queue_0[$];
	pkt_seq_item rev_queue_0[$];

	pkt_seq_item exp_queue_1[$];
	pkt_seq_item rev_queue_1[$];

	extern function new(string name = "Ble_Scoreboard",ovm_component parent);
	extern task run();
	//extern task collect_wr_data(pkt_seq_item inst);
	//extern task compare_rd_data(pkt_seq_item inst);

	extern task T_DoTxRxPkt();
	extern task T_DoRxTxPkt();
	extern task tx_mod0();
	extern task rx_mod0();
	extern task rx_mod1();
	extern task tx_mod1();
	extern task tx_rx_compare();


endclass

  function Ble_Scoreboard::new(string name = "Ble_Scoreboard", ovm_component parent);
   super.new(name, parent);
	 tx_fifo_0 = new("tx_fifo_0", this);
	 rx_fifo_1 = new("rx_fifo_1", this);
	 tx_fifo_1 = new("tx_fifo_1", this);
	 rx_fifo_0 = new("rx_fifo_0", this);
   endfunction


	task Ble_Scoreboard::run();
     fork

				// Collect the data from the analysis ports
				//
				// Tx PKT Compare Task (T_DoTxRxPkt();)
				
				 T_DoTxRxPkt();
				 T_DoRxTxPkt();			
			
      tx_rx_compare(); 
		 join
	endtask


//-----------------------------------------------------------------------------------
// Tx PKT Compare Task
//----------------------------------------------------------------------------------
	task Ble_Scoreboard:: T_DoTxRxPkt();
    
		forever
		 begin
       fork
           begin
		 	       tx_mod0(); // Collect the data from the tx agent and store or flush the data based on the received event
	         end		
	         begin
	         	 rx_mod1();
		       //tx_rx_compare();  // Task should be written to compare datas betwen to ips
	         end
       join 
			end
	  
	endtask : T_DoTxRxPkt


	task Ble_Scoreboard:: T_DoRxTxPkt();
    
		forever
		 begin
       fork
           begin
		 	       tx_mod1(); // Collect the data from the tx agent and store or flush the data based on the received event
	         end		
	         begin
	         	 rx_mod0();
		       //tx_rx_compare();  // Task should be written to compare datas betwen to ips
	         end
       join 
			end
	  
	endtask : T_DoRxTxPkt

	task Ble_Scoreboard::tx_mod0();

		fork
			begin

			// Fifo/memory get data 
		 		tx_fifo_0.get(pkt_tx0);
     `ovm_info("Ble_Scoreboard"," *Packet sequence item received from tx monitor",OVM_LOW)
				pkt_tx0.print();
			// Hold the data into the memory
			 	exp_queue_0.push_back(pkt_tx0);

			// Wait for the pkt recevied event raised
			       // wait(packet_event_raised);	

			end

			// begin
			// Timer Task to count 625micro seconds
			//	 #625000; 

			// Flush the data from the TX Memory
			 	//exp_queue_0.pop_front(pkt_trans);
		
			// end
		join
		
		// disable fork;
	
	endtask : tx_mod0


task Ble_Scoreboard::tx_mod1();

		fork
			begin

			// Fifo/memory get data 
		 		tx_fifo_1.get(pkt_tx1);
     `ovm_info("Ble_Scoreboard"," *Packet sequence item received from tx monitor",OVM_LOW)
				pkt_tx1.print();
			// Hold the data into the memory
			 	exp_queue_1.push_back(pkt_tx1);

			// Wait for the pkt recevied event raised
			       // wait(packet_event_raised);	

			end

			// begin
			// Timer Task to count 625micro seconds
			//	 #625000; 

			// Flush the data from the TX Memory
			 	//exp_queue_0.pop_front(pkt_trans);
		
			// end
		join
		
		// disable fork;
	
	endtask : tx_mod1

	task Ble_Scoreboard::rx_mod1();

	      fork : RX

		begin

			//collect the packet from the rx monitor
				rx_fifo_1.get(pkt_rx1);
     `ovm_info("Ble_Scoreboard","*Packet sequence item received from rx monitor",OVM_LOW)
				pkt_rx1.print();

			//store it in the rx memory
				rev_queue_1.push_back(pkt_rx1);

			//Trigger the event for packet recived and comparision
			//
			//

		end

		//begin
		//	wait (packet_recieved_event && packet_written_in_monitor_event);
		//	


		//end

		begin

		//	// ADV Timer Logic
		//	// This is only for adv packets,In further
		//	// implementation this is changed based on the type of
		//	// packet and timer value should be updated with the
		//	// current type of packet.
			#10000000; 

		end

	      join_any

	disable RX;

	endtask : rx_mod1


	task Ble_Scoreboard::rx_mod0();

	      fork : RX

		begin

			//collect the packet from the rx monitor
				rx_fifo_0.get(pkt_rx0);
     `ovm_info("Ble_Scoreboard","*Packet sequence item received from rx monitor",OVM_LOW)
				pkt_rx0.print();

			//store it in the rx memory
				rev_queue_0.push_back(pkt_rx0);

			//Trigger the event for packet recived and comparision
			//
			//

		end

		//begin
		//	wait (packet_recieved_event && packet_written_in_monitor_event);
		//	


		//end

		begin

		//	// ADV Timer Logic
		//	// This is only for adv packets,In further
		//	// implementation this is changed based on the type of
		//	// packet and timer value should be updated with the
		//	// current type of packet.
			#10000000; 

		end

	      join_any

	disable RX;

	endtask : rx_mod0

	task Ble_Scoreboard:: tx_rx_compare();

	 int count_0, count_1;
  
	 fork
    forever
	 	begin
        wait(exp_queue_0.size != 0 && rev_queue_1.size != 0)
	 		  begin
          tx_trans_0 = exp_queue_0.pop_front();
	 			  rx_trans_1 = rev_queue_1.pop_front();
          tx_trans_0.pack(frame_tx0);
	 			  rx_trans_1.pack(frame_rx1);
          //$display("Frame_tx %p & \n Frame_rx %p", frame_tx, frame_rx); 
          if(frame_tx0.size == frame_rx1.size)
	 			  begin
            foreach(frame_tx0[i])
	 				  if(frame_tx0[i] != frame_rx1[i])
             `ovm_error("Ble_Scoreboard","Bit by bit comparison Failure")
	 					else
	 					begin
	 					count_0++;
	 					if(count_0 == frame_tx0.size)
	 					`ovm_info("Ble_Scoreboard","PACKET COMPARISON SUCCESS FOR TX0-RX1",OVM_LOW) 
	 					end
	 				end
	 			 else
	 			  begin
            `ovm_error("Ble_Scoreboard","TX packet and RX packet length did not match")
	 				end
          

	 			end
	 		count_0 = 0;	
	 	end
    

		forever
	 	begin
        wait(exp_queue_1.size != 0 && rev_queue_0.size != 0)
	 		  begin
          tx_trans_1 = exp_queue_1.pop_front();
	 			  rx_trans_0 = rev_queue_0.pop_front();
          tx_trans_1.pack(frame_tx1);
	 			  rx_trans_0.pack(frame_rx0);
          //$display("Frame_tx %p & \n Frame_rx %p", frame_tx, frame_rx); 
          if(frame_tx1.size == frame_rx0.size)
	 			  begin
            foreach(frame_tx1[i])
	 				  if(frame_tx1[i] != frame_rx0[i])
             `ovm_error("Ble_Scoreboard","Bit by bit comparison Failure")
	 					else
	 					begin
	 					count_1++;
	 					if(count_1 == frame_tx1.size)
	 					`ovm_info("Ble_Scoreboard","PACKET COMPARISON SUCCESS FOR TX1-RX0",OVM_LOW) 
	 					end
	 				end
	 			 else
	 			  begin
            `ovm_error("Ble_Scoreboard","TX packet and RX packet length did not match")
	 				end
          

	 			end
	 		count_1 = 0;	
	 	end

		 
	 join

  endtask : tx_rx_compare



