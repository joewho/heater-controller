enum states {WELCOME, FLOOR, FLOOR_EDIT, CAB, CAB_EDIT, ROOM, ROOM_EDIT, MULTI};  // states for ZONE ID and the state machine/display 
enum Function {HEATING, COOLING};

#include "Sensor.h"
#include "ButtonController2.h"
#include "GUI.h"
//#include Driver
#include "Constants.h"


float floorTargetTemp = 88;
float floorTargetTempCool = -30;

float cabTargetTemp = 55;
float cabTargetTempCool = -30;

float roomTargetTemp = 60;
float roomTargetTempCool = -30;

const int threshold = 3;

unsigned long time_delay = 1000; //milliseconds of delay to check time
unsigned long last_time_check = 0; //time at instance
unsigned long current_time = 0;

//instantiate button controller
ButtonController2 buttonController;
//instantiate GUI
GUI userInterface;
//instantiate all temperature sensors
Sensor s10("inFloor", "TEMP", floorTempSensorPin); 
Sensor s11("inFloorPump", "TEMP", floorPumpTempSensorPin);
Sensor s20("Cabinet","TEMP", cabTempSensorPin);   
Sensor s21("CabinetPump","TEMP", cabPumpTempSensorPin);
Sensor s22("CabinetFin","TEMP", cabFinTempSensorPin);
Sensor s30("roomAir","TEMP", roomTempSensorPin);  
Sensor s31("roomAirPump","TEMP", roomPumpTempSensorPin);
Sensor s32("roomAirFin","TEMP", roomFinTempSensorPin);

//specific to floor
struct SensorMap{
  Sensor* sensor;
  float target;
  byte actuatorPin;
  byte greenLedPin;
  byte redLedPin;
  states zoneID;
  byte alarmID;
  Function function;
  bool actuatorOn;
  bool toggleActuator; //true if the actuator needs to be turned on or off to regulate the temperature
  bool zoneOn;
};

SensorMap inFloorTemp{sensor: &s10,
                  target: floorTargetTemp,
                  actuatorPin: floorPumpPin,
                  greenLedPin: floorGreenLedPin,
                  redLedPin: floorRedLedPin,
                  zoneID: FLOOR,
                  alarmID:-1,
                  function: HEATING,
                  actuatorOn: false,
                  toggleActuator: false,
                  zoneOn: true};

SensorMap inFLoorPumpTemp{};
                  
SensorMap cabinetTemp{sensor: &s20,
                  target: cabTargetTemp,
                  actuatorPin: cabPumpPin,
                  greenLedPin: cabGreenLedPin,
                  redLedPin: cabRedLedPin,
                  zoneID: CAB,
                  alarmID:-1,
                  function: HEATING,
                  actuatorOn: false,
                  toggleActuator: false,
                  zoneOn: true};

SensorMap cabinetPumpTemp{};

SensorMap cabinetFinTubeTemp{};

SensorMap roomAirTemp{sensor: &s30,
                  target: roomTargetTemp,
                  actuatorPin: roomPumpPin,
                  greenLedPin: roomGreenLedPin,
                  redLedPin: roomRedLedPin,
                  zoneID: ROOM,
                  alarmID:-1,
                  function: HEATING,
                  actuatorOn: false,
                  toggleActuator: false,
                  zoneOn: true};

SensorMap roomAirPumpTemp{};

SensorMap roomAirFinTubeTemp{};

                                    
SensorMap * mapArray[10] = {&inFloorTemp, &cabinetTemp, &roomAirTemp};
int mapArraySize = 3;

//get new values for each sensor in sensorMap
void getNewValues(){
  for(int i=0;i<mapArraySize;i++){
    if(mapArray[i]->function == HEATING){
     // if(mapArray[i]->zoneOn){
        if(mapArray[i]->actuatorOn) 
          mapArray[i]->sensor->read(1);
        else
          mapArray[i]->sensor->read(-1);
    //  }else
    //    mapArray[i]->sensor->read(-1);
    }else if(mapArray[i]->function == COOLING){
    //  if(mapArray[i]->zoneOn){
        if(mapArray[i]->actuatorOn)
          mapArray[i]->sensor->read(-1);
        else
          mapArray[i]->sensor->read(1);
      //}else
      //  mapArray[i]->sensor->read(1);
    }
      
  }//for loop
}

//send values to GUI unit
GUIOutput* sendValuesToGUI(){
  //for now print out values to serial
  SensorOutput sensorOutput[3];
  for(int i=0;i<mapArraySize;i++){
    sensorOutput[i].name = mapArray[i]->sensor->getName();
    sensorOutput[i].value = mapArray[i]->sensor->getValue();
    sensorOutput[i].target = mapArray[i]->target;
  }
    return userInterface.uploadUserInputs(sensorOutput, mapArraySize, buttonController.getButtonOutputs(), buttonController.arrayLength());
}

void compareValuesHeating(int i){//i is index of SensorMap in mapArray
 if(mapArray[i]->zoneOn){
   if(mapArray[i]->actuatorOn){
      if(mapArray[i]->sensor->getValue() >= (mapArray[i]->target + threshold))
        mapArray[i]->toggleActuator = true;
    }else if(mapArray[i]->sensor->getValue() <= (mapArray[i]->target - threshold))
        mapArray[i]->toggleActuator = true;
  }
}
void compareValuesCooling(int i){
  if(mapArray[i]->zoneOn){
    if(mapArray[i]->actuatorOn){
      if(mapArray[i]->sensor->getValue() <= (mapArray[i]->target - threshold))
        mapArray[i]->toggleActuator = true;
    }else if(mapArray[i]->sensor->getValue() >= (mapArray[i]->target + threshold))
      mapArray[i]->toggleActuator = true;
  }
}

void compareValues(){
  
  for(int i=0;i<mapArraySize;i++){
    if(mapArray[i]->function == HEATING)
      compareValuesHeating(i);
     else
      compareValuesCooling(i);
  }
}
//send value to driver/switching unit
void sendToSwitcher(GUIOutput* g){
    /*              
  Serial.print("sendToSwitcher\n");
  for(int i=0;i<mapArraySize;i++){
    
  
    Serial.print("SensorMap["+(String)i+"] name: ");
    Serial.print(mapArray[i]->sensor->getName());
      Serial.print("  SensorMap["+(String)i+"] value: ");
    Serial.print(mapArray[i]->sensor->getValue());
    Serial.print("  SensorMap["+(String)i+"] target: ");
    Serial.print(mapArray[i]->target);
      Serial.print("  SensorMap["+(String)i+"] zoneID: ");
    Serial.print(mapArray[i]->zoneID);
      Serial.print("  SensorMap["+(String)i+"] actuatorOn: ");
    Serial.print(mapArray[i]->actuatorOn);
  Serial.print("  SensorMap["+(String)i+"] toggleActuator: ");
    Serial.print(mapArray[i]->toggleActuator);
    Serial.print("  SensorMap["+(String)i+"] zoneOn: ");
    Serial.println(mapArray[i]->zoneOn);
  }
                   
  Serial.print("index: "+(String)g->sensorIndex+" ");
  Serial.print("update: "+(String)g->needToUpdate+" ");
  Serial.print("target: "+(String)g->newTarget+" ");
  Serial.println("toggle: "+(String)g->toggleOnOff);
  */
  for(int i=0;i<mapArraySize;i++){
    if(!mapArray[i]->zoneOn){
      mapArray[i]->actuatorOn = false;
      mapArray[i]->toggleActuator = false;
    }

    if(mapArray[i]->toggleActuator){
      mapArray[i]->actuatorOn = !mapArray[i]->actuatorOn;
      
      digitalWrite(mapArray[i]->actuatorPin,mapArray[i]->actuatorOn);
      //toggle leds
      digitalWrite(mapArray[i]->greenLedPin,mapArray[i]->actuatorOn);
      digitalWrite(mapArray[i]->redLedPin,!mapArray[i]->actuatorOn);
      mapArray[i]->toggleActuator = false;  
    }
    
     if(g->needToUpdate){
      if(!g->toggleOnOff)
        mapArray[g->sensorIndex]->target = g->newTarget;
      else{
        //toggleZoneOff
        mapArray[g->sensorIndex]->zoneOn = !mapArray[g->sensorIndex]->zoneOn;
        //toggleActuator
        mapArray[g->sensorIndex]->actuatorOn = !mapArray[g->sensorIndex]->actuatorOn;
        digitalWrite(mapArray[g->sensorIndex]->actuatorPin,mapArray[g->sensorIndex]->actuatorOn);
        digitalWrite(mapArray[g->sensorIndex]->greenLedPin,mapArray[g->sensorIndex]->actuatorOn);
        digitalWrite(mapArray[g->sensorIndex]->redLedPin,!mapArray[g->sensorIndex]->actuatorOn);
        mapArray[g->sensorIndex]->toggleActuator = false;  
      }//if(toggleOnOff)
      
    }//if(needToUpdate
  }

  
}//sendToSwitcher()

bool firstRun;
GUIOutput* guiOutput;

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(floorRedLedPin,OUTPUT);
  pinMode(floorGreenLedPin,OUTPUT);
  

  pinMode(cabRedLedPin,OUTPUT);
  pinMode(cabGreenLedPin,OUTPUT);
  
  pinMode(roomRedLedPin,OUTPUT);
  pinMode(roomGreenLedPin,OUTPUT);

   s10.setValue(floorTargetTemp);
   s20.setValue(cabTargetTemp);
   s30.setValue(roomTargetTemp);
   time_delay = 1500;


    buttonController.addButton("menu",button1Pin);
    buttonController.addButton("onOff",button2Pin);
    buttonController.addButton("up",button3Pin);
    buttonController.addButton("down",button4Pin);
  
  buttonController.beginSequence();
    lcd.print("Hello World!");
//    getNewValues();
}
void loop() {

  buttonController.listening();
  //Serial.print(buttonController.toStringPretty());
  
  guiOutput = sendValuesToGUI();
  compareValues();
  sendToSwitcher(guiOutput);

    current_time = millis();
  if((current_time - last_time_check)>= time_delay){
    last_time_check = current_time;
    getNewValues();
  }
  
}
