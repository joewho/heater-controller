#include <OneWire.h>
#include <DallasTemperature.h>
 
// Data wire is plugged into pin 2 on the Arduino
#define busPin 2
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire bus(busPin);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&bus);
DeviceAddress sensor1;
DeviceAddress sensor2;
DeviceAddress sensor3;

byte address[8];

int timeDelay = 1000;
unsigned long lastTimeCheck = 0;
unsigned long currentTime = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sensors.begin();
    while(bus.search(address)){
    for(int i=0;i<7;i++){
      Serial.print(address[i], HEX);
      Serial.print(' ');
    }
    //checksum OK or fail
    if(OneWire::crc8(address, 7) == address[7])
      Serial.println(" CRC OK");
    else
      Serial.println(" CRC FAIL");
      
  }
  
  Serial.print("Number of devices... ");
  Serial.println(sensors.getDeviceCount());

  if(!sensors.getAddress(sensor2,1))
    Serial.println("DS18B20 NUMBER 2 NOT FOUND!");

  if(!sensors.getAddress(sensor3,2))
    Serial.println("DS18B20 NUMBER 3 NOT FOUND!");  
}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime = millis();
  if((currentTime - lastTimeCheck) >= timeDelay || lastTimeCheck == 0){
    lastTimeCheck = currentTime;
  sensors.requestTemperatures();
  float temp1F = sensors.getTempF(sensor1);
  float temp2F = sensors.getTempF(sensor2);
  float temp3F = sensors.getTempF(sensor3);
  Serial.print(temp1F);
  Serial.print(' ');
  Serial.print(temp2F);
  Serial.print(' ');
  Serial.print(temp3F);
  Serial.print(' ');
  Serial.println("end loop");
  }

  
}
