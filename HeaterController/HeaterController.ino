enum states {WELCOME, FLOOR, CAB, ROOM, MULTI};  // states for the state machine
static states CURRENT_STATE = WELCOME; // current state machine state
static states DISPLAYED_STATE = WELCOME;

enum sensorType {TEMP, FLOW, FLOAT};
enum interaction {CONTROL, READ};

#include "ButtonController.h"
#include "PumpController.h"
#include "StateMachine.h"
#include "LCDController.h"
//#include "SensorHandler.h"
//#include "ZoneController.h"
const byte speaker_pin = 28;

unsigned long time_delay = 880; //milliseconds of delay to check time
unsigned long last_time_check = 0; //time at instance
unsigned long current_time = 0;



String displayLine1;
String displayLine2;

void turnOffHeat(int index) {
//  Serial.println("Heat OFF");
  digitalWrite(monitor_arr[index]->GetPin(), LOW);
  monitor_arr[index]->SetCallingForHeat(false);
}

void turnOnHeat(int index) {
//  Serial.println("Heat ON");
  digitalWrite(monitor_arr[index]->GetPin(), HIGH);
  monitor_arr[index]->SetCallingForHeat(true);
}

void checkAllMonitors(){
  for (int i = 0; i < monitor_arr_length; i++) {
    monitor_arr[i]->Update();
 //   Serial.println(monitor_arr[i]->ToStringShort());
    if (monitor_arr[i]->IsCallingForHeat() && monitor_arr[i]->stateChanged)
      turnOnHeat(i);
    else if (!monitor_arr[i]->IsCallingForHeat() && monitor_arr[i]->stateChanged) {
      turnOffHeat(i);
    }//else if
  }//for
}//checkAllMonitors

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("HELLO WORLD2!");
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Heating Control");
  lcd.setCursor(0, 1);
  lcd.print("Simulation v1.0");
  
  setupButtons();
  setupPumps();
}
void loop() {
  // put your main code here, to run repeatedly:

  buttonOutput *myOutput;
  myOutput = listenToAllButtons();
  current_time = millis();
  if((current_time - last_time_check)>= time_delay){
    last_time_check = current_time;
    checkAllMonitors();
    updateLCD();
  }
  checkStateChange();
  

}
