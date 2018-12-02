//#include <JC_Button.h>
#include "ButtonController.hpp"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("HELLO WORLD!");
  startButtons();
}
void loop() {
  // put your main code here, to run repeatedly:

  buttonOutput *myOutput;
  myOutput = listenToAllButtons();
//  serialPrintButtonOutput(myOutput);
  

}
