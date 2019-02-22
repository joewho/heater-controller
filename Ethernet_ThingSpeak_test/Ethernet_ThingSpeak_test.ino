#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

char thingSpeakAddress[] = "api.thingspeak.com";
String writeAPIkey = "1R2S1NL85TGT0X0P";
const int updateThingSpeakInterval = 25 * 1000;


long lastConnectionTime = 0;
bool lastConnected = false;
int failedCounter = 0;

int testCounter= 0;

EthernetClient client;


void setup() {
  Serial.begin(9600);
  
  Serial.print("Server started on: ");
  Serial.println(Ethernet.localIP());

  startEthernet();
}

void loop() {
  if(client.available()){
    char c = client.read();
    Serial.print(c);
  }

  //disconnect from thingspeak
  if(!client.connected() && lastConnected){
    Serial.println("...disconnected");
    Serial.println();

    client.stop();
  }

  //update thingspeak
  if(!client.connected() && (millis()- lastConnectionTime > updateThingSpeakInterval)){
    updateThingSpeak("field1="+(String)testCounter);
    testCounter++;
  }

  //check if arduino ethernet needs to be restarted
  if(failedCounter > 3 ) startEthernet();
  lastConnected = client.connected();
}

void updateThingSpeak(String tsData){
  if(client.connect(thingSpeakAddress,80)){
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+ writeAPIkey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");
    client.print(tsData);

    lastConnectionTime = millis();

    if(client.connected()){
      Serial.println("Connecting to ThingSpeak Server");
      Serial.println();

      failedCounter = 0;
    }else{
      failedCounter++;

      Serial.println("Connection to ThingSpeak failed ("+String(failedCounter, DEC)+")");
      Serial.println();
    }
  }else{
    failedCounter++;

    Serial.println("Connection to ThingSpeak Failed ("+String(failedCounter, DEC)+")");
    Serial.println();

    lastConnectionTime = millis();
  }
}

void startEthernet(){
  client.stop();

  Serial.println("Connecting to current network please wait...");
  Serial.println();

  delay(1000);

  //connect to network and obtain ip address using dhcp
  if(Ethernet.begin(mac) == 0){
    Serial.println("DHCP Failed, reset the device and try again");
    Serial.println();
  }else{
    Serial.println("Connected to network w/d HDCP");
    Serial.println();
  }
  delay(1000);
}
