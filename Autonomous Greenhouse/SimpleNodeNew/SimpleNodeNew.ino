#include "DHT.h"
#include <SPI.h>
#include <RF22.h>
#include <RF22Router.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

//RF STUFF
#define MY_ADDRESS 102 
#define DESTINATION_ADDRESS_1 101 //1 -> master node of row
RF22Router rf22(MY_ADDRESS);
//RF22 rf22;

DHT dht(DHTPIN, DHTTYPE);

int rainPin = A0;
long randNumber;
boolean successful_packet = false;
int max_delay=3000;
int number_of_bytes=0; // will be needed to measure bytes of message


void setup() {
    Serial.begin(9600);
  //SETUP MY RF--begin
  if (!rf22.init()) // initialize my radio
    Serial.println("RF22 init failed");  
    if (!rf22.setFrequency(434.0))
    Serial.println("setFrequency Fail");
    rf22.setTxPower(RF22_TXPOW_20DBM);  
    rf22.setModemConfig(RF22::OOK_Rb40Bw335  ); // set the desired modulation
    rf22.addRouteTo(DESTINATION_ADDRESS_1, DESTINATION_ADDRESS_1); // tells my radio card that if I want to send data to DESTINATION_ADDRESS_1 then I will send them directly to DESTINATION_ADDRESS_1 and not to another radio who would act as a relay
  //SETUP MY RF--end
    pinMode(rainPin, INPUT);
    randomSeed(analogRead(A0));
    //DHT setupBEGIN
    dht.begin();
    //DHT setupEND
  
    delay(1000); // delay for 1 s
  
}

void loop() {
    
  //DHT CODE -- begin
  float hf = dht.readHumidity();        // read humidity
  float tf = dht.readTemperature();     // read temperature 
  long int h=hf;
  long int t=tf;
    if (isnan(hf) || isnan(tf)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
  //DHT CODE -- end
	
  //READ SOIL MOISTURE SENSOR
  int sensorValue = analogRead(rainPin); //0 -> 1023 ... 1023 = 0% humidity
  
  //ENCODE PREVIOUS MEASUREMENTS IN ONE COMPLETE PACKET
  long int completePacket =  sensorValue * 1000000 + t * 1000 + h;
  
  //serial prints
  Serial.print("the room temperature is ");
  Serial.println(tf);
  Serial.print("the room humidity is ");
  Serial.println(hf); 
  Serial.print("Soil is ");
  Serial.print(sensorValue);
  Serial.println(" (0 means: Soil humidity 100% -> 1023 means: Soil humidity 0%) ");
	
  // the following variables are used in order to transform my integer measured value into a uint8_t variable, which is proper for my radio
  char data_read[RF22_ROUTER_MAX_MESSAGE_LEN];
  uint8_t data_send[RF22_ROUTER_MAX_MESSAGE_LEN];
  memset(data_read, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  memset(data_send, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);    
  sprintf(data_read, "%ld", completePacket); // I'm copying the measurement sensorVal into variable data_read
  data_read[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0'; 
  memcpy(data_send, data_read, RF22_ROUTER_MAX_MESSAGE_LEN); // now I'm copying data_read to data_send
  number_of_bytes=sizeof(data_send); // I'm counting the number of bytes of my message
  
  successful_packet = false;
//TRANSMIT MESSAGE -- begin
  while (!successful_packet){
  
  if (rf22.sendtoWait(data_send, sizeof(data_send), DESTINATION_ADDRESS_1) != RF22_ROUTER_ERROR_NONE) // I'm sending the data in variable data_send to DESTINATION_ADDRESS_1... cross fingers
  {
    Serial.println("sendtoWait failed"); // for some reason I have failed
    randNumber=random(200,max_delay);
    delay(randNumber);
  }
  else
  {
  successful_packet = true;
    Serial.println("sendtoWait Successful"); // I have received an acknowledgement from DESTINATION_ADDRESS_1. Data have been delivered!
	
  }
  // just demonstrating that the string I will send, after those transformation from integer to char and back remains the same
  }
//TRANSMIT MESSAGE -- end
  delay(15000); //wait for 15sec -> simple nodes take measurements and transmit them every 15 seconds
  Serial.println("-----------------------------");   
}
