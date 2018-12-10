#include <LiquidCrystal.h>
#include "ButtonController2.h"
LiquidCrystal lcd(12,11,5,4,3,2);
ButtonController2 buttonController;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
lcd.begin(16,2);
lcd.print("Hello World");
buttonController.addButton("LEFT",9);
buttonController.addButton("LEFT MIDDLE",10);
buttonController.addButton("RIGHT MIIDLE",7);
buttonController.addButton("RIGHT",8   );
buttonController.beginSequence();
Serial.println(buttonController.toString());

}

void loop() {
  // put your main code here, to run repeatedly:
buttonController.listening();
ButtonOutput * bo = buttonController.getButtonOutputs();
    for(int i=0;i<buttonController.arrayLength();i++){
        if(bo[i].action == "isPressed"){
          lcd.clear();
          lcd.setCursor(0,1);
          lcd.print(bo[i].name);      
        }
//      String *str = buttonController.getListener(i).toStringPretty();
//        Serial.print(str);
//      Serial.print(bo[i].name+" ");
//      Serial.print((String)bo[i].pin+" ");
//      Serial.print(bo[i].action+" ");
//      Serial.print((String)bo[i].lastChange+" ");
//      Serial.println((String)bo[i].hasChanged);
    }
    //Serial.println();
Serial.print(buttonController.toStringPretty());
    
//lcd.setCursor(0,1);
//lcd.print(millis()/1000);
}
