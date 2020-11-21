#include <platform.h>
#include <gpio.h>
#include "delay.h"
#include "leds.h"


//If the value that defined in TYPE_OF_EXPERIMENT is on ,the first type experiment will be executed, else the second.
# define TYPE_OF_EXPERIMENT "on" ; 


volatile int stop=0;// the value of this variable will change into interrupt handler,so I declare it as volatile.

void button_press_isr(int sources) { 
	
	gpio_set(P_DBG_ISR, 1);
	
	if ((sources << GET_PIN_INDEX(P_SW)) & (1 << GET_PIN_INDEX(P_SW))) {
			
			
			stop = 1;// change the value of "stop" from 0 to 1
	
	} 
	gpio_set(P_DBG_ISR, 0);
}


__asm void store(long long int value){
	
	  MOV32 r1, #0x20000450 // move in r1 the address 0x20000450
		
    STR r0, [r1] //store r0, into memory pointed by r1
		BX lr // return in main


}




int main(void) {
	
	// Initialise LEDs. 
	leds_init(); 
	leds_set(0, 0, 0);
	
	
	// Set up debug signals.
	gpio_set_mode(P_DBG_ISR, Output); 
	gpio_set_mode(P_DBG_MAIN, Output);
	
	
	// Set up on-board switch. 
	gpio_set_mode(P_SW, PullUp); 
	gpio_set_trigger(P_SW, Rising);
	gpio_set_callback(P_SW, button_press_isr);
	
	__enable_irq(); // enable all interrupts
	
	long long int cycles = 0; // counter for execution cycles
	int countExps = 0; // counter for number of experiments
	long  long int arrayOfResponse[5]; // array with results from the experiments
	
	long	long int  average = 0; // the average of five experiments will be saved here 
	int i=0;// counter
	
	
	int random_delay=0; // in this variable will be stored the random number of delay in ms, between two experiments
	
	while (1) { // infinite loop
		
		while(countExps != 5){ // the inside code,will run 5 times,for 5 experiments
			
			gpio_toggle(P_DBG_MAIN); // toggle mains' debug signal
		
		 random_delay = (rand(0) % 5000) + 2000; // random delay between 2 experiments 2-7 secs
			
			//check which experiment will be executed
			
			#ifdef TYPE_OF_EXPERIMENT //If TYPE_OF_EXPERIMENT has been defined ,the led is on at start...so..
				delay_ms(random_delay); // ...delay "random_delay" ms between 2 experiments
				leds_set(1,1,1); // ...turn the led on
				
			#endif
			
			#ifndef TYPE_OF_EXPERIMENT//If TYPE_OF_EXPERIMENT has not been defined .. 
				leds_set(1,1,1); // ...turn the led on, so the user will understand that the experiments is about to start
				delay_ms(random_delay); // ...delay "random_delay" ms between 2 experiments
				leds_set(0,0,0); // ..turn the led off
			#endif
			
			while(stop != 1){//count cycles until, the button is pressed...Then, "stop" is 1 and the loop stops
				
				cycles++; // increase number of cycles
				delay_cycles(1500); // force every repetition in while,to "cost" 5000 processor's cycles
				
			
			}
		//when the above loop is terminated,depending on the type of experiment, the led should be turned off or on.
			#ifdef TYPE_OF_EXPERIMENT
			
				leds_set(0,0,0); // turn the led off
				
			#endif
		
			#ifndef TYPE_OF_EXPERIMENT
				
				leds_set(1,1,1); // turn the led on
			
			#endif
			arrayOfResponse[countExps] = cycles;
			
			//prepare the variables for the next experiment
			cycles = 0;
			stop = 0;
			
			countExps++;// increase counter for experiments
		
		}
		//just turn the led off in the end ,in case the type of experiment is not defined.
		#ifndef TYPE_OF_EXPERIMENT
			
			leds_set(0,0,0);
		
		#endif
		
		
		
	// calculate the average for the 5 experiments
		while(i!=5){
			
			
			arrayOfResponse[i] = arrayOfResponse[i]*1500; // calculate the number of real processor's cycles and put it in array
			arrayOfResponse[i] = arrayOfResponse[i]*(0.0000625); // calculate the real time ,multiplying cycles with processor's clock
			
			average += arrayOfResponse[i];// add the element i of arrayOfResponse to average
			i++;
		}
		average = average/5;  //divide with 5 to caclulate the average
		store(average); // store the average in memory 
		
		delay_ms(15000);// Delay 15 seconds and start over, the next five experiments.
		// change the value of countExps,average and i to 0,for the next five experiments
		countExps = 0;
		average = 0; 
		i=0;
	}

}

// *******************************ARM University Program Copyright © ARM Ltd 2016*************************************   
