//
//  DisplayController.h
//  
//
//  Created by Joseph Cutro on 12/8/18.
//

#ifndef DisplayController_h
#define DisplayController_h
#include <LiquidCrystal.h>

int blinkCount =0;
bool blinkON = false;
unsigned long timeDelay = 200; //milliseconds of delay to check time
unsigned long lastTimeCheck = 0; //time at instance
unsigned long currentTime = 0;

const int rs=52,en=50,d4=51,d5=48,d6=49,d7=46;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

class DisplayController{
private:
public:
    DisplayController(){}
    
    void updateSensorDisplay(String);
    void updateAndBlink(String, String);
    void changeDisplay(String, String);
    
};

#endif /* DisplayController_h */
//updates just line of display that shows the temp
void DisplayController::updateSensorDisplay(String line2){
    lcd.setCursor(0,1);
    lcd.print(line2);
}

void DisplayController::updateAndBlink(String onString, String offString){
    blinkCount = 0;
    while(blinkCount <=2){
        currentTime = millis();
        if((currentTime - lastTimeCheck)>= timeDelay){
            blinkCount++;
            lcd.setCursor(0,1);
            lastTimeCheck = currentTime;
            if(blinkON == false){
                lcd.print(offString);
                blinkON = true;
            }else{
                lcd.print(onString);
                blinkON = false;
            }
        }
    }
}
//change whole screen of display
void DisplayController::changeDisplay(String line1, String line2){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(line1);
    lcd.setCursor(0,1);
    lcd.print(line2);
}
