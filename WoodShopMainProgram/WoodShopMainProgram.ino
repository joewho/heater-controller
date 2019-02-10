#include "Constants.h"
#include "Structs.h"
//#include "Relay.h"
//#include "UserInputHandler.h"
#include "GUIHandler.h"
#include "RelayHandler.h"
#include "ButtonController2.h"
#include "HydronicSystemObject.h"
#include "LiquidCrystal.h"

//Object to manage input from multiple buttons
ButtonController2 buttonController;
ButtonMessage* buttonMessage;

HydronicSystemObject hydroHeater;
HydronicSystemMessage hydroBabel;
HydronicDisplayData hydroDisplayData[3];
HydronicDisplayData hydroSettingsChanges;

GUIHandler gui;

RelayHandler relayHandler;
RelayMessage* relayMessages;
RelayMessage relaySettingsChanges;

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

unsigned long demoTimeCheck = 0;
unsigned long demoTimeDelay = 750;

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

void printRelayData(RelayMessage tmp){
  Serial.println(tmp.name);
  Serial.println("Index: "+(String)tmp.index);
  Serial.println("Pin: "+(String)tmp.pin);
  Serial.println("PowerOn: "+(String)tmp.powerOn);
  Serial.println("isInverse: "+(String)tmp.isInverse);
  Serial.println("lastChanged: "+(String)tmp.lastChanged);
  Serial.println();
}

void printGUIHydroData(HydronicDisplayData tmp){
    Serial.println("GUI HYDRO DATA");
//      for(int i=0;i<hydroBabel.zoneCount;i++){
        //HydronicDisplayData tmp = hydroDisplayData[i];
        Serial.println(tmp.name);
        Serial.println("index: "+(String)tmp.arrayIndex);
        Serial.println("current "+(String)tmp.currentTemp);
        Serial.println("target "+(String)tmp.targetTemp);
        Serial.println("low: "+(String)tmp.lowTemp);
        Serial.println("high: "+(String)tmp.highTemp);
        Serial.println("zone Control: "+(String)tmp.zoneControlOn);
        Serial.println("water flow: "+(String)tmp.waterFlowOpen);
        Serial.println("alarm state: "+(String)tmp.alarmState);
        Serial.println("alarm message: "+tmp.alarmMessage);
        Serial.println();
  //}
  Serial.println();
}

void prepGUIHydroData(){
    HydronicDisplayData tmp;
    
    for(int i=0;i<hydroBabel.zoneCount;i++){
      tmp.arrayIndex = i;
      tmp.name = hydroBabel.zoneGroups[i]->name;
      tmp.currentTemp = hydroBabel.zoneGroups[i]->zoneTemp.getValue();
      tmp.targetTemp = hydroBabel.zoneGroups[i]->targetTemp;
      tmp.lowTemp = hydroBabel.zoneGroups[i]->lowTemp;
      tmp.highTemp = hydroBabel.zoneGroups[i]->highTemp;
      tmp.zoneControlOn = hydroBabel.zoneGroups[i]->zoneControlOn;
      tmp.waterFlowOpen = hydroBabel.zoneGroups[i]->waterFlowOpen;
      tmp.alarmState = hydroBabel.zoneGroups[i]->alarmState;
      tmp.alarmMessage = hydroBabel.zoneGroups[i]->alarmMessage;
      hydroDisplayData[i] = tmp;
  }
}

void setup() {
  Serial.begin(9600);
  //lcd.begin(16,2);
  
  pinMode(battery1,OUTPUT);
  pinMode(battery2,OUTPUT);
  pinMode(battery3,OUTPUT);
  pinMode(battery4,OUTPUT);
  buttonController.initiate();
  relayHandler.initiate();
  relayMessages = relayHandler.getAllRelayStatus();
  hydroHeater.initiate();
  gui.initiate();
  relayMessages = relayHandler.getAllRelayStatus();
  gui.preloadRelayData(relayMessages);
  //print relay data
  Serial.println("SETUP::RELAY_DATA-"); 
  for(int i=0;i<relayHandler.relayCount();i++){
    printRelayData(relayMessages[i]);
  }
  //relayHandler.initiate(); - set values of relays based on initialized hydroHeater values
  //gui.updateRelayData(relayHandler.getAllRelayStatus());
}

void loop() {
  //start listening for any button inputs
  buttonController.listening();
   //Serial.print(buttonController.toStringPretty());
    //print button name & action on lcd when pressed
    //buttonOutput = buttonController.getButtonOutputs();
    buttonMessage = buttonController.getButtonMessage();
  currentTime = millis();
  if((currentTime - demoTimeCheck)>= demoTimeDelay){
    demoTimeCheck = currentTime;
   gui.demoMode();
   relaySettingsChanges = gui.getRelayChanges();
   Serial.println("WoodShopMainProgram::demoMode()");
   printRelayData(relaySettingsChanges);
   relayHandler.editSettings(relaySettingsChanges);
   //update relayData
   gui.changeRelaySettings = false;
   gui.updateRelayData(relayHandler.getAllRelayStatus());
  }
    
    for(int i=0;i<4;i++){
      //if(buttonOutput[i].hasChanged){
      if(buttonMessage[i].hasChanged){
        //gui.updateButtonInput(buttonOutput[i].name,buttonOutput[i].action);
        gui.updateButtonInput(buttonMessage[i].name,buttonMessage[i].action);

        if(gui.changeHeaterSettings){
          //get updated settings from gui
          hydroSettingsChanges = gui.getHeaterChanges();
          //send updated settings to heater
          hydroHeater.editSettings(hydroSettingsChanges);
          gui.changeHeaterSettings = false;
        }
          
        if(gui.changeRelaySettings){
          //get updated data
          relaySettingsChanges = gui.getRelayChanges();
          Serial.println("WoodShopMainProgram::relayChanges()");
          printRelayData(relaySettingsChanges);
          relayHandler.editSettings(relaySettingsChanges);
          //update relayData
          gui.changeRelaySettings = false;
          gui.updateRelayData(relayHandler.getAllRelayStatus());

          }
          
      }
    }
   
    currentTime = millis();
  if((currentTime - lastTimeCheck)>= timeDelay){
    lastTimeCheck = currentTime;
  
    //tell heat system to update sensors and system values
    hydroHeater.updateSystem();
    hydroBabel = hydroHeater.getSystemStatus();
    //printBabel(hydroBabel);
    
    //display content
    prepGUIHydroData();
    //printGUIHydroData();
    gui.updateHydroData(hydroDisplayData);
    
    //edit output signal to relays based on acutator values
    for(int i=0;i<hydroBabel.zoneCount;i++){
      //check each actuator
      digitalWrite(hydroBabel.zoneGroups[i]->controlValve.getPin(),hydroBabel.zoneGroups[i]->controlValve.isPowerOn());
      //digitalWrite(hydroBabel.zoneGroups[i]->backflowValve.getPin(),hydroBabel.zoneGroups[i]->backflowValve.isPowerOn());
      //digitalWrite(hydroBabel.zoneGroups[i]->circFan.getPin(),hydroBabel.zoneGroups[i]->circFan.isPowerOn());
      //digitalWrite(hydroBabel.zoneGroups[i]->finFan.getPin(),hydroBabel.zoneGroups[i]->finFan.isPowerOn());
    }


  }
  //analogWrite(batteryBank[batteryCounter],27);
  digitalWrite(batteryBank[batteryCounter],HIGH);
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
