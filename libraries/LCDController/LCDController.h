//
//  Header.h
//  
//
//  Created by Joseph Cutro on 12/2/18.
//

#ifndef LCDController_h
#define LCDController_h
#include <LiquidCrystal.h>

LiquidCrystal lcd(52, 50, 48, 46, 44, 42);

//void updateStateChange();
//void updateReadingChange();

void updateLCD(){
    switch(CURRENT_STATE){
        case WELCOME:
            break;
        case FLOOR:
            if(DISPLAYED_STATE != CURRENT_STATE){
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print(zone1Temp.getName());
                if(!zone1Temp.isOn())
                    lcd.print("  OFF");
            }
            lcd.setCursor(0,1);
            lcd.print(zone1Temp.getReading());
            lcd.print(" hold-"+(String)zone1Temp.getTarget());
            break;
        case CAB:
            if(DISPLAYED_STATE != CURRENT_STATE){
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print(zone2Temp.getName());
                if(!zone2Temp.isOn())
                    lcd.print("  OFF");
            }
            lcd.setCursor(0,1);
            lcd.print(zone2Temp.getReading());
            lcd.print(" hold-"+(String)zone2Temp.getTarget());
            break;
        case ROOM:
            if(DISPLAYED_STATE != CURRENT_STATE){
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print(zone3Temp.getName());
                if(!zone3Temp.isOn())
                    lcd.print("  OFF");
            }
            lcd.setCursor(0,1);
            lcd.print(zone3Temp.getReading());
            lcd.print(" hold-"+(String)zone3Temp.getTarget());
            break;
        case MULTI:
            if(DISPLAYED_STATE != CURRENT_STATE){
                lcd.clear();
                lcd.setCursor(0, 0);
            }
            lcd.print("Floor Cab   Room");
            lcd.setCursor(0,1);
            lcd.print((String)zone1Temp.getReading()+" "+
                      (String)zone2Temp.getReading()+" "+
                      (String)zone3Temp.getReading());
            break;
    }
}

#endif /* LCDController_h */
