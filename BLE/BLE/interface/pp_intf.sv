interface pp_intf(input bit clock, rst);

 logic [5:0] channel_index ;
 
 logic PPI;
 logic BLE_TX;
 logic BLE_RX;
 logic BLE_TIFS;

 logic TX_FLAG;                         // Flag from controller to decide on transmission or receiption

 parameter TX_RX_COUNT = 625;

 task tifs();
  repeat(150)    
  @(posedge clock);
 endtask


 task tx_gen();
  forever
     begin
      BLE_TX = 1;      
      repeat(TX_RX_COUNT)
      @(posedge clock);
      BLE_TX = 0;
      BLE_TIFS = 1;
      tifs();
      BLE_TIFS = 0;
      BLE_RX = 1;
      repeat(TX_RX_COUNT)
      @(posedge clock);
      BLE_RX = 0;
      BLE_TIFS = 1;
      tifs();
      BLE_TIFS = 0;
    end
 endtask

 task rx_gen();
  forever
     begin
      BLE_RX = 1;      
      repeat(TX_RX_COUNT)
      @(posedge clock);
      BLE_RX = 0;
      BLE_TIFS = 1;
      tifs();
      BLE_TIFS = 0;
      BLE_TX = 1;
      repeat(TX_RX_COUNT)
      @(posedge clock);
      BLE_TX = 0;
      BLE_TIFS = 1;
      tifs();
      BLE_TIFS = 0;
    end
 endtask

 initial
  begin
  forever
  begin
   @(posedge clock);
    if(TX_FLAG == 1)
     begin
     fork
     tx_gen();
     join_none
     break;
     end
    else if(TX_FLAG == 0)
     begin
     fork
     rx_gen();
     join_none
     break;
     end
  end
  end
endinterface
