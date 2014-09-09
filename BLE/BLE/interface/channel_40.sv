

 module channel_module(pp_intf pp_if_1, pp_if_2, channel_if ch_if);

   
  always@(*)
   begin
      
     if(pp_if_1.BLE_TX == 1)
     begin

     case (pp_if_1.channel_index)
      32'd0  : ch_if.channel[0]  = pp_if_1.PPI; 
      32'd1  : ch_if.channel[1]  = pp_if_1.PPI;
      32'd2  : ch_if.channel[2]  = pp_if_1.PPI;
      32'd3  : ch_if.channel[3]  = pp_if_1.PPI; 
      32'd4  : ch_if.channel[4]  = pp_if_1.PPI;
      32'd5  : ch_if.channel[5]  = pp_if_1.PPI;
      32'd6  : ch_if.channel[6]  = pp_if_1.PPI;
      32'd7  : ch_if.channel[7]  = pp_if_1.PPI;
      32'd8  : ch_if.channel[8]  = pp_if_1.PPI; 
      32'd9  : ch_if.channel[9]  = pp_if_1.PPI;
      32'd10 : ch_if.channel[10] = pp_if_1.PPI;
      32'd11 : ch_if.channel[11] = pp_if_1.PPI;
      32'd12 : ch_if.channel[12] = pp_if_1.PPI;
      32'd13 : ch_if.channel[13] = pp_if_1.PPI;
      32'd14 : ch_if.channel[14] = pp_if_1.PPI;
      32'd15 : ch_if.channel[15] = pp_if_1.PPI;
      32'd16 : ch_if.channel[16] = pp_if_1.PPI;
      32'd17 : ch_if.channel[17] = pp_if_1.PPI;
      32'd18 : ch_if.channel[18] = pp_if_1.PPI;
      32'd19 : ch_if.channel[19] = pp_if_1.PPI; 
      32'd20 : ch_if.channel[20] = pp_if_1.PPI;
      32'd21 : ch_if.channel[21] = pp_if_1.PPI;
      32'd22 : ch_if.channel[22] = pp_if_1.PPI;
      32'd23 : ch_if.channel[23] = pp_if_1.PPI;
      32'd24 : ch_if.channel[24] = pp_if_1.PPI; 
      32'd25 : ch_if.channel[25] = pp_if_1.PPI;
      32'd26 : ch_if.channel[26] = pp_if_1.PPI;
      32'd27 : ch_if.channel[27] = pp_if_1.PPI;
      32'd28 : ch_if.channel[28] = pp_if_1.PPI;
      32'd29 : ch_if.channel[29] = pp_if_1.PPI;
      32'd30 : ch_if.channel[30] = pp_if_1.PPI;
      32'd31 : ch_if.channel[31] = pp_if_1.PPI;
      32'd32 : ch_if.channel[32] = pp_if_1.PPI; 
      32'd33 : ch_if.channel[33] = pp_if_1.PPI;
      32'd34 : ch_if.channel[34] = pp_if_1.PPI;
      32'd35 : ch_if.channel[35] = pp_if_1.PPI;
      32'd36 : ch_if.channel[36] = pp_if_1.PPI;
      32'd37 : ch_if.channel[37] = pp_if_1.PPI;        //Adv Channel index
      32'd38 : ch_if.channel[38] = pp_if_1.PPI;        //Adv Channel index
      32'd39 : ch_if.channel[39] = pp_if_1.PPI;        //Adv Channel index

     endcase
     
    end
   else if(pp_if_1.BLE_RX == 1)
    begin
     case(pp_if_1.channel_index)
      32'd0  : pp_if_1.PPI = ch_if.channel[0] ; 
      32'd1  : pp_if_1.PPI = ch_if.channel[1] ;
      32'd2  : pp_if_1.PPI = ch_if.channel[2] ;
      32'd3  : pp_if_1.PPI = ch_if.channel[3] ; 
      32'd4  : pp_if_1.PPI = ch_if.channel[4] ;
      32'd5  : pp_if_1.PPI = ch_if.channel[5] ;
      32'd6  : pp_if_1.PPI = ch_if.channel[6] ;
      32'd7  : pp_if_1.PPI = ch_if.channel[7] ;
      32'd8  : pp_if_1.PPI = ch_if.channel[8] ; 
      32'd9  : pp_if_1.PPI = ch_if.channel[9] ;
      32'd10 : pp_if_1.PPI = ch_if.channel[10];
      32'd11 : pp_if_1.PPI = ch_if.channel[11];
      32'd12 : pp_if_1.PPI = ch_if.channel[12];
      32'd13 : pp_if_1.PPI = ch_if.channel[13];
      32'd14 : pp_if_1.PPI = ch_if.channel[14];
      32'd15 : pp_if_1.PPI = ch_if.channel[15];
      32'd16 : pp_if_1.PPI = ch_if.channel[16];
      32'd17 : pp_if_1.PPI = ch_if.channel[17];
      32'd18 : pp_if_1.PPI = ch_if.channel[18];
      32'd19 : pp_if_1.PPI = ch_if.channel[19]; 
      32'd20 : pp_if_1.PPI = ch_if.channel[20];
      32'd21 : pp_if_1.PPI = ch_if.channel[21];
      32'd22 : pp_if_1.PPI = ch_if.channel[22];
      32'd23 : pp_if_1.PPI = ch_if.channel[23];
      32'd24 : pp_if_1.PPI = ch_if.channel[24]; 
      32'd25 : pp_if_1.PPI = ch_if.channel[25];
      32'd26 : pp_if_1.PPI = ch_if.channel[26];
      32'd27 : pp_if_1.PPI = ch_if.channel[27];
      32'd28 : pp_if_1.PPI = ch_if.channel[28];
      32'd29 : pp_if_1.PPI = ch_if.channel[29];
      32'd30 : pp_if_1.PPI = ch_if.channel[30];
      32'd31 : pp_if_1.PPI = ch_if.channel[31];
      32'd32 : pp_if_1.PPI = ch_if.channel[32]; 
      32'd33 : pp_if_1.PPI = ch_if.channel[33];
      32'd34 : pp_if_1.PPI = ch_if.channel[34];
      32'd35 : pp_if_1.PPI = ch_if.channel[35];
      32'd36 : pp_if_1.PPI = ch_if.channel[36];
      32'd37 : pp_if_1.PPI = ch_if.channel[37];        //Adv Channel index
      32'd38 : pp_if_1.PPI = ch_if.channel[38];        //Adv Channel index
      32'd39 : pp_if_1.PPI = ch_if.channel[39];        //Adv Channel index
     endcase
    end


   end


   always @(*)
    begin
      if(pp_if_2.BLE_TX == 1)
    begin

     case (pp_if_2.channel_index)
      32'd0  : ch_if.channel[0]  = pp_if_2.PPI; 
      32'd1  : ch_if.channel[1]  = pp_if_2.PPI;
      32'd2  : ch_if.channel[2]  = pp_if_2.PPI;
      32'd3  : ch_if.channel[3]  = pp_if_2.PPI; 
      32'd4  : ch_if.channel[4]  = pp_if_2.PPI;
      32'd5  : ch_if.channel[5]  = pp_if_2.PPI;
      32'd6  : ch_if.channel[6]  = pp_if_2.PPI;
      32'd7  : ch_if.channel[7]  = pp_if_2.PPI;
      32'd8  : ch_if.channel[8]  = pp_if_2.PPI; 
      32'd9  : ch_if.channel[9]  = pp_if_2.PPI;
      32'd10 : ch_if.channel[10] = pp_if_2.PPI;
      32'd11 : ch_if.channel[11] = pp_if_2.PPI;
      32'd12 : ch_if.channel[12] = pp_if_2.PPI;
      32'd13 : ch_if.channel[13] = pp_if_2.PPI;
      32'd14 : ch_if.channel[14] = pp_if_2.PPI;
      32'd15 : ch_if.channel[15] = pp_if_2.PPI;
      32'd16 : ch_if.channel[16] = pp_if_2.PPI;
      32'd17 : ch_if.channel[17] = pp_if_2.PPI;
      32'd18 : ch_if.channel[18] = pp_if_2.PPI;
      32'd19 : ch_if.channel[19] = pp_if_2.PPI; 
      32'd20 : ch_if.channel[20] = pp_if_2.PPI;
      32'd21 : ch_if.channel[21] = pp_if_2.PPI;
      32'd22 : ch_if.channel[22] = pp_if_2.PPI;
      32'd23 : ch_if.channel[23] = pp_if_2.PPI;
      32'd24 : ch_if.channel[24] = pp_if_2.PPI; 
      32'd25 : ch_if.channel[25] = pp_if_2.PPI;
      32'd26 : ch_if.channel[26] = pp_if_2.PPI;
      32'd27 : ch_if.channel[27] = pp_if_2.PPI;
      32'd28 : ch_if.channel[28] = pp_if_2.PPI;
      32'd29 : ch_if.channel[29] = pp_if_2.PPI;
      32'd30 : ch_if.channel[30] = pp_if_2.PPI;
      32'd31 : ch_if.channel[31] = pp_if_2.PPI;
      32'd32 : ch_if.channel[32] = pp_if_2.PPI; 
      32'd33 : ch_if.channel[33] = pp_if_2.PPI;
      32'd34 : ch_if.channel[34] = pp_if_2.PPI;
      32'd35 : ch_if.channel[35] = pp_if_2.PPI;
      32'd36 : ch_if.channel[36] = pp_if_2.PPI;
      32'd37 : ch_if.channel[37] = pp_if_2.PPI;        //Adv Channel index
      32'd38 : ch_if.channel[38] = pp_if_2.PPI;        //Adv Channel index
      32'd39 : ch_if.channel[39] = pp_if_2.PPI;        //Adv Channel index

     endcase
     
    end
   else if(pp_if_2.BLE_RX == 1)
    begin
     case(pp_if_2.channel_index)
      32'd0  : pp_if_2.PPI = ch_if.channel[0] ; 
      32'd1  : pp_if_2.PPI = ch_if.channel[1] ;
      32'd2  : pp_if_2.PPI = ch_if.channel[2] ;
      32'd3  : pp_if_2.PPI = ch_if.channel[3] ; 
      32'd4  : pp_if_2.PPI = ch_if.channel[4] ;
      32'd5  : pp_if_2.PPI = ch_if.channel[5] ;
      32'd6  : pp_if_2.PPI = ch_if.channel[6] ;
      32'd7  : pp_if_2.PPI = ch_if.channel[7] ;
      32'd8  : pp_if_2.PPI = ch_if.channel[8] ; 
      32'd9  : pp_if_2.PPI = ch_if.channel[9] ;
      32'd10 : pp_if_2.PPI = ch_if.channel[10];
      32'd11 : pp_if_2.PPI = ch_if.channel[11];
      32'd12 : pp_if_2.PPI = ch_if.channel[12];
      32'd13 : pp_if_2.PPI = ch_if.channel[13];
      32'd14 : pp_if_2.PPI = ch_if.channel[14];
      32'd15 : pp_if_2.PPI = ch_if.channel[15];
      32'd16 : pp_if_2.PPI = ch_if.channel[16];
      32'd17 : pp_if_2.PPI = ch_if.channel[17];
      32'd18 : pp_if_2.PPI = ch_if.channel[18];
      32'd19 : pp_if_2.PPI = ch_if.channel[19]; 
      32'd20 : pp_if_2.PPI = ch_if.channel[20];
      32'd21 : pp_if_2.PPI = ch_if.channel[21];
      32'd22 : pp_if_2.PPI = ch_if.channel[22];
      32'd23 : pp_if_2.PPI = ch_if.channel[23];
      32'd24 : pp_if_2.PPI = ch_if.channel[24]; 
      32'd25 : pp_if_2.PPI = ch_if.channel[25];
      32'd26 : pp_if_2.PPI = ch_if.channel[26];
      32'd27 : pp_if_2.PPI = ch_if.channel[27];
      32'd28 : pp_if_2.PPI = ch_if.channel[28];
      32'd29 : pp_if_2.PPI = ch_if.channel[29];
      32'd30 : pp_if_2.PPI = ch_if.channel[30];
      32'd31 : pp_if_2.PPI = ch_if.channel[31];
      32'd32 : pp_if_2.PPI = ch_if.channel[32]; 
      32'd33 : pp_if_2.PPI = ch_if.channel[33];
      32'd34 : pp_if_2.PPI = ch_if.channel[34];
      32'd35 : pp_if_2.PPI = ch_if.channel[35];
      32'd36 : pp_if_2.PPI = ch_if.channel[36];
      32'd37 : pp_if_2.PPI = ch_if.channel[37];        //Adv Channel index
      32'd38 : pp_if_2.PPI = ch_if.channel[38];        //Adv Channel index
      32'd39 : pp_if_2.PPI = ch_if.channel[39];        //Adv Channel index
     endcase
    end

    end

 endmodule

