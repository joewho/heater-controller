#include "Constants.h"
#include "Structs.h"
//#include "Relay.h"
//#include "UserInputHandler.h"
#include "GUIHandler.h"
//#include "RelayHandler.h"
#include "ButtonController2.h"
#include "HydronicSystemObject.h"
#include "LiquidCrystal.h"

//Object to manage input from multiple buttons
ButtonController2 buttonController;
ButtonOutput* buttonOutput;
HydronicSystemObject hydroHeater;
HydronicSystemMessage hydroBabel;
GUIHandler gui;

const byte battery1 = 5;
const byte battery2 = 6;
const byte battery3 = 7;
const byte battery4 = 8;
const byte batteryCount = 4;
byte batteryCounter = 0;
const int  chargingTimer = 1000; //ms of time to induce charge on battery
unsigned long lastBatteryCheck = 0;
byte* batteryBank[] = {battery1,battery2,battery3,battery4};

unsigned long timeDelay = 1000; //milliseconds to use for delay reading of sensors
unsigned long lastTimeCheck = 0;
unsigned long currentTime = 0;

void printBabel(HydronicSystemMessage babel){
  Serial.println("PRINT BABEL");
  Serial.println("ZoneGroups");
  for(int i=0;i<babel.zoneCount;i++){
    Serial.println(babel.zoneGroups[i]->name);
    Serial.println("\ttargetTemp: "+(String)babel.zoneGroups[i]->targetTemp);
    Serial.println("\tlowTemp: "+(String)babel.zoneGroups[i]->lowTemp);
    Serial.println("\thighTemp: "+(String)babel.zoneGroups[i]->highTemp);
//    Serial.println("\talarmLowTemp: "+(String)babel.zoneGroups[i]->alarmLowTemp);
//    Serial.println("\talarmHighTemp: "+(String)babel.zoneGroups[i]->alarmHighTemp);
//    Serial.println("\tcontrolValvePin: "+(String)babel.zoneGroups[i]->controlValve.getPin());
    Serial.println("\tcontrolValvePowerOn: "+(String)babel.zoneGroups[i]->controlValve.isPowerOn());
//    Serial.println("\tbackflowValvePin: "+(String)babel.zoneGroups[i]->backflowValve.getPin());
//    Serial.println("\tbackflowValvePowerOn: "+(String)babel.zoneGroups[i]->backflowValve.isPowerOn());
//    Serial.println("\tcircFanPin: "+(String)babel.zoneGroups[i]->circFan.getPin());
//    Serial.println("\tcircFanPowerOn: "+(String)babel.zoneGroups[i]->circFan.isPowerOn());
//    Serial.println("\tfinFanPin: "+(String)babel.zoneGroups[i]->finFan.getPin());
//    Serial.println("\tfinFanPowerOn: "+(String)babel.zoneGroups[i]->finFan.isPowerOn());
//    Serial.println("\tfinTemp: "+(String)babel.zoneGroups[i]->finTemp.getValue());
//    Serial.println("\treturnWaterTemp: "+(String)babel.zoneGroups[i]->returnWaterTemp.getValue());
    Serial.println("\tzoneTemp: "+(String)babel.zoneGroups[i]->zoneTemp.getValue());
    Serial.println("\twaterFlowOpen: "+(String)babel.zoneGroups[i]->waterFlowOpen);
    Serial.println("\tzoneControlOn: "+(String)babel.zoneGroups[i]->zoneControlOn);
    Serial.println("\talarmState: "+(String)babel.zoneGroups[i]->alarmState);
    Serial.println("\talarmMessage: "+babel.zoneGroups[i]->alarmMessage);  
  }
//  Serial.println("zoneCount: "+(String)babel.zoneCount);
//  Serial.println("secondaryPumpOn: "+(String)babel.secondaryPump.isPowerOn());
//  Serial.println("secondaryBypassOpen: "+(String)babel.secondaryPumpBypassValve.isPowerOn());
//  Serial.println("systemRunning: "+(String)babel.systemRunning);
//  Serial.println("alarmState: "+(String)babel.alarmState);
//  Serial.println("alarmMessage: "+babel.alarmMessage);
  Serial.println("runningTime: "+(String)babel.runningTime);


}

void setup() {
  Serial.begin(9600);
  //lcd.begin(16,2);
  
  pinMode(battery1,OUTPUT);
  pinMode(battery2,OUTPUT);
  pinMode(battery3,OUTPUT);
  pinMode(battery4,OUTPUT);
  //create buttons and add to ButtonController
    buttonController.addButton("navButton",button1Pin);
    buttonController.addButton("selectButton",button2Pin);
    buttonController.addButton("upButton",button3Pin);
    buttonController.addButton("downButton",button4Pin);
  //prepare buttons for use
  buttonController.beginSequence();
  gui.initiate();
  hydroHeater.initiate();
  //relayHandler.initiate(); - set values of relays based on initialized hydroHeater values
  //displayHandler.initiate(); - initialized display and set to welcome screen
}

void loop() {
  //start listening for any button inputs
  buttonController.listening();
   //Serial.print(buttonController.toStringPretty());
    //print button name & action on lcd when pressed
    buttonOutput = buttonController.getButtonOutputs();
    
    for(int i=0;i<4;i++){
      if(buttonOutput[i].hasChanged){
        gui.updateButtonInput(buttonOutput[i].name,buttonOutput[i].action);
        /*
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(buttonOutput[i].name);
        lcd.print(" changed");
        lcd.setCursor(0,1);
        lcd.print(buttonOutput[i].lastChange);
        */
      }
      /*
      if(buttonOutput[i].action == "isPressed"){
      
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(buttonOutput[i].name);
        lcd.setCursor(0,1);
        lcd.print("is Pressed");
      }
      else if(buttonOutput[i].action == "pressedFor"){
        //lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(buttonOutput[i].name);
        lcd.setCursor(0,1);
        lcd.print("Long Press");
      }
      */
    }
   
    currentTime = millis();
  if((currentTime - lastTimeCheck)>= timeDelay){
    lastTimeCheck = currentTime;
  
    //tell heat system to update sensors and system values
    hydroHeater.updateSystem();
    hydroBabel = hydroHeater.getSystemStatus();
    //printBabel(hydroBabel);

    //edit output signal to relays based on acutator values
    for(int i=0;i<hydroBabel.zoneCount;i++){
      //check each actuator
      digitalWrite(hydroBabel.zoneGroups[i]->controlValve.getPin(),hydroBabel.zoneGroups[i]->controlValve.isPowerOn());
      digitalWrite(hydroBabel.zoneGroups[i]->backflowValve.getPin(),hydroBabel.zoneGroups[i]->backflowValve.isPowerOn());
      digitalWrite(hydroBabel.zoneGroups[i]->circFan.getPin(),hydroBabel.zoneGroups[i]->circFan.isPowerOn());
      digitalWrite(hydroBabel.zoneGroups[i]->finFan.getPin(),hydroBabel.zoneGroups[i]->finFan.isPowerOn());
    }

    //display content
    gui.updateHydroData();
  }
  analogWrite(batteryBank[batteryCounter],27);
  //digitalWrite(batteryBank[batteryCounter],HIGH);
  if((currentTime - lastBatteryCheck) >= chargingTimer){
    lastBatteryCheck = currentTime;
    digitalWrite(batteryBank[batteryCounter],LOW);
    //lcd.clear();
    //lcd.setCursor(0,0);
    batteryCounter++;
   
    if(batteryCounter >= batteryCount) batteryCounter=0;
    //lcd.print("battery ");
    //lcd.print(batteryCounter+1);
  }

  //update relays with hydroBabel input

  //update displays with hydroBabel input
  
}
