

module clk_agent(output bit clock, ppi_clock);
 
 bit [4:0] counter;

 always
  #10 clock = ~clock;

 always @(posedge clock)
  begin
    counter <= counter + 1;
    if(counter == 5'd24)
     begin
      ppi_clock <= ~ppi_clock;
      counter <= 5'd0;
     end
  end
 

endmodule
