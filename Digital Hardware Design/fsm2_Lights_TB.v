`timescale 100us/10us //set timescale to 100us with 10us precision
module fsm2_Lights_TB;
	//declaration of necessary signals that will drive/be driven the/by dut
	reg Clock_TB;
	reg Reset_TB;
	reg CAR_TB;
	wire GRN_TB;
	wire YLW_TB;
	wire RED_TB;
	// declaration of Design under test and declaration of ports
	fsm2_Lights dut(.Clock(Clock_TB), 
		.Reset(Reset_TB), .CAR(CAR_TB), 
		.YLW(YLW_TB), .RED(RED_TB), .GRN(GRN_TB));
	
	// block for reset
	initial
	   begin
	   
	        Reset_TB = 1'b0;
		CAR_TB = 0;
	   #10  Reset_TB = 1'b1;
	           
	        
	   end
	//initial block for clock
       initial 
	    begin
		Clock_TB = 1'b0;//initialize clock to 0
	end	
	// always block to model clock pulses
	always 
	  begin
  	    #5 Clock_TB = ~Clock_TB;//change clock's value every 500us, make a 1000us period for clock
	  end
	//initial block with different inputs for testing the FSM
	initial
	   begin
	     #3  CAR_TB = 1'b1;//300us
 	     #3  CAR_TB = 1'b0;//600us
	     #19 CAR_TB = 1'b1;//2.5ms
             #75 CAR_TB = 1'b0;//10ms
	     #190000 CAR_TB = 1'b1;//19.01 sec
	     #80 CAR_TB = 1'b0;

	   end 
	
endmodule
