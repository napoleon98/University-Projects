module fsm2_Lights(output reg GRN, YLW, RED, input wire Clock, Reset, CAR);
	// declaration of necessary local parameters
	localparam [1:0] 
		S0 = 2'b00,
		S1 = 2'b01,
		S2 = 2'b10;
	reg [1:0] currentState, nextState; // declaration of two signals for current and next state
	reg [31:0] Counter, Counter_2; //declaration of two 32bit counters
	reg TIMEOUT, TIMEOUT_2; // declare the TIMEOUT and TIMEOUT_2 signals
	//block for storing the machine's state, triggered by positice edge of clock or negative edge of Reset
	always @(posedge Clock or negedge Reset)
	     begin: STATE_MEMORY
		if(!Reset)
		  begin	
		     currentState <= S0; // if reset is active, store S0 in current state..
		    // initialization of counters and timeout dsignals
		     Counter <= 15000; 
		     Counter_2 <= 3000;
		     TIMEOUT <= 0;
		     TIMEOUT_2 <= 0;
		  end
		else
		   currentState <= nextState; // ..else store nextstate in current state
	     end
	//block for next state, triggered by currentState or CAR or positive edge of two timeout signals
	always @(currentState or CAR or posedge TIMEOUT or posedge TIMEOUT_2)
             begin: NEXT_STATE_LOGIC
	//check which is the current state	
		case(currentState)
		   S0: if(CAR) nextState = S1;// in case of S0 check the CAR signal, if it is 1,set S1 in next state..
		       else nextState = S0;// ..else set S0 in next state
		   S1:if(TIMEOUT_2)// in case of S1 check the TIMEOUT_2 signal, if it is 1,set S2 in next state
	                 begin  
		           nextState = S2;
		         end
		   S2: if(TIMEOUT)// in case of S2 check the TIMEOUT signal, if it is 1 set S0 in next state
			  begin
		            nextState = S0;
		            				
			   end
		       else nextState = S2; 
		   default: nextState = S0; // in any other case, set S0 in next state
		endcase
	end
	// block for Down Counter,triggered by positive edge of clock.
	always @(posedge Clock)
	     begin: DOWN_COUNTER
		if(currentState == S2) // check if current state is S2..
		  begin
		     Counter = Counter - 1; //decrease counter
		     if(Counter == 0)//check if counter is 0
			begin
			 TIMEOUT  = 1;//change the value of TIMEOUT to 1
			 
		    	end
		  end

		if(currentState == S1) // check if current state is S1..
		   begin
		     Counter_2 = Counter_2 - 1; //decrease counter_2
		     if(Counter_2 == 0) //check if counter_2 is 0
			begin
				TIMEOUT_2 = 1;//change the value of TIMEOUT_2 to 1
				
			end
		  end
	     end
	// block for output, triggered by currentState 	        
	always @(currentState)
	     begin:OUTPUT_LOGIC
		//check which is the current state
                case(currentState)
		  S0: begin // in case of S0, set GRN to 1 and YLW,RED to 0
			GRN = 1'b1; 
			YLW = 1'b0;
		        RED = 1'b0;
			TIMEOUT = 0;
			Counter = 15000; //reset counter to his first value
		      end
		  S1:begin // in case of S1, set YLW to 1 and GRN ,RED to 0
 			GRN = 1'b0;
			YLW = 1'b1;
 			RED = 1'b0;
		     end
		  S2: begin //in case of S2, set RED to 1 and YLW, GRN to 0
			GRN = 1'b0; 
			YLW = 1'b0; 
			RED = 1'b1;
			TIMEOUT_2 = 0;
			Counter_2 = 3000; // reset counter_2 to his first value
			end
		  default: begin // in any other case, set GRN to 1 and YLW,RED to 0
			     GRN = 1'b1; 
			     YLW = 1'b0;
  			     RED = 1'b0;
			    end
		endcase
	     end
endmodule 