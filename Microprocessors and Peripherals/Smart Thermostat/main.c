#include <platform.h>
#include <gpio.h>
#include "delay.h"
#include "lcd.h"
#include "switches.h"
#include "leds.h"
#include <stdio.h>
#include "timer.h"
#include "stm32f4xx_hal.h" 
 
#define Echo_Pin PA_0 // Proximity sensor's echo pin 0
#define Trigger_Pin PA_1 // Proximity sensor's trigger pin 1
#define Temperature_Sensor_Pin PC_1 // Temperature senosr Pin 1
 
//volatile variablaes that change in interrupt handlers
volatile int signal_proximity = 0;
volatile int signal_5secElapsed = 0;
volatile int signal_10secElapsed = 0;
volatile int timer_systick_flag = 0;

//if this var is 1,we know that the last message that was printed in Lcd was from Proximity sensor's activation
int IamClose = 0;

float upper_limit = 28;// if temperature is above "upper_limit" a red led will be turned on 
float lower_limit = 26;// if temperature is under "lower_limit" a blue led will be turned on
//they are used in timer's initialization
TIM_HandleTypeDef  TIM3_Handle, TIM4_Handle;

//these variables  help us calculate the width of echo pulse (proximity sensor)
uint32_t timer_counter_start = 0;//it takes its value when echo pin is high
uint32_t timer_counter_finish = 0;//it takes its values when echo pin is low(after high)
uint32_t echo_pulse_width = 0;	
float distance_cm = 0;

char str[20] = {0};
float array_of_temperatures[24]={0};//in this array will be saved 24 measures of temperature
int counter_temps = 0;//above array's counter
float mean_temperature = 0;//the mean will be stored here
int calculating_mean = 0;//flag,that let us know if mean has been calculated,so should be printed in Lcd for the next 10 seconds
int print_mean = 1;//when it is 1,mean should be printed again.another message was interleaved before 10 seconds elapse

//initialize switches,leds ,lcd and timers 3 and 4
void Initializations(){
    switches_init();
    leds_init();
    lcd_init();
   
   
	
      
   
		gpio_set_mode(Echo_Pin,PullDown);
    gpio_set_mode(Trigger_Pin,Output);
    gpio_set(Trigger_Pin,0);
    
	
//****************************************************	
		
	
//*********TIM3 initialization*********************	
	__TIM3_CLK_ENABLE();
	TIM3_Handle.Init.Prescaler = 1599; //Fistly, Clock is 16Mhz, aftre scaling it is 16000000/1600=10000Hz -> period = 0.1ms
  TIM3_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
  TIM3_Handle.Init.Period = 1900; // 190ms period
  TIM3_Handle.Instance=TIM3;
  HAL_TIM_Base_Init(&TIM3_Handle);
  HAL_TIM_Base_Start_IT(&TIM3_Handle);
  HAL_NVIC_SetPriority(TIM3_IRQn,0,1);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
	
	//*********TIM4 initialization*********************	
   __TIM4_CLK_ENABLE();
	TIM4_Handle.Init.Prescaler = 1599; //Fistly, Clock is 16Mhz, aftre scaling it is 16000000/16000=10000Hz -> period = 0.1ms
  TIM4_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
  TIM4_Handle.Init.Period = 50000; // 5sec period 50000
  TIM4_Handle.Instance=TIM4;
  HAL_TIM_Base_Init(&TIM4_Handle);
  HAL_TIM_Base_Start_IT(&TIM4_Handle);
  HAL_NVIC_SetPriority(TIM4_IRQn,0,1);
  HAL_NVIC_EnableIRQ(TIM4_IRQn);
   
   
    
 
}
 //Irq handler for timer 3,it is called when period is elapsed
void TIM3_IRQHandler(){

		HAL_TIM_IRQHandler(&TIM3_Handle);
		signal_proximity = 1;// change value of signal_proximity to 1 every 100 ms


}


//Irq handler for timer 4, it is called when 5sec period is elapsed
void TIM4_IRQHandler(){
		HAL_TIM_IRQHandler(&TIM4_Handle);
		signal_5secElapsed = 1;// change value of signal_5secElapsed to 1
	 
}

//Systick's timer interrupt handler
void timer_isr(){
	
	signal_10secElapsed++; // increase signal_10secElapsed, every sec
	if(signal_10secElapsed == 10){// in 10th sec..
			timer_systick_flag = 1;//.. change value of timer_systick_flag to 1..
		  signal_10secElapsed = 0;//..reset signal_10secElapsed to 0
	}

}


//send the 10us pulse that starts the proximity sensor
void Trigger_Pulse(){
    gpio_set(Trigger_Pin,1);
    delay_us(10);
    gpio_set(Trigger_Pin,0);
}


//********************Temperature_Sensor********************************** 
//initialization of sensor as it is described  in sensor's datasheet
int Temperature_Sensor_Init(void){
    int response=0;
    gpio_set_mode(Temperature_Sensor_Pin,Output);
    gpio_set(Temperature_Sensor_Pin,0);
    delay_us(480);
    gpio_set_mode(Temperature_Sensor_Pin,Input);
    delay_us(80);
    if(!gpio_get(Temperature_Sensor_Pin))
		{
			response=1;
		}	
    else 
		{
			response=-1;
    }
		delay_us(400);
   
return response;
 
}
 //write function of sensor as it is described  in sensor's datasheet
void Temperature_Sensor_Write(uint8_t data){
    gpio_set_mode(Temperature_Sensor_Pin,Output);
    int i=0;
    for(i=0;i<8;i++){
			if((data &(1<<i))!=0){
				
				gpio_set_mode(Temperature_Sensor_Pin,Output);
				gpio_set(Temperature_Sensor_Pin,0);
				delay_us(1);
				gpio_set_mode(Temperature_Sensor_Pin,Input);
				delay_us(60);
   
			}
			else {
				
				gpio_set_mode(Temperature_Sensor_Pin,Output);
				gpio_set(Temperature_Sensor_Pin,0);
				delay_us(60);
				gpio_set_mode(Temperature_Sensor_Pin,Input);
			
			}
   
    }
   
   
}
 //read function of sensor as it is described  in sensor's datasheet
uint8_t Temperature_Sensor_Read(void){
   
    uint8_t val=0;
    gpio_set_mode(Temperature_Sensor_Pin,Input);
   
    int i=0;
   
    for(i=0;i<8;i++){
			gpio_set_mode(Temperature_Sensor_Pin,Output);
			gpio_set(Temperature_Sensor_Pin,0);
			delay_us(2);
			gpio_set_mode(Temperature_Sensor_Pin,Input);
       
			if(gpio_get(Temperature_Sensor_Pin))
			{
				val|=1<<i;
			}
			delay_us(60);
       
		}
return val;
}
 //Print temperature with suitable messages in Lcd. The messages depend on meanonly's value
void Print_Temperature(float temperature, int meanOnly){

    char string[30]={0};
    lcd_clear();
    lcd_set_cursor(0,0);
		if(meanOnly == 1){// in case of 1,print the mean temperature
				sprintf(string,"MEAN TEMPERATURE: %.2f C",temperature);
				lcd_print(string);
		}
		 if(meanOnly == 0) {// in case of 0,print the mean temperature and the temperature now
			sprintf(string,"TEMP NOW:%.2fC MEAN TEMP:%.2fC",array_of_temperatures[counter_temps - 1],temperature);
				lcd_print(string);
		}
		 
		
		
}
//  temperature Sensor's main function 
void Temperature_Sensor(){
    int tempeInitReturn;
    uint8_t temperatureByte1,temperatureByte2;
    uint16_t temperature_all_bits;
    float final_temperature;
   
    tempeInitReturn=Temperature_Sensor_Init();
    delay_ms(1);
    Temperature_Sensor_Write(0xCC);
    Temperature_Sensor_Write(0x44);
    delay_ms(800);
    tempeInitReturn=Temperature_Sensor_Init();
    delay_ms(1);
    
	  Temperature_Sensor_Write(0xCC);
    Temperature_Sensor_Write(0xBE);   
   
   	temperatureByte1=Temperature_Sensor_Read();
    temperatureByte2=Temperature_Sensor_Read();
   

// if I am close to the device(so the proximity sensor is activated) clear lcd and set some flags 

if(IamClose){	
    lcd_clear();
		lcd_set_cursor(0,0);
	  
		print_mean = 1;
		IamClose = 0;
}	
   
    temperature_all_bits=(temperatureByte2<<8)|(temperatureByte1); // concatinate the 2 byte variables in one variable
   
    final_temperature=(float)temperature_all_bits/16;
		
   array_of_temperatures[counter_temps] = final_temperature; // put the temperature in an array of 24 elements.
// if temperature is above upper limit, print "It's Hot" in Lcd and turn the red Led on		
	if(final_temperature > upper_limit){
			if(calculating_mean == 0){
					char string[30]={0};
					lcd_clear();
					lcd_set_cursor(0,0);
					sprintf(string,"    It's Hot        %.2f C",final_temperature);
					lcd_print(string);
					print_mean = 1;
		}
			leds_set(1,1,0);
	}
	
// if temperature is below lower limit, print "It's Cold" in Lcd and turn the green Led on
	else if(final_temperature < lower_limit ){
				if(calculating_mean == 0){	
					char string[30]={0};
					lcd_clear();
					lcd_set_cursor(0,0);
					sprintf(string,"    It's Cold        %.2f C",final_temperature);
					lcd_print(string);
					
					print_mean = 1;
				}
			 leds_set(0,0,1);			
	}
	else {//clean screen and turn off leds while temperature is between limits
		leds_set(0,0,0);
		lcd_clear();
		lcd_set_cursor(0,0);
	}
	
	
	
    counter_temps++; // increase counter, so in the next call of function, temperature will be stored in next array's position
    delay_ms(30);
}
 //****************Proximity Sensor*****************************

void Proximity_Sensor(){
		
	//if timer's handler has changed signal_proximity's value to 1(every 100ms)..	
		if(signal_proximity){
			
			
		//if mean should be printed in Lcd but "I was close"	so the proximity sensor was acivated and printed in Lcd,clear Lcd and let mean be printed again
			if(IamClose && (calculating_mean == 1)){	
				lcd_clear();
				lcd_set_cursor(0,0);
	  
				print_mean = 1;//let mean be printed again
				IamClose = 0;//reset this flag
			}	
				signal_proximity = 0;//change value to 0
				Trigger_Pulse();//send the first 10us pulse for activation
			//Do nothing while echo pin is low
				while(!gpio_get(Echo_Pin)){ 
						
				}
			//when echo pin is high,store timer's counter value in 	"timer_counter_start"
				timer_counter_start = TIM3->CNT ;
			// Do nothing while echo pin is high	
				while(gpio_get(Echo_Pin)){
					  
				}
			///when echo pin is low,store timer's counter value in 	"timer_counter_finish"	
				timer_counter_finish = TIM3->CNT;
		    echo_pulse_width = timer_counter_finish - timer_counter_start;//find the difference os two values,now we now how many timer's period elapsed
			

				echo_pulse_width = echo_pulse_width * 100; // find echo pulse width in us
				distance_cm = echo_pulse_width/58;//find the distance in cm
				//check if someone is closer than 10 cm from the sensor
				if(distance_cm < 10){
					
//check if we are in mean's ten seconds window			
					if((counter_temps == 0) && (calculating_mean == 1) ){
							char string[30]={0};
							lcd_clear();
							lcd_set_cursor(0,0);
							sprintf(string,"TEMP NOW:%.2fC MEAN TEMP:%.2fC",array_of_temperatures[23],mean_temperature);
							lcd_print(string);
						  print_mean = 0;
							
							IamClose = 1;
					}
				//print temperature now and mean temperature of the previous 2 minutes	
					else{
						Print_Temperature(mean_temperature, 0);
						print_mean = 0;
						
						IamClose = 1; //change the value of "IamClose" to 1.
					}
				}
				
		
		}

}
//this function is called every 5 secs for measuring temperature
void Temperature_Every5secs(){
	//if timer's handler has changed the value of  signal_5secElapsed to 1(every 5 secs)
		if(signal_5secElapsed){
				
				signal_5secElapsed = 0;//change the value to 0
				Temperature_Sensor();//call Temperature Sensor,where the temperature is measured
			// check if we have measured 24 times the temperature	
			if(counter_temps == 24){
						
						counter_temps = 0;//reset counter to 0
						mean_temperature = 0;//reset mean's variable to 0
						for(int i=0;i<24;i++){
								mean_temperature += array_of_temperatures[i];// sum all elements of array in mean_temperature
						}
						
						mean_temperature = mean_temperature/24;//find the mean temperature dividing with 24
						
						calculating_mean = 1;//when  24 temperatures have been measured,and mean is calculating ,put 1 to calculating_mean
						timer_enable();//enable timer for 10secs printing mean in Lcd
						Print_Temperature(mean_temperature, 1);// print the mean temperature in lcd
				}
			
		
		}
		
}
int main(void) {
    
   
    
    Initializations();  
	timer_init(1000000);//initialize systick's timer period to 1sec 
	timer_set_callback(timer_isr); //set timer's handler that woyld be called when timer expires
    __enable_irq();//enable interrupts
	timer_disable();//disable timer
	//force all signals that handlers change to be 0
	signal_5secElapsed = 0;
	signal_proximity = 0;
	signal_10secElapsed = 0;
	
	timer_systick_flag = 0;
	
	leds_set(1,1,1);//turn the leds off
	
	 
	
	while(1){
	
		//call functions for temperature and distance measuring 
		Temperature_Every5secs();
		Proximity_Sensor();
		
		
		
			//******** print mean time only when it is essential*************
			/*
			if 2 minutes elapsed and we 24 temperatures have been measured(calculating mean is 1)
			The mean should be printed to Lcd for 10 seconds, every 2 minutes.
			there is one variable(print_mean)that takes 1 ,and force mean to be printed again,
			whenever another message was printed in 10 seconds mean printing window.
			
			*/
			if(calculating_mean){
				
				if(print_mean){
						Print_Temperature(mean_temperature, 1);
						print_mean = 0;//reset to 0,so if nothing was printed to Lcd for 10 seconds,there is no reason for mean to be printed again
				}
				
				//check if 10 secs elapsed. timer_systick_flag changes to 1 when this happens
				if(timer_systick_flag){
					//reset to their first values all variables that let mean be printed for 10 seconds
						print_mean = 1;
						calculating_mean = 0;
						timer_systick_flag = 0;
					//disable timer and clear Lcd
					  timer_disable();
						lcd_clear();
						lcd_set_cursor(0,0);
				}
					
				
		}
		
	
	}
   
    
   
}

// *******************************ARM University Program Copyright © ARM Ltd 2016*************************************   
