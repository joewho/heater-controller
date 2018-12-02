#include "ButtonController.hpp"

ButtonController bc;
const byte menu_button_pin = 32;
const byte warning_button_pin = 34;
const byte up_button_pin = 53;
const byte down_button_pin = 51;
const int LONG_PRESS = 1000;
const int MAX_BUFFER = 10;

void setup() {
  Serial.begin(9600);
  Serial.println("Hello World!");
    bc.addButton("menu",menu_button_pin);
//    bc.addButton("warning",warning_button_pin);
//    bc.addButton("up",up_button_pin);
//    bc.addButton("down",down_button_pin);
//    Serial.println((String)bc.getLength());
  Serial.println(bc.toString());
//  Serial.println(bc.getListener(2).toString());
  bc.start();
}
void loop() {
  //bc.listen();

 // for(int i =0;i<bc.getLength();i++){
    //Serial.print(bc.getListener(i).toStringSimple()+"   ");
 // }
  //Serial.println();
  // put your main code here, to run repeatedly:
 // for(auto i = 0;i <arrLength;i++){
    //Serial.print(myArr[i]);
  //}
}
