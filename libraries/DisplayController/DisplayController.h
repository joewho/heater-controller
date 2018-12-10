//
//  DisplayController.h
//  
//
//  Created by Joseph Cutro on 12/8/18.
//

#ifndef DisplayController_h
#define DisplayController_h
#include <LiquidCrystal.h>

const int rs=52,en=50,d4=51,d5=48,d6=49,d7=46;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

class DisplayController{
private:
public:
    DisplayController(){}
    
    void updateSensorDisplay(String);
    void changeDisplay(String, String);
};

#endif /* DisplayController_h */
//updates just line of display that shows the temp
void DisplayController::updateSensorDisplay(String line2){
    lcd.setCursor(0,1);
    lcd.print(line2);
}
//change whole screen of display
void DisplayController::changeDisplay(String line1, String line2){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(line1);
    lcd.setCursor(0,1);
    lcd.print(line2);
}
