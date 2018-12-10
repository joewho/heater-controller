
#include "Sensor.h"
#include "ButtonController2.h"
#include "GUI.h"
//#include Driver
#include "Constants.h"
enum Function {HEATING, COOLING};

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
  SensorOutput sensorOutput[3];
  for(int i=0;i<mapArraySize;i++){
    sensorOutput[i].name = mapArray[i]->sensor->getName();
    sensorOutput[i].value = mapArray[i]->sensor->getValue();
    sensorOutput[i].target = mapArray[i]->target;
  }
  userInterface.uploadUserInputs(sensorOutput, mapArraySize, buttonController.getButtonOutputs(), buttonController.arrayLength());
}

void compareValuesHeating(int i){//i is index of SensorMap in mapArray
 if(mapArray[i]->actuatorOn){
    if(mapArray[i]->sensor->getValue() >= (mapArray[i]->target + threshold))
      mapArray[i]->toggleActuator = true;
  }else if(mapArray[i]->sensor->getValue() <= (mapArray[i]->target - threshold)){
      mapArray[i]->toggleActuator = true;
    }
 
}
void compareValuesCooling(int i){
  if(mapArray[i]->actuatorOn){
    if(mapArray[i]->sensor->getValue() <= (mapArray[i]->target - threshold))
      mapArray[i]->toggleActuator = true;
  }else if(mapArray[i]->sensor->getValue() >= (mapArray[i]->target + threshold))
    mapArray[i]->toggleActuator = true;
}

void compareValues(){
  
  for(int i=0;i<mapArraySize;i++){
//    Serial.print("compareHeating- value: "+(String)mapArray[i]->sensor->getValue());
//    Serial.print("  actuatorOn: "+(String)mapArray[i]->actuatorOn);
    if(mapArray[i]->function == HEATING)
      compareValuesHeating(i);
     else
      compareValuesCooling(i);
 //     Serial.print("         ");
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
bool firstRun;
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
   time_delay = 3000;

   //pinMode(button1Pin,INPUT);
    //pinMode(button2Pin,INPUT);
    //pinMode(button3Pin,INPUT);
    //pinMode(button4Pin,INPUT);
    buttonController.addButton("LL",button1Pin);
    //ButtonListener * listener = buttonController.getListener(0);
    //Serial.println(buttonController.getListener(0)->toString());
    buttonController.addButton("LR",button2Pin);
    buttonController.addButton("RL",button3Pin);
    buttonController.addButton("RR",button4Pin);
  //
  buttonController.beginSequence();
    Serial.println(buttonController.toString());
    //Serial.println(bo[0].name);
//    Serial.println(buttonController.toStringPretty());
  //  firstRun = true;
    Serial.println("FIRST RUN");
    
}
void loop() {
  buttonController.listening();
  Serial.print(buttonController.toStringPretty());
/*  
    ButtonOutput * bo = buttonController.getButtonOutputs();
    for(int i=0;i<buttonController.arrayLength();i++){
      Serial.print(bo[i].name+" ");
      Serial.print((String)bo[i].pin+" ");
      Serial.print(bo[i].action+" ");
      Serial.print((String)bo[i].lastChange+" ");
      Serial.println((String)bo[i].hasChanged);
    }
    Serial.println();
*/
    
    current_time = millis();
  if((current_time - last_time_check)>= time_delay){
    last_time_check = current_time;
    //    Serial.println(buttonController.toString());

    getNewValues();
    sendValuesToGUI();
    compareValues();
    sendToSwitcher();
    //Serial.println();
  }
}
