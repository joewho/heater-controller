//
//  GUI.h
//  
//
//  Created by Joseph Cutro on 12/8/18.
//

#ifndef GUI_h
#define GUI_h
#include "DisplayController.h"

struct SensorOutput{
    String name;
    float value;
    float target;
};

struct GUIOutput{
    bool needToUpdate;
    int sensorIndex;
    float newTarget;
    bool toggleOnOff;
};

class GUI{
private:
    ButtonOutput* _buttonInputs;
    SensorOutput* _sensorInputs;
    GUIOutput _guiOutput;
    states _currentState;
    states _displayedState;
    void _processAndDisplay();
    DisplayController display;
public:
    GUI(){
        _currentState = WELCOME;
        _displayedState = WELCOME;
    }
    
    GUIOutput* uploadUserInputs(SensorOutput s[], int sensorLength, ButtonOutput b[], int buttonLength){
        _buttonInputs = b;
        _sensorInputs = s;
        _processAndDisplay();
        return &_guiOutput;
    }

    states getCurrentState(){return _currentState;}
    void setCurrentState(states s){_currentState = s;}
    
    states getDisplayedState(){return _displayedState;}
    void setDisplayedState(states s){_displayedState =s;}
    
};

#endif /* GUI_h */
void GUI::_processAndDisplay(){
    String s;
    _guiOutput = {false,-1,-888,false};
    switch(_currentState){
        case WELCOME:
            if(_buttonInputs[0].action == "wasReleased"){
                Serial.println("menu was pressed WELCOME->FLOOR");
                _currentState = FLOOR;
                s = (String)_sensorInputs[0].value + " hold-"+(String)_sensorInputs[0].target;
                display.changeDisplay(_sensorInputs[0].name,s);
                Serial.println(_sensorInputs[0].name);
                Serial.println(s);
                
            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up was released WELCOME");
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down was released WELCOME");
            }else if(_buttonInputs[3].action == "wasReleased"){
                Serial.println("warning was released WELCOME");
            }
            break;
        case FLOOR:
            //switch(_currentMode){
            display.updateSensorDisplay((String)_sensorInputs[0].value+ " hold-"+(String)_sensorInputs[0].target);
            
            _guiOutput.sensorIndex = 0;
            _guiOutput.newTarget = _sensorInputs[0].target;
            
            if(_buttonInputs[0].action == "wasReleased"){
                Serial.println("menu was pressed FLOOR->CAB");
                _currentState = CAB;
                s =(String) _sensorInputs[1].value + " hold-"+(String)_sensorInputs[1].target;
                display.changeDisplay(_sensorInputs[1].name,s);
                Serial.println(_sensorInputs[1].name);
                Serial.println(s);
                
            }else if(_buttonInputs[0].action == "pressedFor"){
                Serial.println("menu pressedFOR FLOOR->CAB");
                _currentState = FLOOR_TARGET;
                display.changeDisplay(_sensorInputs[0].name+" EDIT",
                                      (String)_sensorInputs[0].value+ " hold-"+(String)_sensorInputs[0].target);

            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up was released FLOOR");
                _guiOutput.needToUpdate = true;
                _guiOutput.newTarget++;
                
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down was released FLOOR");
                _guiOutput.needToUpdate = true;
                _guiOutput.newTarget--;
                
            }else if(_buttonInputs[3].action == "wasReleased"){
                Serial.println("warning was released FLOOR");
                _guiOutput.needToUpdate = true;
                _guiOutput.toggleOnOff = true;
            }
            break;
        case FLOOR_TARGET:
            if(_buttonInputs[0].action == "isPressed"){
                Serial.println("menu isPressed FLOOR_EDIT");

            }else if(_buttonInputs[0].action == "wasPressed"){
                Serial.println("menu wasPressed FLOOR_EDIT->FLOOR");
                _currentState = FLOOR;
                s = (String)_sensorInputs[0].value + " hold-"+(String)_sensorInputs[0].target;
                display.changeDisplay(_sensorInputs[0].name,s);
                
            }else if(_buttonInputs[0].action == "wasReleased"){
                Serial.println("menu wasReleased FLOOR_EDIT");
                
            }else if(_buttonInputs[0].action == "releasedFor"){
                Serial.println("menu releasedFor FLOOR_EDIT");
            }
            else if(_buttonInputs[0].action == "isReleased"){
                Serial.println("menu isReleased FLOOR_EDIT");
                lcd.noDisplay();
                delay(500);
                lcd.display();
            }
            else if(_buttonInputs[0].action == "pressedFor"){
                Serial.println("menu pressedFor FLOOR_EDIT");
            }
            break;
        case CAB:
            display.updateSensorDisplay((String)_sensorInputs[1].value+ " hold-"+(String)_sensorInputs[1].target);
            
            _guiOutput.sensorIndex = 1;
            _guiOutput.newTarget = _sensorInputs[1].target;
            
            if(_buttonInputs[0].action == "wasReleased"){
                Serial.println("menu was pressed CAB->ROOM");
                _currentState = ROOM;
                s = (String)_sensorInputs[2].value + " hold-"+(String)_sensorInputs[2].target;
                display.changeDisplay(_sensorInputs[2].name,s);
                Serial.println(_sensorInputs[2].name);
                Serial.println(s);
                
            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up was released CAB");
                _guiOutput.needToUpdate = true;
                _guiOutput.newTarget++;
                
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down was released CAB");
                _guiOutput.needToUpdate = true;
                _guiOutput.newTarget--;
            }else if(_buttonInputs[3].action == "wasReleased"){
                Serial.println("warning was released CAB");
                _guiOutput.needToUpdate = true;
                _guiOutput.toggleOnOff = true;
            }
            break;
        case ROOM:
            display.updateSensorDisplay((String)_sensorInputs[2].value+ " hold-"+(String)_sensorInputs[2].target);
            
            _guiOutput.sensorIndex = 2;
            _guiOutput.newTarget = _sensorInputs[2].target;
            
            if(_buttonInputs[0].action == "wasReleased"){
                Serial.println("menu was pressed ROOM->MULTI");
                _currentState = MULTI;
                String s;
                s = (String)_sensorInputs[0].value +" "+ (String)_sensorInputs[1].value +" "+ (String)_sensorInputs[2].value;
                display.changeDisplay("Floor Cab  Room",s);
                Serial.println("Floor Cab  Room");
                Serial.println(s);
                
            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up was released ROOM");
                _guiOutput.needToUpdate = true;
                _guiOutput.newTarget++;
                
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down was released ROOM");
                _guiOutput.needToUpdate = true;
                _guiOutput.newTarget--;
                
            }else if(_buttonInputs[3].action == "wasReleased"){
                Serial.println("warning was released ROOM");
                _guiOutput.needToUpdate = true;
                _guiOutput.toggleOnOff = true;
            }
            break;
        case MULTI:
            display.updateSensorDisplay((String)_sensorInputs[0].value +" "
                                        + (String)_sensorInputs[1].value +" "
                                        + (String)_sensorInputs[2].value);
            if(_buttonInputs[0].action == "wasReleased"){
                Serial.println("menu was pressed MULTI->FLOOR");
                _currentState = FLOOR;
                s = (String)_sensorInputs[0].value + " hold-"+(String)_sensorInputs[0].target;
                display.changeDisplay(_sensorInputs[0].name,s);
                Serial.println(_sensorInputs[0].name);
                Serial.println(s);
            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up was released MULTI");
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down was released MULTI");
            }else if(_buttonInputs[3].action == "wasReleased"){
                Serial.println("warning was released MULTI");
            }
            break;
    }//switch
    _displayedState = _currentState;
    //return _guiOutput;
 //   Serial.print("index: "+(String)_guiOutput.sensorIndex+" ");
 //   Serial.print("update: "+(String)_guiOutput.needToUpdate+" ");
 //   Serial.print("target: "+(String)_guiOutput.newTarget+" ");
  //  Serial.println("toggle: "+(String)_guiOutput.toggleOnOff);
}//getNewState()
