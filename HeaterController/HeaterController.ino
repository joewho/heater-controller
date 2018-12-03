enum states {WELCOME, FLOOR, CAB, ROOM, MULTI};  // states for the state machine
static states CURRENT_STATE = WELCOME; // current state machine state
static states DISPLAYED_STATE = WELCOME;

enum sensorType {TEMP, FLOW, FLOAT};
enum interaction {CONTROL, READ};
#include "ZoneController.h"

void changeTargetTemp(states s,int diff){
  switch(s){
    case FLOOR:
      zone1Temp.setTarget(zone1Temp.getTarget()+diff);
      break;
    case CAB:
      zone2Temp.setTarget(zone2Temp.getTarget()+diff);
      break;
    case ROOM:
      zone3Temp.setTarget(zone3Temp.getTarget()+diff);
      break;
  }
}

void toggleZonePower(states s){
  switch(s){
     case FLOOR:
     if(zone1Temp.isOn())
        zone1Temp.isOn(false);//turn off power to zone
     else
        zone1Temp.isOn(true);//turn on power to zone
      break;
    case CAB:
     if(zone2Temp.isOn())
        zone2Temp.isOn(false);//turn off power to zone
     else
        zone2Temp.isOn(true);//turn on power to zone
      break;
    case ROOM:
     if(zone3Temp.isOn())
        zone3Temp.isOn(false);//turn off power to zone
     else
        zone3Temp.isOn(true);//turn on power to zone
      break;
  }
}

#include "ButtonController.h"

#include "StateMachine.h"
#include "LCDController.h"

const byte speaker_pin = 28;

unsigned long time_delay = 880; //milliseconds of delay to check time
unsigned long last_time_check = 0; //time at instance
unsigned long current_time = 0;

void turnOffHeat(int index){
  digitalWrite(sensorArr[index]->getPin(),LOW);
  sensorArr[index]->setCallingHeat(false);
}

void turnOnHeat(int index){
  digitalWrite(sensorArr[index]->getPin(),HIGH);
  sensorArr[index]->setCallingHeat(true);
}

void checkAllSensors(){
  for(int i=0;i<sensorArrLength;i++){
    sensorArr[i]->updateHandler();
    Serial.println(sensorArr[i]->toString());
    if(!sensorArr[i]->isOn()) turnOffHeat(i);
    else if (sensorArr[i]->isCallingHeat() && sensorArr[i]->stateChanged)
      turnOnHeat(i);
    else if (!sensorArr[i]->isCallingHeat() && sensorArr[i]->stateChanged) {
      turnOffHeat(i);
    }//else if
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("HELLO WORLD!");
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Heating Control");
  lcd.setCursor(0, 1);
  lcd.print("Simulation v1.0");
  
  setupButtons();
  setupZones();
}
void loop() {
  // put your main code here, to run repeatedly:

  buttonOutput *myOutput;
  myOutput = listenToAllButtons();
  
  current_time = millis();
  if((current_time - last_time_check)>= time_delay){
    last_time_check = current_time;
    checkAllSensors();
    updateLCD();
  }
  checkStateChange();
  

}
