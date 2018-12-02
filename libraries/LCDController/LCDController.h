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

void updateStateChange();
void updateReadingChange();

void updateLCD(){
    if(DISPLAYED_STATE != CURRENT_STATE){
        updateStateChange();
        DISPLAYED_STATE = CURRENT_STATE;
    }else{
        updateReadingChange();
    }
}

void updateReadingChange(){
    switch(CURRENT_STATE){
        case WELCOME:
            break;
        case FLOOR:
            lcd.setCursor(0,1);
            lcd.print(floor_monitor.GetReadingTemp());
            lcd.print(" hold-"+(String)floor_monitor.GetTargetTemp());
            break;
        case CAB:
            lcd.setCursor(0,1);
            lcd.print(cabinet_monitor.GetReadingTemp());
            lcd.print(" hold-"+(String)cabinet_monitor.GetTargetTemp());
            break;
        case ROOM:
            lcd.setCursor(0,1);
            lcd.print(room_monitor.GetReadingTemp());
            lcd.print(" hold-"+(String)room_monitor.GetTargetTemp());
            break;
        case MULTI:
            lcd.setCursor(0,1);
            lcd.print((String)floor_monitor.GetReadingTemp()+" "+
                      (String)cabinet_monitor.GetReadingTemp()+" "+
                      (String)room_monitor.GetReadingTemp());
            break;
    }
}

void updateStateChange(){
    switch(CURRENT_STATE){
        case WELCOME:
            break;
        case FLOOR:
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(floor_monitor.GetName());
            lcd.setCursor(0,1);
            lcd.print(floor_monitor.GetReadingTemp());
            lcd.print(" hold-"+(String)floor_monitor.GetTargetTemp());
            break;
        case CAB:
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(cabinet_monitor.GetName());
            lcd.setCursor(0,1);
            lcd.print(cabinet_monitor.GetReadingTemp());
            lcd.print(" hold-"+(String)cabinet_monitor.GetTargetTemp());
            break;
        case ROOM:
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(room_monitor.GetName());
            lcd.setCursor(0,1);
            lcd.print(room_monitor.GetReadingTemp());
            lcd.print(" hold-"+(String)room_monitor.GetTargetTemp());
            break;
        case MULTI:
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Floor Cab   Room");
            lcd.setCursor(0,1);
            lcd.print((String)floor_monitor.GetReadingTemp()+" "+
                      (String)cabinet_monitor.GetReadingTemp()+" "+
                      (String)room_monitor.GetReadingTemp());
            break;
    }
}
#endif /* LCDController_h */
