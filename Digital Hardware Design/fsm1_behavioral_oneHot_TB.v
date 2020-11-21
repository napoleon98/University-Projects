`timescale 1ns/1ns // set timescale to 1 ns with 1ns precision
module fsm1_behavioral_oneHot_TB;
	//declaration of necessary signals that will drive/be driven the/by dut
	reg clk;
	reg reset;
	reg in;
	wire out;
	fsm1_behavioral_oneHot dut(.Clock(clk), .Reset(reset), .Din(in), .Dout(out));// declaration of Design under test and declaration of ports
	// block for reset
	initial 
	   begin
   	         reset = 1'b0;//set reset to 0(active low)
	    #15  reset = 1'b1;//set reset to 1 after 15ns-deactivate it
	end
	//initial block for clock
	initial 
	    begin
		clk = 1'b0;//initialize clock to 0
	end
	// always block to model clock pulses
	always 
	    begin
   		#10 clk = ~clk;//change clock's value every 10ns,make a 20ns period for clock
	end
	//initial block with different inputs for testing the FSM
	initial 
	   begin
	           in = 1'b1;
	      #15  in = 0'b0;
	      #15  in = 1'b1;
	      #20  in = 1'b0;  
	      #30  in = 1'b1;  
	      #10  in = 1'b1;
	      #20  in = 1'b1;
              #10  in = 1'b0;
	   end
endmodule
   