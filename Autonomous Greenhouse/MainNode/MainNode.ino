#include <RF22.h>
#include <RF22Router.h>
#include <SPI.h>

#define MY_ADDRESS 1 
#define DESTINATION_ADDRESS_1 60 //1 -> master node of row
#define num_of_rows 1
RF22Router rf22(MY_ADDRESS);
//RF22 rf22;
int received_value=0;
//int blueLED = 8;
//int orangeLED = 6;

int waterRow[num_of_rows];

int fanRow[num_of_rows];


void setup() {
    Serial.begin(9600);
    waterRow[0] = 8;//orizoume to pin pou antistoixei sto potistiko ths kathe grammhs
    fanRow[0] = 6;
	

  if (!rf22.init()) // initialize my radio
    Serial.println("RF22 init failed");
  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
  if (!rf22.setFrequency(434.0)) // set the desired frequency
    Serial.println("setFrequency Fail");
  rf22.setTxPower(RF22_TXPOW_20DBM); // set the desired power for my transmitter in dBm
  //1,2,5,8,11,14,17,20 DBM
  rf22.setModemConfig(RF22::OOK_Rb40Bw335  ); // set the desired modulation
  //modulation

  // Manually define the routes for this network
  rf22.addRouteTo(DESTINATION_ADDRESS_1, DESTINATION_ADDRESS_1); // tells my radio card that if I want to send data to DESTINATION_ADDRESS_1 then I will send them directly to DESTINATION_ADDRESS_1 and not to another radio who would act as a relay
  
    int k = 0;
	for(k=0;k<num_of_rows;k++){
		
		pinMode(waterRow[k], OUTPUT);
		digitalWrite(waterRow[k], LOW);
	}
	
	for(k=0;k<num_of_rows;k++){
		
		pinMode(fanRow[k], OUTPUT);
		digitalWrite(fanRow[k], LOW);
	}
	  
    delay(1000); // delay for 1 s
  
}

void loop() {
	
  char message[RF22_ROUTER_MAX_MESSAGE_LEN];
  int row=0;
  int temp=0;
  int needWater = 0;
  int hum=0;
  int dmg=0;

  
  uint8_t buf[RF22_ROUTER_MAX_MESSAGE_LEN];
  char incoming[RF22_ROUTER_MAX_MESSAGE_LEN];
  memset(buf, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  memset(incoming, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  uint8_t len = sizeof(buf); 
  uint8_t from;
  //digitalWrite(5, LOW);
  if (rf22.recvfromAck(buf, &len, &from)) // I'm always expecting to receive something from any legitimate transmitter (DESTINATION_ADDRESS_1, DESTINATION_ADDRESS_2). If I do, I'm sending an acknowledgement
  {
 //   digitalWrite(5, HIGH);
    buf[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
    memcpy(incoming, buf, RF22_ROUTER_MAX_MESSAGE_LEN); // I'm copying what I have received in variable incoming
    //Serial.print("got request from : ");
    //Serial.println(from, DEC);
    //received_value=atoi((char*)incoming); // transforming my data into an integer

    //Serial.println(received_value); // and showing them on the screen
		
  
  int i=0;
  int r=0;
  
 //Decode Message
	  if(incoming[i] == 'R'){
		  i++;
		  while(incoming[i]!='W'){
			  message[r]=incoming[i];
			  r++;
			  i++;
		  }
		  message[r] = '\0';
		  r=0;
		  row=atoi((char*)message);
		  memset(message, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
		  
	  }
	  if(incoming[i] == 'W'){
		  i++;
		  while(incoming[i]!='T'){
			  message[r]=incoming[i];
			  r++;
			  i++;
		  }
		  message[r] = '\0';
		  r=0;
		  needWater=atoi((char*)message);
		  memset(message, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
		  
	  }
	  
	  if(incoming[i] == 'T'){
		  i++;
		  while(incoming[i]!='H'){
			  message[r]=incoming[i];
			  r++;
			  i++;
		  }
		  message[r] = '\0';
		  r=0;
		  temp=atoi((char*)message);
		  memset(message, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
		  
	  }
	  
	  if(incoming[i] == 'H'){
		  i++;
		  while(incoming[i]!='D'){
			  message[r]=incoming[i];
			  r++;
			  i++;
		  }
		  message[r] = '\0';
		  r=0;
		  hum=atoi((char*)message);
		  memset(message, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
		  
	  }
	  
	  if(incoming[i] == 'D'){
		  i++;
		  while(incoming[i]!='\0'){
			  message[r]=incoming[i];
			  r++;
			  i++;
		  }
		  message[r] = '\0';
		  r=0;
		  dmg=atoi((char*)message);
		  memset(message, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
		  
	  }
	  
		if(needWater == 1)digitalWrite(waterRow[row-1], HIGH);
		else digitalWrite(waterRow[row-1], LOW);

    
		
		if(temp>31)digitalWrite(fanRow[row-1], HIGH);
		else digitalWrite(fanRow[row-1], LOW);
		
    Serial.print("Row ");
    Serial.print(row,DEC);
    Serial.print(" Need water ");
    Serial.print(needWater,DEC);
    Serial.print(" Mean Temp: ");
    Serial.print(temp,DEC);
    Serial.print(" Mean Humidity: ");
    Serial.print(hum,DEC);
    Serial.print("% and Has Damage: ");
    Serial.println(dmg,DEC);
    Serial.println("---------------------------------");
    
}
  //Decode Message
  
}
