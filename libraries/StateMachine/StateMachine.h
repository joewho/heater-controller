//
//  StateMachine.h
//  
//
//  Created by Joseph Cutro on 12/2/18.
//

#ifndef StateMachine_h
#define StateMachine_h

void checkStateChange(){
    switch(CURRENT_STATE){
        case WELCOME:
            if(menuButton.wasReleased()){
                Serial.println("menu was pressed WELCOME->FLOOR");
                CURRENT_STATE = FLOOR;
            }else if(upButton.wasReleased()){
                Serial.println("up was pressed WELCOME");
            }else if(downButton.wasReleased()){
                Serial.println("down was pressed WELCOME");
            }else if(warningButton.wasReleased()){
                Serial.println("warning was pressed WELCOME");
            }
            break;
        case FLOOR:
            if(menuButton.wasReleased()){
                Serial.println("menu was released FLOOR->CAB");
                CURRENT_STATE = CAB;
            }else if(upButton.wasReleased()){
                Serial.println("up was released FLOOR");
                floor_monitor.IncTargetTemp();
            }else if(downButton.wasReleased()){
                Serial.println("down was released FLOOR");
                floor_monitor.DecTargetTemp();
            }else if(warningButton.wasReleased()){
                Serial.println("warning was released FLOOR");
            }
            break;
        case CAB:
            if(menuButton.wasReleased()){
                Serial.println("menu was released CAB->ROOM");
                CURRENT_STATE = ROOM;
            }else if(upButton.wasReleased()){
                Serial.println("up was released CAB");
                cabinet_monitor.IncTargetTemp();
            }else if(downButton.wasReleased()){
                Serial.println("down was released CAB");
                cabinet_monitor.DecTargetTemp();
            }else if(warningButton.wasReleased()){
                Serial.println("warning was released CAB");
            }
            break;
        case ROOM:
            if(menuButton.wasReleased()){
                Serial.println("menu was released ROOM->MULTI");
                CURRENT_STATE = MULTI;
            }else if(upButton.wasReleased()){
                Serial.println("up was released ROOM");
                room_monitor.IncTargetTemp();
            }else if(downButton.wasReleased()){
                Serial.println("down was released ROOM");
                room_monitor.DecTargetTemp();
            }else if(warningButton.wasPressed()){
                Serial.println("warning was pressed ROOM");
            }
            break;
        case MULTI:
            if(menuButton.wasReleased()){
                Serial.println("menu was released MULTI->FLOOR");
                CURRENT_STATE = FLOOR;
            }else if(upButton.wasReleased()){
                Serial.println("up was released MULTI");
            }else if(downButton.wasReleased()){
                Serial.println("down was released MULTI");
            }else if(warningButton.wasReleased()){
                Serial.println("warning was released MULTI");
            }
            break;
        }
}

#endif /* StateMachine_h */
