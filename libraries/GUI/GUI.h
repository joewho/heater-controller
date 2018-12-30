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
    float upperLimit;
    float lowerLimit;
};

struct GUIOutput{
    bool needToUpdate;
    int sensorIndex;
    float newTarget;
    float newLowerLimit;
    float newUpperLimit;
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
    _guiOutput = {false,-1,-150,-150,-150,false};

    switch(_currentState){
        case WELCOME:
            if(_buttonInputs[1].action == "isPressed" && _buttonInputs[2].action == "isPressed"){
                Serial.println("BOTH BUTTONS PRESED");
                //_currentState = RELAY_1;
                display.changeDisplay("RELAY_1","OFF");
            }
            else if(_buttonInputs[0].action == "wasReleased"){
                Serial.println("menu was pressed WELCOME->FLOOR");
                _currentState = FLOOR;
                s = (String)_sensorInputs[0].value + " hold-"+(String)_sensorInputs[0].target;
                display.changeDisplay(_sensorInputs[0].name,s);
                _displayedState = FLOOR;
                
                Serial.println(_sensorInputs[0].name);
                Serial.println(s);
                Serial.println("_displayedState: "+(String)_displayedState);
                Serial.println("_currentState: "+(String)_currentState);
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
            _guiOutput.newLowerLimit = _sensorInputs[0].lowerLimit;
            _guiOutput.newUpperLimit = _sensorInputs[0].upperLimit;
            
                    if(_buttonInputs[0].action == "wasReleased"){
                        Serial.println("_currentState = "+(String)_currentState);
                        Serial.println("_displayedState = " +(String)_displayedState);
                        if(_displayedState != FLOOR){
                            s = (String)_sensorInputs[0].value + " hold-"+(String)_sensorInputs[0].target;
                            display.changeDisplay(_sensorInputs[0].name,s);
                            _displayedState = FLOOR;
                            
                        }else if(_displayedState == _currentState){
                            Serial.println("menu was pressed FLOOR->CAB");
                            _currentState = CAB;
                            s =(String) _sensorInputs[1].value + " hold-"+(String)_sensorInputs[1].target;
                            display.changeDisplay(_sensorInputs[1].name,s);
                            _displayedState = CAB;
                        
                            Serial.println(_sensorInputs[1].name);
                            Serial.println(s);
                        }
                        
                        
                    }else if(_buttonInputs[0].action == "pressedFor"){
                        Serial.println("menu pressedFOR FLOOR->FLOOR_TARGET");
                        _currentState = FLOOR_TARGET;
                        display.changeDisplay(_sensorInputs[0].name+" TARGET",(String)_sensorInputs[0].target);
                        _displayedState = FLOOR_TARGET;

                    }else if(_buttonInputs[1].action == "wasReleased"){
                        Serial.println("up was released FLOOR");
                        
                    }else if(_buttonInputs[2].action == "wasReleased"){
                        Serial.println("down was released FLOOR");
                        
                    }else if(_buttonInputs[3].action == "wasReleased"){
                        Serial.println("warning was released FLOOR");
                        _guiOutput.needToUpdate = true;
                        _guiOutput.toggleOnOff = true;
                    }
                
            
            break;
        case FLOOR_TARGET:
            display.updateSensorDisplay((String)_sensorInputs[0].target);
            
            _guiOutput.sensorIndex = 0;
            _guiOutput.newTarget = _sensorInputs[0].target;
            _guiOutput.newLowerLimit = _sensorInputs[0].lowerLimit;
            _guiOutput.newUpperLimit = _sensorInputs[0].upperLimit;
            
                if(_buttonInputs[0].action == "isPressed"){
                    Serial.println("menu isPressed FLOOR_TARGET");

                }else if(_buttonInputs[0].action == "wasPressed"){
                    Serial.println("menu wasPressed FLOOR_TARGET->FLOOR_LOWER");
                    _currentState = FLOOR_LOWER;
                    display.changeDisplay(_sensorInputs[0].name+"THRESHOLD",
                                          (String)_sensorInputs[0].lowerLimit+ "->"+(String)_sensorInputs[0].upperLimit);
                    _displayedState = FLOOR_LOWER;
                    
                }else if(_buttonInputs[1].action == "wasReleased"){
                    Serial.println("up wasReleased FLOOR_TARGET");
                    _guiOutput.needToUpdate = true;
                    _guiOutput.newTarget++;
                    
                }else if(_buttonInputs[2].action == "wasReleased"){
                    Serial.println("down wasReleased FLOOR_TARGET");
                    _guiOutput.needToUpdate = true;
                    _guiOutput.newTarget--;
                }
           break;
            
        case FLOOR_LOWER:
            display.updateSensorDisplay((String)_sensorInputs[0].lowerLimit+ "->"+(String)_sensorInputs[0].upperLimit);
            
            _guiOutput.sensorIndex = 0;
            _guiOutput.newTarget = _sensorInputs[0].target;
            _guiOutput.newLowerLimit = _sensorInputs[0].lowerLimit;
            _guiOutput.newUpperLimit = _sensorInputs[0].upperLimit;
            
            if(_buttonInputs[0].action == "isPressed"){
                Serial.println("menu isPressed FLOOR_LOWER");
                
            }else if(_buttonInputs[0].action == "wasPressed"){
                Serial.println("menu wasPressed FLOOR_LOWER->FLOOR_UPPER");
                _currentState = FLOOR_UPPER;
                display.changeDisplay(_sensorInputs[0].name+"THRESHOLD",
                                      (String)_sensorInputs[0].lowerLimit+ "->"+(String)_sensorInputs[0].upperLimit);
                _displayedState = FLOOR_UPPER;
                
            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up wasReleased FLOOR_LOWER");
                _guiOutput.needToUpdate = true;
                _guiOutput.newLowerLimit++;
                
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down releasedFor FLOOR_LOWER");
                _guiOutput.needToUpdate = true;
                _guiOutput.newLowerLimit--;
            }
            else if(_buttonInputs[0].action == "isReleased"){
                Serial.println("menu isReleased FLOOR_LOWER");
                display.updateAndBlink((String)_sensorInputs[0].lowerLimit+ "->"+(String)_sensorInputs[0].upperLimit,
                                       "     ->"+(String)_sensorInputs[0].upperLimit);
            }
            else if(_buttonInputs[0].action == "pressedFor"){
                Serial.println("menu pressedFor FLOOR_LOWER");
            }
            
            break;
        case FLOOR_UPPER:
            display.updateSensorDisplay((String)_sensorInputs[0].lowerLimit+ "->"+(String)_sensorInputs[0].upperLimit);
            
            _guiOutput.sensorIndex = 0;
            _guiOutput.newTarget = _sensorInputs[0].target;
            _guiOutput.newLowerLimit = _sensorInputs[0].lowerLimit;
            _guiOutput.newUpperLimit = _sensorInputs[0].upperLimit;
            
            if(_buttonInputs[0].action == "isPressed"){
                Serial.println("menu isPressed FLOOR_UPPER");
                
            }else if(_buttonInputs[0].action == "wasPressed"){
                Serial.println("menu wasPressed FLOOR_UPPER->FLOOR");
                _currentState = FLOOR;
                s = (String)_sensorInputs[0].value + " hold-"+(String)_sensorInputs[0].target;
                display.changeDisplay(_sensorInputs[0].name,s);
                
            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up wasReleased FLOOR_UPPER");
                _guiOutput.needToUpdate = true;
                _guiOutput.newUpperLimit++;
                
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down releasedFor FLOOR_UPPER");
                _guiOutput.needToUpdate = true;
                _guiOutput.newUpperLimit--;
                
            }else if(_buttonInputs[0].action == "isReleased"){
                Serial.println("menu isReleased FLOOR_UPPER");
                
                display.updateAndBlink((String)_sensorInputs[0].lowerLimit+ "->"+             (String)_sensorInputs[0].upperLimit,
                                       (String)_sensorInputs[0].lowerLimit+"->       ");
            }
            else if(_buttonInputs[0].action == "pressedFor"){
                Serial.println("menu pressedFor FLOOR_UPPER");
            }
            break;
        case CAB:
            display.updateSensorDisplay((String)_sensorInputs[1].value+ " hold-"+(String)_sensorInputs[1].target);
            
            _guiOutput.sensorIndex = 1;
            _guiOutput.newTarget = _sensorInputs[1].target;
            
            if(_buttonInputs[0].action == "wasReleased"){
                if(_displayedState != CAB){
                    s = (String)_sensorInputs[1].value + " hold-"+(String)_sensorInputs[1].target;
                    display.changeDisplay(_sensorInputs[1].name,s);
                    _displayedState = CAB;
                    
                }else if(_displayedState == _currentState){
                    Serial.println("menu was pressed CAB->ROOM");
                    _currentState = ROOM;
                    s =(String) _sensorInputs[2].value + " hold-"+(String)_sensorInputs[2].target;
                    display.changeDisplay(_sensorInputs[2].name,s);
                    _displayedState = ROOM;
                    
                    Serial.println(_sensorInputs[1].name);
                    Serial.println(s);
                }
            }else if(_buttonInputs[0].action == "pressedFor"){
                Serial.println("menu pressedFOR CAB->CAB_TARGET");
                _currentState = CAB_TARGET;
                display.changeDisplay(_sensorInputs[1].name+" TARGET",(String)_sensorInputs[1].target);
                _displayedState = CAB_TARGET;
                
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down was released CAB");
                //_guiOutput.needToUpdate = true;
                //_guiOutput.newTarget--;
            }else if(_buttonInputs[3].action == "wasReleased"){
                Serial.println("warning was released CAB");
                _guiOutput.needToUpdate = true;
                _guiOutput.toggleOnOff = true;
            }
            break;
            
        case CAB_TARGET:
            display.updateSensorDisplay((String)_sensorInputs[1].target);
            
            _guiOutput.sensorIndex = 1;
            _guiOutput.newTarget = _sensorInputs[1].target;
            _guiOutput.newLowerLimit = _sensorInputs[1].lowerLimit;
            _guiOutput.newUpperLimit = _sensorInputs[1].upperLimit;
            
            if(_buttonInputs[0].action == "isPressed"){
                Serial.println("menu isPressed CAB_TARGET");
                
            }else if(_buttonInputs[0].action == "wasPressed"){
                Serial.println("menu wasPressed CAB_TARGET->CAB_LOWER");
                _currentState = CAB_LOWER;
                display.changeDisplay(_sensorInputs[1].name+"THRESHOLD",
                                      (String)_sensorInputs[1].lowerLimit+ "->"+(String)_sensorInputs[1].upperLimit);
                _displayedState = CAB_LOWER;
                
            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up wasReleased CAB_TARGET");
                _guiOutput.needToUpdate = true;
                _guiOutput.newTarget++;
                
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down wasReleased CAB_TARGET");
                _guiOutput.needToUpdate = true;
                _guiOutput.newTarget--;
            }
            break;
            
        case CAB_LOWER:
            display.updateSensorDisplay((String)_sensorInputs[1].lowerLimit+ "->"+(String)_sensorInputs[1].upperLimit);
            
            _guiOutput.sensorIndex = 1;
            _guiOutput.newTarget = _sensorInputs[1].target;
            _guiOutput.newLowerLimit = _sensorInputs[1].lowerLimit;
            _guiOutput.newUpperLimit = _sensorInputs[1].upperLimit;
            
            if(_buttonInputs[0].action == "isPressed"){
                Serial.println("menu isPressed CAB_LOWER");
                
            }else if(_buttonInputs[0].action == "wasPressed"){
                Serial.println("menu wasPressed CAB_LOWER->CAB_UPPER");
                _currentState = CAB_UPPER;
                display.changeDisplay(_sensorInputs[1].name+"THRESHOLD",
                                      (String)_sensorInputs[1].lowerLimit+ "->"+(String)_sensorInputs[1].upperLimit);
                _displayedState = CAB_UPPER;
                
            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up wasReleased CAB_LOWER");
                _guiOutput.needToUpdate = true;
                _guiOutput.newLowerLimit++;
                
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down releasedFor CAB_LOWER");
                _guiOutput.needToUpdate = true;
                _guiOutput.newLowerLimit--;
            }
            else if(_buttonInputs[0].action == "isReleased"){
                Serial.println("menu isReleased CAB_LOWER");
                display.updateAndBlink((String)_sensorInputs[1].lowerLimit+ "->"+(String)_sensorInputs[1].upperLimit,
                                       "     ->"+(String)_sensorInputs[1].upperLimit);
            }
            else if(_buttonInputs[0].action == "pressedFor"){
                Serial.println("menu pressedFor CAB_LOWER");
            }
            
            break;
        case CAB_UPPER:
            display.updateSensorDisplay((String)_sensorInputs[1].lowerLimit+ "->"+(String)_sensorInputs[1].upperLimit);
            
            _guiOutput.sensorIndex = 1;
            _guiOutput.newTarget = _sensorInputs[1].target;
            _guiOutput.newLowerLimit = _sensorInputs[1].lowerLimit;
            _guiOutput.newUpperLimit = _sensorInputs[1].upperLimit;
            
            if(_buttonInputs[0].action == "isPressed"){
                Serial.println("menu isPressed CAB_UPPER");
                
            }else if(_buttonInputs[0].action == "wasPressed"){
                Serial.println("menu wasPressed CAB_UPPER->CAB");
                _currentState = CAB;
                s = (String)_sensorInputs[1].value + " hold-"+(String)_sensorInputs[1].target;
                display.changeDisplay(_sensorInputs[1].name,s);
                
            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up wasReleased CAB_UPPER");
                _guiOutput.needToUpdate = true;
                _guiOutput.newUpperLimit++;
                
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down releasedFor CAB_UPPER");
                _guiOutput.needToUpdate = true;
                _guiOutput.newUpperLimit--;
                
            }else if(_buttonInputs[0].action == "isReleased"){
                Serial.println("menu isReleased CAB_UPPER");
                
                display.updateAndBlink((String)_sensorInputs[1].lowerLimit+ "->"+             (String)_sensorInputs[1].upperLimit,
                                       (String)_sensorInputs[1].lowerLimit+"->       ");
            }
            else if(_buttonInputs[0].action == "pressedFor"){
                Serial.println("menu pressedFor CAB_UPPER");
            }
            break;
        case ROOM:
            display.updateSensorDisplay((String)_sensorInputs[2].value+ " hold-"+(String)_sensorInputs[2].target);
            
            _guiOutput.sensorIndex = 2;
            _guiOutput.newTarget = _sensorInputs[2].target;
            _guiOutput.newLowerLimit = _sensorInputs[2].lowerLimit;
            _guiOutput.newUpperLimit = _sensorInputs[2].upperLimit;
            
            
            if(_buttonInputs[0].action == "wasReleased"){
                if(_displayedState != ROOM){
                    s = (String)_sensorInputs[2].value + " hold-"+(String)_sensorInputs[2].target;
                    display.changeDisplay(_sensorInputs[2].name,s);
                    _displayedState = ROOM;
                    
                }else if(_displayedState == _currentState){
                    Serial.println("menu was pressed ROOM->MULTI");
                    _currentState = MULTI;
                    String s;
                    s = (String)_sensorInputs[0].value +" "+ (String)_sensorInputs[1].value +" "+ (String)_sensorInputs[2].value;
                    display.changeDisplay("Floor Cab  Room",s);
                    _displayedState = MULTI;
                    Serial.println("Floor Cab  Room");
                    Serial.println(s);
                }

            }else if(_buttonInputs[0].action == "pressedFor"){
                Serial.println("menu pressedFOR ROOM->ROOM_TARGET");
                _currentState = ROOM_TARGET;
                display.changeDisplay(_sensorInputs[2].name+" TARGET",(String)_sensorInputs[2].target);
                _displayedState = ROOM_TARGET;
                
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down was released ROOM");
                //_guiOutput.needToUpdate = true;
                //_guiOutput.newTarget--;
                
            }else if(_buttonInputs[3].action == "wasReleased"){
                Serial.println("warning was released ROOM");
                _guiOutput.needToUpdate = true;
                _guiOutput.toggleOnOff = true;
            }
            break;
        case ROOM_TARGET:
            display.updateSensorDisplay((String)_sensorInputs[2].target);
            
            _guiOutput.sensorIndex = 2;
            _guiOutput.newTarget = _sensorInputs[2].target;
            _guiOutput.newLowerLimit = _sensorInputs[2].lowerLimit;
            _guiOutput.newUpperLimit = _sensorInputs[2].upperLimit;
            
            if(_buttonInputs[0].action == "isPressed"){
                Serial.println("menu isPressed ROOM_TARGET");
                
            }else if(_buttonInputs[0].action == "wasPressed"){
                Serial.println("menu wasPressed ROOM_TARGET->ROOM_LOWER");
                _currentState = ROOM_LOWER;
                display.changeDisplay(_sensorInputs[2].name+"THRESHOLD",
                                      (String)_sensorInputs[2].lowerLimit+ "->"+(String)_sensorInputs[2].upperLimit);
                _displayedState = ROOM_LOWER;
                
            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up wasReleased ROOM_TARGET");
                _guiOutput.needToUpdate = true;
                _guiOutput.newTarget++;
                
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down wasReleased ROOM_TARGET");
                _guiOutput.needToUpdate = true;
                _guiOutput.newTarget--;
            }
            break;
            
        case ROOM_LOWER:
            display.updateSensorDisplay((String)_sensorInputs[2].lowerLimit+ "->"+(String)_sensorInputs[2].upperLimit);
            
            _guiOutput.sensorIndex = 2;
            _guiOutput.newTarget = _sensorInputs[2].target;
            _guiOutput.newLowerLimit = _sensorInputs[2].lowerLimit;
            _guiOutput.newUpperLimit = _sensorInputs[2].upperLimit;
            
            if(_buttonInputs[0].action == "isPressed"){
                Serial.println("menu isPressed ROOM_LOWER");
                
            }else if(_buttonInputs[0].action == "wasPressed"){
                Serial.println("menu wasPressed ROOM_LOWER->ROOM_UPPER");
                _currentState = ROOM_UPPER;
                display.changeDisplay(_sensorInputs[2].name+"THRESHOLD",
                                      (String)_sensorInputs[2].lowerLimit+ "->"+(String)_sensorInputs[2].upperLimit);
                _displayedState = ROOM_UPPER;
                
            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up wasReleased ROOM_LOWER");
                _guiOutput.needToUpdate = true;
                _guiOutput.newLowerLimit++;
                
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down releasedFor ROOM_LOWER");
                _guiOutput.needToUpdate = true;
                _guiOutput.newLowerLimit--;
            }
            else if(_buttonInputs[0].action == "isReleased"){
                Serial.println("menu isReleased ROOM_LOWER");
                display.updateAndBlink((String)_sensorInputs[2].lowerLimit+ "->"+(String)_sensorInputs[2].upperLimit,
                                       "     ->"+(String)_sensorInputs[2].upperLimit);
            }
            else if(_buttonInputs[0].action == "pressedFor"){
                Serial.println("menu pressedFor ROOM_LOWER");
            }
            
            break;
        case ROOM_UPPER:
            display.updateSensorDisplay((String)_sensorInputs[2].lowerLimit+ "->"+(String)_sensorInputs[2].upperLimit);
            
            _guiOutput.sensorIndex = 2;
            _guiOutput.newTarget = _sensorInputs[2].target;
            _guiOutput.newLowerLimit = _sensorInputs[2].lowerLimit;
            _guiOutput.newUpperLimit = _sensorInputs[2].upperLimit;
            
            if(_buttonInputs[0].action == "isPressed"){
                Serial.println("menu isPressed ROOM_UPPER");
                
            }else if(_buttonInputs[0].action == "wasPressed"){
                Serial.println("menu wasPressed ROOM_UPPER->ROOM");
                _currentState = ROOM;
                s = (String)_sensorInputs[2].value + " hold-"+(String)_sensorInputs[2].target;
                display.changeDisplay(_sensorInputs[2].name,s);
                
            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up wasReleased ROOM_UPPER");
                _guiOutput.needToUpdate = true;
                _guiOutput.newUpperLimit++;
                
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down releasedFor ROOM_UPPER");
                _guiOutput.needToUpdate = true;
                _guiOutput.newUpperLimit--;
                
            }else if(_buttonInputs[0].action == "isReleased"){
                Serial.println("menu isReleased ROOM_UPPER");
                
                display.updateAndBlink((String)_sensorInputs[2].lowerLimit+ "->"+             (String)_sensorInputs[2].upperLimit,
                                       (String)_sensorInputs[2].lowerLimit+"->       ");
            }
            else if(_buttonInputs[0].action == "pressedFor"){
                Serial.println("menu pressedFor ROOM_UPPER");
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
                _displayedState = FLOOR;
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
//    /_displayedState = _currentState;
    
    //return _guiOutput;
 //   Serial.print("index: "+(String)_guiOutput.sensorIndex+" ");
 //   Serial.print("update: "+(String)_guiOutput.needToUpdate+" ");
 //   Serial.print("target: "+(String)_guiOutput.newTarget+" ");
  //  Serial.println("toggle: "+(String)_guiOutput.toggleOnOff);
}//getNewState()
