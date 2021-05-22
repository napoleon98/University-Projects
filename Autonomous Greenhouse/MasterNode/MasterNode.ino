#include "DHT.h"
#include <SPI.h>
#include <RF22.h>
#include <RF22Router.h>
#define HIGH_TEMP 31
#define HIGH_HUM 35
#define NUM_OF_NODES 3

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
int row = 1; //leei se poia grammh anoikei o master node
//RF STUFF
#define MY_ADDRESS 101 

#define DESTINATION_ADDRESS_1 1 

RF22Router rf22(MY_ADDRESS);
int packetCounter=0;
long randNumber;
boolean successful_packet = false;
int max_delay=3000;

DHT dht(DHTPIN, DHTTYPE);

int rainPin = A0;
int orangeLED = 6; //will be used to show us if there is damage in watering system
int redLED = 7; //will be used to show us if temperature is high or not
int blueLED = 8; //will be used to show us if humidity is high or not
// you can adjust the threshold value
int thresholdValue = 600; //if measurement of soil moisture is higher than thresholdValue, watering system will be turned on
int extremeLowHum = 720; // if measurement of soil moisture is higher than extremeLowHum, watering system has damage
int wateringDone = 380; // if measurement of soil moisture is lower than wateringDone, watering system should be turned off
int needWater = 0; //will be used  to store if the watering system needs to be turned on(1) or off(0)


int nodeNum[NUM_OF_NODES];// will be filled with 1 for each node that has sent his message
int tNode[NUM_OF_NODES];//will be stored measurements of temperature that each simple node has sent
int hNode[NUM_OF_NODES];//will be stored measurements of humidity  that each simple node has sent
int soilNode[NUM_OF_NODES];//will be stored measurements of soil humidity  that each simple node has sent
int waterSystemDamage[NUM_OF_NODES];//will be filled with 1 or 0 for each node that has damage or not(respectively) to his watering system

long int received_value=0;//initialisation of variable in which will be stored the incoming message, as an integer


void setup() {
    Serial.begin(9600);
    
 //initialize rf--begin 
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
  
  //initialize rf--end
 
    
  //initialize gpio pins--begin  
  pinMode(rainPin, INPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(orangeLED, OUTPUT);
  digitalWrite(blueLED, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(orangeLED, LOW);
  //initialize gpio pins--end
  
  randomSeed(analogRead(A0));//reading  analog pin A0, we give a different seed every time setup function is executed
  //DHT setupBEGIN
  dht.begin();
  //DHT setupEND
    
  //initialise the arrays filling them with 0 
  int z=0;
  for(z=0;z<NUM_OF_NODES;z++){
nodeNum[z]=0;
tNode[z]=0;
hNode[z]=0;
soilNode[z]=0;
waterSystemDamage[z]=0;
  
}  
  delay(1000); // delay for 1 s  
}

void loop() {

  nodeNum[0]=1;//initialise the first cell with 1. First cell is for this (master) node
  int whoSent = 0;//will be used to store from which node we received the message and is initialised with 0
  int allSent = 1;//will help us to understand if at least on node didn't send his message
  received_value = 0;//reset value to 0
   //RECEIVE MODE--begin 
  uint8_t buf[RF22_ROUTER_MAX_MESSAGE_LEN];
  char incoming[RF22_ROUTER_MAX_MESSAGE_LEN];
  memset(buf, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  memset(incoming, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  uint8_t len = sizeof(buf); 
  uint8_t from = 0;
  if (rf22.recvfromAck(buf, &len, &from)) // I'm always expecting to receive something from any legitimate transmitter (DESTINATION_ADDRESS_1, DESTINATION_ADDRESS_2). If I do, I'm sending an acknowledgement
  {
    buf[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
    memcpy(incoming, buf, RF22_ROUTER_MAX_MESSAGE_LEN); // I'm copying what I have received in variable incoming
    received_value=atol((char*)incoming); // transforming my data into an integer   
  }
  //RECEIVE MODE--end
  
  whoSent = ((int)from)%100;//store who sent the current message
  
  int i=0;
  int k=0;
  
   //store to the proper cell of each array the measurement, according to who node has sent the current message
  for(i=1;i<(NUM_OF_NODES+1);i++){
    //check who node has just sent
    if(whoSent == i){
      
      nodeNum[i-1]=1;//store 1,so we know that "whoSent" node has sent 
      //decode received value, knowing in which 3 digits is stored each measurement  
      soilNode[i-1] = received_value / 1000000;
      received_value = received_value % 1000000;
      tNode[i-1] = received_value/1000;
      received_value = received_value % 1000;
      hNode[i-1]=received_value;
    }
    }
 //ckeck if any node of row the has not send his messages 
  for(k=0;k<NUM_OF_NODES;k++){
    if(nodeNum[k]==0)allSent=0;//for each node that has not send yet, store 1 to his cell 
    
  }
  //check if all nodes of the row has sent 
  if(allSent==1){
      Serial.print("Packets counted Simple: ");
      Serial.print(packetCounter);
    Serial.println("--------------------------------------");
    
    process();//cal process function
    
  } 
}

void process(){
  //DHT CodeBEGIN
  
  int p =0;
  for(p=0;p<NUM_OF_NODES;p++){
    
    nodeNum[p] = 0;
    
  }
  
  float h = dht.readHumidity();        // read humidity
  float t = dht.readTemperature();     // read temperature
  int hasDamage = 0;

    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    //store humidity and temperature to first cell of the arrays.this cell is for this node
   tNode[0] = t;
   hNode[0] = h;
  //read soil humidity
  soilNode[0] = analogRead(rainPin); //0 -> 1023 ... 1023 = 0% humidity
  Serial.print("Master node soil: ");
  Serial.print(soilNode[0]);
  Serial.println(" (0 means: Soil humidity 100% -> 1023 means: Soil humidity 0%) ");
  
  //calculations
  int meanHum = 0;
  int meanTemp = 0;
  int meanSoil = 0;
  
  
  int l=0;
  int m=0;
  
  //calcualte mean temperature,humidity and soil moisture for this row
  for(l=0;l<NUM_OF_NODES;l++){
    meanHum = meanHum + hNode[l];
    meanTemp = meanTemp + tNode[l];
    meanSoil = meanSoil + soilNode[l];
    
  }
  
  meanHum = meanHum / NUM_OF_NODES;
  meanTemp = meanTemp / NUM_OF_NODES;
  meanSoil = meanSoil / NUM_OF_NODES;
  
  //check if watering system needs to be turned on/iff and if there is damage in any node
  if(meanSoil > thresholdValue) needWater = 1;
  if(meanSoil < wateringDone) needWater = 0;
  if(needWater == 1)
    for(m=0;m<NUM_OF_NODES;m++){
      if(soilNode[m]>extremeLowHum){
        waterSystemDamage[m]=1;
        Serial.print("Node ");
        Serial.print(m,DEC);
        Serial.println(" has water system damage");
        hasDamage = 1;
        }
      else {waterSystemDamage[m] = 0;
        Serial.println("Watering system works fine");
      }
    }
    Serial.print("mean row room temp: ");
  Serial.println(meanTemp);
  
  Serial.print("mean row room hum: ");
  Serial.println(meanHum);
  Serial.print("mean row soil: ");
  Serial.println(meanSoil);
  //turn on/off leds for high temperature,humidity and watering system damage 
  if(meanTemp > HIGH_TEMP)digitalWrite(redLED, HIGH);
  if(meanTemp < HIGH_TEMP)digitalWrite(redLED, LOW);
  if(meanHum > HIGH_HUM)digitalWrite(blueLED, HIGH);
  if(meanHum < HIGH_HUM)digitalWrite(blueLED, LOW);
  if(hasDamage==1)digitalWrite(orangeLED,HIGH);
  if(hasDamage==0)digitalWrite(orangeLED,LOW);
  
   // the following variables are used in order to transform my integer measured value into a uint8_t variable, which is proper for my radio
  char data_read[RF22_ROUTER_MAX_MESSAGE_LEN];
  uint8_t data_send[RF22_ROUTER_MAX_MESSAGE_LEN];
  memset(data_read, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  memset(data_send, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);    
  sprintf(data_read, "R%dW%dT%dH%dD%d", row,needWater,meanTemp,meanHum,hasDamage); // I'm copying the number of row, variable "needwater", measurements and variable "hasdamage" into variable data_read,storing one different letter before number of row (R),needwater(W),hasdamage(D) and each measurement(T,H)
  data_read[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0'; 
  memcpy(data_send, data_read, RF22_ROUTER_MAX_MESSAGE_LEN); // now I'm copying data_read to data_send
  //number_of_bytes=sizeof(data_send); // I'm counting the number of bytes of my message
  successful_packet = false;
  Serial.print("tha steilw: ");
  Serial.println(data_read);
  //TRANSMIT MESSAGE -- begin  
  while (!successful_packet){ 
  
  if (rf22.sendtoWait(data_send, sizeof(data_send), DESTINATION_ADDRESS_1) != RF22_ROUTER_ERROR_NONE) // I'm sending the data in variable data_send to DESTINATION_ADDRESS_1... cross fingers
  {
    Serial.println("sendtoWait failed"); // for some reason I have failed
  randNumber=random(200,max_delay);
  delay(randNumber);//Delay random number ms and try to send again-ALOHA
  }
  else
  {
    Serial.println("sendtoWait Successful"); // I have received an acknowledgement from DESTINATION_ADDRESS_1. Data have been delivered!
  successful_packet = true;
  packetCounter++;
 }
  //TRANSMIT MESSAGE -- end
  }
}
