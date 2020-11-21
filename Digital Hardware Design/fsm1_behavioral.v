module fsm1_behavioral(output reg Dout, input wire Clock, Reset, Din);
 //declaration of necessary parameters
	localparam [1:0] 
		start = 2'b00,
		midway = 2'b01,
		done = 2'b10;
	reg [1:0] currentState, nextState; // declaration of two signals for current and next state
	//block for storing the machine's state, triggered by positive edge of clock or negative edge of Reset
	always @(posedge Clock or negedge Reset)
	     begin: STATE_MEMORY
		  if(!Reset)
		     currentState <=start; // if reset is active, store start in current state..
		  else 
		     currentState <=nextState; // ..else store nextstate in current state
	     end
       //block for next state, triggered by currentState or Din
	always @(currentState or Din)
             begin: NEXT_STATE_LOGIC
	//check which is the current state
		case(currentState)
		   start: if(Din) nextState=midway; // in case of start, check the Din signal, if it is 1,set Midway in next state..
			 else nextState=start;// .. else set start in next state
		   midway: nextState=done; // in case of midway just set Done in next state
		   done: nextState=start; // in case of Done ust set start in next state
		   default: nextState=start; // in any other case, set start in next state
		endcase 
	     end
	// block for output, triggered by currentState or Din
	always @(currentState or Din)
	     begin:OUTPUT_LOGIC
	//check which is the current state
                case(currentState)
		   done: if(Din) // in case of done, check the Din signal, if it is 1, set 1 to Dout
			    Dout = 1'b1;
			 else   // else set 0 to Dout
			    Dout = 1'b0;
		   default: Dout = 1'b0;
		endcase
	     end 
endmodule 