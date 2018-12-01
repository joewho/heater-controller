#include <JC_Button.h>   
const byte
    BUTTON_PIN(10),
    LED_PIN(6); 
Button myBtn(BUTTON_PIN,25,false,false); 

void setup() {
  // put your setup code here, to run once:
  myBtn.begin();
  pinMode(LED_PIN,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  static bool ledState;
  myBtn.read();

  if(myBtn.wasPressed()){
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}
