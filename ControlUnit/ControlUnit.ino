
#include "Sensor.h"
#include "ButtonController2.h"
//#include GUI
//#include Driver
enum Function {HEATING, COOLING};

Button upButton(-1,25,false,false);
ButtonListener bc("upButton",&upButton,-1);

const byte floorTempSensorPin = -1;
const byte floorPumpPin = 11;//22;
const byte floorPumpTempSensorPin = -1;
const byte floorFlowSensorPin = -1;
const byte floorGreenLedPin = 11;//22;
const byte floorRedLedPin = 23;

const byte cabTempSensorPin = -1;
const byte cabPumpPin= 24;
const byte cabPumpTempSensorPin = -1;
const byte cabFinTempSensorPin = -1;
const byte cabFanPin = -1;
const byte cabFlowSensorPin = -1;
const byte cabGreenLedPin = 12;//24;
const byte cabRedLedPin = 25;

const byte roomTempSensorPin = -1;
const byte roomPumpPin = 26;
const byte roomPumpTempSensorPin = -1;
const byte roomFinTempSensorPin = -1;
const byte roomFanPin = -1;
const byte roomFlowSensorPin = -1;
const byte roomGreenLedPin = 13;//26;
const byte roomRedLedPin = 27;


const byte returnTempSensorPin = -1;
const byte supplyTempSensorPin = -1;


float floorTargetTemp = 88;
float floorTargetTempCool = -30;

float cabTargetTemp = 55;
float cabTargetTempCool = -30;

float roomTargetTemp = 60;
float roomTargetTempCool = -30;

const int threshold = 3;

int tempReading;

unsigned long time_delay = 1000; //milliseconds of delay to check time
unsigned long last_time_check = 0; //time at instance
unsigned long current_time = 0;

//instantiate all temperature sensors
Sensor s10("inFloor", "TEMP", floorTempSensorPin); 
Sensor s11("inFloorPump", "TEMP", floorPumpTempSensorPin);
Sensor s20("Cabinet","TEMP", cabTempSensorPin);   
Sensor s21("CabinetPump","TEMP", cabPumpTempSensorPin);
Sensor s22("CabinetFin","TEMP", cabFinTempSensorPin);
Sensor s30("roomAir","TEMP", roomTempSensorPin);  
Sensor s31("roomAirPump","TEMP", roomPumpTempSensorPin);
Sensor s32("roomAirFin","TEMP", roomFinTempSensorPin);

//instantiate all buttons with ButtonController
ButtonController2 inputHandler;


//specific to floor
struct SensorMap{
  Sensor* sensor;
  float target;
  byte actuatorPin;
  byte greenLedPin;
  byte redLedPin;
  byte zoneID;
  byte alarmID;
  Function function;
  bool actuatorOn;
  bool toggleActuator; //true if the actuator needs to be turned on or off to regulate the temperature
};

SensorMap inFloorTemp{sensor: &s10,
                  target: floorTargetTemp,
                  actuatorPin: floorPumpPin,
                  greenLedPin: floorGreenLedPin,
                  redLedPin: floorRedLedPin,
                  zoneID: -1,
                  alarmID:-1,
                  function: HEATING,
                  actuatorOn: false,
                  toggleActuator: false};

SensorMap inFLoorPumpTemp{};
                  
SensorMap cabinetTemp{sensor: &s20,
                  target: cabTargetTemp,
                  actuatorPin: cabPumpPin,
                  greenLedPin: cabGreenLedPin,
                  redLedPin: cabRedLedPin,
                  zoneID: -1,
                  alarmID:-1,
                  function: HEATING,
                  actuatorOn: false,
                  toggleActuator: false};

SensorMap cabinetPumpTemp{};

SensorMap cabinetFinTubeTemp{};

SensorMap roomAirTemp{sensor: &s30,
                  target: roomTargetTemp,
                  actuatorPin: roomPumpPin,
                  greenLedPin: roomGreenLedPin,
                  redLedPin: roomRedLedPin,
                  zoneID: -1,
                  alarmID:-1,
                  function: HEATING,
                  actuatorOn: false,
                  toggleActuator: false};

SensorMap roomAirPumpTemp{};

SensorMap roomAirFinTubeTemp{};

                                    
SensorMap * mapArray[10] = {&inFloorTemp, &cabinetTemp, &roomAirTemp};
int mapArraySize = 3;

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
        mapArray[i]->sensor->read(1);
    }
      
  }//for loop
}

//send values to GUI unit
void sendValuesToGUI(){
  //for now print out values to serial
  for(int i=0;i<mapArraySize;i++){
  //  Serial.print(mapArray[i]->sensor->getName()+" ");
  //  Serial.print((String)mapArray[i]->sensor->getValue()+" ");
  //  Serial.print((String)mapArray[i]->target+"   ");
  }
  //Serial.println();
}

void compareValuesHeating(int i){//i is index of SensorMap in mapArray
 // Serial.print("compareHeating- value: "+(String)mapArray[i]->sensor->getValue());
//  Serial.print("  actuatorOn: "+(String)mapArray[i]->actuatorOn);
 // Serial.print("  toggleActuator: "+(String)mapArray[i]->toggleActuator);
  if(mapArray[i]->actuatorOn){
    if(mapArray[i]->sensor->getValue() >= (mapArray[i]->target + threshold))
      mapArray[i]->toggleActuator = true;
  }else if(mapArray[i]->sensor->getValue() <= (mapArray[i]->target - threshold)){
      mapArray[i]->toggleActuator = true;
    }
 
}
void compareValuesCooling(int i){
 //   Serial.print("compareCooling- value: "+(String)mapArray[i]->sensor->getValue());
//  Serial.print("  actuatorOn: "+(String)mapArray[i]->actuatorOn);
 // Serial.print("  toggleActuator: "+(String)mapArray[i]->toggleActuator);
  if(mapArray[i]->actuatorOn){
    if(mapArray[i]->sensor->getValue() <= (mapArray[i]->target - threshold))
      mapArray[i]->toggleActuator = true;
  }else if(mapArray[i]->sensor->getValue() >= (mapArray[i]->target + threshold))
    mapArray[i]->toggleActuator = true;
}

void compareValues(){
  
  for(int i=0;i<mapArraySize;i++){
      Serial.print("compareHeating- value: "+(String)mapArray[i]->sensor->getValue());
  Serial.print("  actuatorOn: "+(String)mapArray[i]->actuatorOn);
    if(mapArray[i]->function == HEATING)
      compareValuesHeating(i);
     else
      compareValuesCooling(i);
      Serial.print("         ");
  }
}
//send value to driver/switching unit
void sendToSwitcher(){
  for(int i=0;i<mapArraySize;i++){
    if(mapArray[i]->toggleActuator){
      mapArray[i]->actuatorOn = !mapArray[i]->actuatorOn;
      
      digitalWrite(mapArray[i]->actuatorPin,mapArray[i]->actuatorOn);
      //toggle leds
      digitalWrite(mapArray[i]->greenLedPin,mapArray[i]->actuatorOn);
      digitalWrite(mapArray[i]->redLedPin,!mapArray[i]->actuatorOn);
      mapArray[i]->toggleActuator = false;  
    }
  }//for loop
}//sendToSwitcher()

void setup() {
  Serial.begin(9600);
  pinMode(floorRedLedPin,OUTPUT);
  pinMode(floorGreenLedPin,OUTPUT);
  

  pinMode(cabRedLedPin,OUTPUT);
  pinMode(cabGreenLedPin,OUTPUT);
  
  pinMode(roomRedLedPin,OUTPUT);
  pinMode(roomGreenLedPin,OUTPUT);

   s10.setValue(floorTargetTemp);
   inFloorTemp.toggleActuator = true;
   s20.setValue(cabTargetTemp);
   cabinetTemp.toggleActuator = true;
   s30.setValue(roomTargetTemp);
   roomAirTemp.toggleActuator= true;
   time_delay = 1000;
   upButton.begin();
   //Serial.println("upButtonController-pin: "+(String)bc.getPin());
   Serial.println(bc.toString());
   inputHandler.addButton("LL",2);
    inputHandler.addButton("LR",3);
    inputHandler.addButton("RL",6);
    inputHandler.addButton("RR",7);
    pinMode(2,INPUT);
    pinMode(3,INPUT);
    pinMode(6,INPUT);
    pinMode(7,INPUT);
}
void loop() {
    current_time = millis();
  if((current_time - last_time_check)>= time_delay){
    last_time_check = current_time;
    
    getNewValues();
    //sendValuesToGUI();
    compareValues();
    sendToSwitcher();
    Serial.println();
  }
}
