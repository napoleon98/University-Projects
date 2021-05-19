#include "DHT.h"
#include <SPI.h>
#include <RF22.h>
#include <RF22Router.h>
#define HIGH_TEMP 23
#define HIGH_HUM 40
#define NUM_OF_NODES 3

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
int row = 1; //leei se poia grammh anoikei o master node
//RF STUFF
#define MY_ADDRESS 101 

#define DESTINATION_ADDRESS_1 1 

RF22Router rf22(MY_ADDRESS);

long randNumber;
boolean successful_packet = false;
int max_delay=3000;

DHT dht(DHTPIN, DHTTYPE);

int rainPin = A0;
int orangeLED = 6; //vlavh se potistiko ths grammhs
int redLED = 7; //anavei otan entopisei upshlh thermokrasia
int blueLED = 8; //upshlh ugrasia
// you can adjust the threshold value
int thresholdValue = 600; //(Moisture threshold) -> apo 600 ews 1023 tha anoigoun potistiria
int extremeLowHum = 750; // apo 900 ews 1023 exoume thema sto potistiko
int wateringDone = 530; // apo 900 ews 1023 exoume thema sto potistiko
int needWater = 0; //0 h 1 analoga an prepei na anoiksei to potistiko h oxi

//Nodes

int nodeNum[NUM_OF_NODES];
int tNode[NUM_OF_NODES];
int hNode[NUM_OF_NODES];
int soilNode[NUM_OF_NODES];
int waterSystemDamage[NUM_OF_NODES];

long int received_value=0;


void setup() {
    Serial.begin(9600);
  
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
  
  pinMode(rainPin, INPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(orangeLED, OUTPUT);
  digitalWrite(blueLED, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(orangeLED, LOW);
  
  
  randomSeed(analogRead(A0));
  //DHT setupBEGIN
  dht.begin();
  //DHT setupEND
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

  nodeNum[0]=1;
  int whoSent = 0;
  int allSent = 1;
  received_value = 0;
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
  
  whoSent = ((int)from)%100;
  
  int i=0;
  int k=0;
  
  for(i=1;i<(NUM_OF_NODES+1);i++){
    
    if(whoSent == i){
      nodeNum[i-1]=1;
      soilNode[i-1] = received_value / 1000000;
      received_value = received_value % 1000000;
      tNode[i-1] = received_value/1000;
      received_value = received_value % 1000;
      hNode[i-1]=received_value;
    }
    }
  
  for(k=0;k<NUM_OF_NODES;k++){
    if(nodeNum[k]==0)allSent=0;   
    
  }
  
  if(allSent==1){
    Serial.println("--------------------------------------");
    process();
    
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
   tNode[0] = t;
   hNode[0] = h;
  
  soilNode[0] = analogRead(rainPin); //0 -> 1023 ... 1023 = 0% humidity
  Serial.print("Master node soil: ");
  Serial.println(soilNode[0]);
  
  //calculations
  int meanHum = 0;
  int meanTemp = 0;
  int meanSoil = 0;
  
  
  int l=0;
  int m=0;
  
  
  for(l=0;l<NUM_OF_NODES;l++){
    meanHum = meanHum + hNode[l];
    meanTemp = meanTemp + tNode[l];
    meanSoil = meanSoil + soilNode[l];
  }
  meanHum = meanHum / NUM_OF_NODES;
  meanTemp = meanTemp / NUM_OF_NODES;
  meanSoil = meanSoil / NUM_OF_NODES;
  
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
    Serial.print("mean temp: ");
  Serial.println(meanTemp);
  Serial.print("mean hum: ");
  Serial.println(meanHum);
  Serial.print("mean soil: ");
  Serial.println(meanSoil);
  if(meanTemp > HIGH_TEMP)digitalWrite(redLED, HIGH);
  if(meanTemp < HIGH_TEMP)digitalWrite(redLED, LOW);
  if(meanHum > HIGH_HUM)digitalWrite(blueLED, HIGH);
  if(meanTemp < HIGH_HUM)digitalWrite(blueLED, LOW);
  if(hasDamage==1)digitalWrite(orangeLED,HIGH);
  if(hasDamage==0)digitalWrite(orangeLED,LOW);
  
  //packetData + send
  char data_read[RF22_ROUTER_MAX_MESSAGE_LEN];
  uint8_t data_send[RF22_ROUTER_MAX_MESSAGE_LEN];
  memset(data_read, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  memset(data_send, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);    
  sprintf(data_read, "R%dW%dT%dH%dD%d", row,needWater,meanTemp,meanHum,hasDamage); // I'm copying the measurement sensorVal into variable data_read
  data_read[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0'; 
  memcpy(data_send, data_read, RF22_ROUTER_MAX_MESSAGE_LEN); // now I'm copying data_read to data_send
  //number_of_bytes=sizeof(data_send); // I'm counting the number of bytes of my message
  successful_packet = false;
  Serial.print("tha steilw: ");
  Serial.println(data_read);
  while (!successful_packet){ 
  
  if (rf22.sendtoWait(data_send, sizeof(data_send), DESTINATION_ADDRESS_1) != RF22_ROUTER_ERROR_NONE) // I'm sending the data in variable data_send to DESTINATION_ADDRESS_1... cross fingers
  {
    Serial.println("sendtoWait failed"); // for some reason I have failed
  randNumber=random(200,max_delay);
  delay(randNumber);
  }
  else
  {
    Serial.println("sendtoWait Successful"); // I have received an acknowledgement from DESTINATION_ADDRESS_1. Data have been delivered!
  successful_packet = true;
 }
  
  }
}
