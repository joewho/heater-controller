//
//  GUI.h
//  
//
//  Created by Joseph Cutro on 12/8/18.
//

#ifndef GUI_h
#define GUI_h
#include "DisplayController.h"
enum states {WELCOME, FLOOR, CAB, ROOM, MULTI};  // states for the state machine
//static states CURRENT_STATE = WELCOME; // current state machine state
//static states DISPLAYED_STATE = WELCOME;
struct SensorOutput{
    String name;
    String value;
    String target;
};

class GUI{
private:
    ButtonOutput* _buttonInputs[10];
    SensorOutput* _sensorInputs[10];
    states _currentState;
    states _displayedState;
    void _getInputsUpdateDisplay();
    DisplayController display;
public:
    GUI(){_currentState = WELCOME; _displayedState = WELCOME;}
    
    //void getUserInputs(ButtonOutput * bo){_buttonInputs = bo; _getInputsUpdateDisplay();}
    void getSensorInputs(SensorOutput * s){
        
    }
    
    states getCurrentState(){return _currentState;}
    void setCurrentState(states s){_currentState = s;}
    
    states getDisplayedState(){return _displayedState;}
    void setDisplayedState(states s){_displayedState =s;}
    
};

#endif /* GUI_h */
void GUI::_getInputsUpdateDisplay(){
    String s;
    switch(_currentState){
        case WELCOME:
            if(_buttonInputs[0]->action == "isPressed"){
                Serial.println("menu was pressed WELCOME->FLOOR");
                _currentState = FLOOR;
                s = _sensorInputs[0]->value + " hold-"+_sensorInputs[0]->target;
                display.changeDisplay(_sensorInputs[0]->name,s);
            }else if(_buttonInputs[1]->action == "wasReleased"){
                Serial.println("up was pressed WELCOME");
            }else if(_buttonInputs[2]->action == "wasReleased"){
                Serial.println("down was pressed WELCOME");
            }else if(_buttonInputs[3]->action == "wasReleased"){
                Serial.println("warning was pressed WELCOME");
            }
            break;
        case FLOOR:
            if(_buttonInputs[0]->action == "wasReleased"){
                Serial.println("menu was released FLOOR->CAB");
                _currentState = CAB;
                s = _sensorInputs[1]->value + " hold-"+_sensorInputs[1]->target;
                display.changeDisplay(_sensorInputs[1]->name,s);
            }else if(_buttonInputs[1]->action == "wasReleased"){
                Serial.println("up was released FLOOR");
//                changeTargetTemp(CURRENT_STATE,1);
            }else if(_buttonInputs[2]->action == "wasReleased"){
                Serial.println("down was released FLOOR");
//                changeTargetTemp(CURRENT_STATE,-1);
            }else if(_buttonInputs[3]->action == "wasReleased"){
                Serial.println("warning was released FLOOR");
//                toggleZonePower(CURRENT_STATE);
            }
            break;
        case CAB:
            if(_buttonInputs[0]->action == "wasReleased"){
                Serial.println("menu was released CAB->ROOM");
                _currentState = ROOM;
                s = _sensorInputs[2]->value + " hold-"+_sensorInputs[2]->target;
                display.changeDisplay(_sensorInputs[2]->name,s);
            }else if(_buttonInputs[1]->action == "wasReleased"){
                Serial.println("up was released CAB");
//                changeTargetTemp(CURRENT_STATE,1);
            }else if(_buttonInputs[2]->action == "wasReleased"){
                Serial.println("down was released CAB");
//                changeTargetTemp(CURRENT_STATE,-1);
            }else if(_buttonInputs[3]->action == "wasReleased"){
                Serial.println("warning was released CAB");
//                toggleZonePower(CURRENT_STATE);
            }
            break;
        case ROOM:
            if(_buttonInputs[0]->action == "wasReleased"){
                Serial.println("menu was released ROOM->MULTI");
                _currentState = MULTI;
                String s;
                s = _sensorInputs[0]->value +" "+ _sensorInputs[1]->value +" "+ _sensorInputs[2]->value;
                display.changeDisplay("Floor Cab  Room",s);
            }else if(_buttonInputs[1]->action == "wasReleased"){
                Serial.println("up was released ROOM");
//                changeTargetTemp(CURRENT_STATE,1);
            }else if(_buttonInputs[2]->action == "wasReleased"){
                Serial.println("down was released ROOM");
//                changeTargetTemp(CURRENT_STATE,-1);
            }else if(_buttonInputs[3]->action == "wasPressed"){
                Serial.println("warning was pressed ROOM");
//                toggleZonePower(CURRENT_STATE);
            }
            break;
        case MULTI:
            if(_buttonInputs[0]->action == "wasReleased"){
                Serial.println("menu was released MULTI->FLOOR");
                _currentState = FLOOR;
                s = _sensorInputs[0]->value + " hold-"+_sensorInputs[0]->target;
                display.changeDisplay(_sensorInputs[0]->name,s);
            }else if(_buttonInputs[1]->action == "wasReleased"){
                Serial.println("up was released MULTI");
            }else if(_buttonInputs[2]->action == "wasReleased"){
                Serial.println("down was released MULTI");
            }else if(_buttonInputs[3]->action == "wasReleased"){
                Serial.println("warning was released MULTI");
            }
            break;
    }//switch
}//getNewState()
