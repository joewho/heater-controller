#include "Constants.h"
#include "Structs.h"
//#include "Relay.h"
//#include "UserInputHandler.h"
//#include "DisplayHandler.h"
//#include "RelayHandler.h"
#include "ButtonController2.h"
#include "HydronicSystemObject.h"

//Object to manage input from multiple buttons
ButtonController2 buttonController;
HydronicSystemObject hydroHeater;
HydronicSystemMessage hydroBabel;

unsigned long timeDelay = 1000; //milliseconds to use for delay reading of sensors
unsigned long lastTimeCheck = 0;
unsigned long currentTime = 0;

void setup() {
  Serial.begin(9600);
  //create buttons and add to ButtonController
    buttonController.addButton("menu",button1Pin);
    buttonController.addButton("onOff",button2Pin);
    buttonController.addButton("up",button3Pin);
    buttonController.addButton("down",button4Pin);
  //prepare buttons for use
  buttonController.beginSequence();
  hydroHeater.initiate();
}

void loop() {
  //start listening for any button inputs
  buttonController.listening();
   //Serial.print(buttonController.toStringPretty());
   
    currentTime = millis();
  if((currentTime - lastTimeCheck)>= timeDelay){
    lastTimeCheck = currentTime;
  
    //tell heat system to update sensors and system values
    hydroHeater.updateSystem();
  }


  //retrieve new values from heat system
//  hydroBabel = hydroHeater.getSystemStatus();

  
  //update relays with hydroBabel input
  //update displays with hydroBabel input
  
}
