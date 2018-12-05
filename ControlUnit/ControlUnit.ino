#include "Sensor.h"
enum Function {HEATING, COOLING};

const byte floorTempSensorPin = 1;
const byte floorPumpPin = 3;
const byte floorLEDPin = 4;

const int threshold = 3;

float floorTargetTemp = 88;
float floorTargetTempCool = -30;

Sensor s1("Floor Temp", "TEMP", floorTempSensorPin);
Sensor s2("Floor Temp2","TEMP", floorTempSensorPin);
//specific to floor
struct SensorMap{
  Sensor* sensor;
  float target;
  byte actuatorPin;
  byte ledPin;
  byte zoneID;
  byte alarmID;
  Function function;
  bool actuatorOn;
  bool toggleActuator; //true if the actuator needs to be turned on or off to regulate the temperature
};

SensorMap concrete{sensor: &s1,
                  target: floorTargetTemp,
                  actuatorPin: floorPumpPin,
                  ledPin: floorLEDPin,
                  zoneID: -1,
                  alarmID:-1,
                  function: HEATING,
                  actuatorOn: false,
                  toggleActuator: false};
                  
SensorMap concrete2{sensor: &s2,
                  target: floorTargetTempCool,
                  actuatorPin: floorPumpPin,
                  ledPin: floorLEDPin,
                  zoneID: -1,
                  alarmID:-1,
                  function: COOLING,
                  actuatorOn: false,
                  toggleActuator: false};
SensorMap * mapArray[10] = {&concrete, &concrete2};
int mapArraySize = 2;

//get new values for each sensor in sensorMap
void getNewValues(){
  for(int i=0;i<mapArraySize;i++){
    if(mapArray[i]->function == HEATING){
      if(mapArray[i]->actuatorOn) 
        mapArray[i]->sensor->read(1);
      else
        mapArray[i]->sensor->read(-1);
    }else if(mapArray[i]->function == COOLING){
      if(mapArray[i]->actuatorOn)
        mapArray[i]->sensor->read(-1);
      else
        mapArray[i]->sensor->read(-1);
    }
      
  }//for loop
}

//send values to GUI unit
void sendValuesToGUI(){
  //for now print out values to serial
  for(int i=0;i<mapArraySize;i++){
    Serial.print(mapArray[i]->sensor->getName()+" ");
    Serial.print((String)mapArray[i]->sensor->getValue()+" ");
    Serial.print((String)mapArray[i]->target+"   ");
  }
  Serial.println();
}

void compareValuesHeating(){}
void compareValuesCooling(){}

void compareValues(){
  
  for(int i=0;i<mapArraySize;i++){
    if(mapArray[i]->function == HEATING)
      compareValuesHeating();
     else
      compareValuesCooling();
  }
}
//send value to driver/switching unit
void sendToSwitcher(){
  for(int i=0;i<mapArraySize;i++){
    if(mapArray[i]->toggleActuator){
      mapArray[i]->actuatorOn != mapArray[i]->actuatorOn;
      mapArray[i]->toggleActuator = false;  
    }
  }//for loop
}//sendToSwitcher()

void setup() {
  Serial.begin(9600);
  Serial.println("s1-value: "+(String)s1.getValue());
  s1.read(-1);
  Serial.println("s1-value: "+(String)s1.getValue());
  concrete.sensor->read(-1);
  Serial.println("s1-value: "+(String)s1.getValue());
}
void loop() {
  getNewValues();
  sendValuesToGUI();
//  compareValues();
//  sendToSwitcher();
/*  while(concrete.function == HEATING && concrete.sensor->getValue() < concrete.target){
    concrete.sensor->read(1);
  }
  */
}
