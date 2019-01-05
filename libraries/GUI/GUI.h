//
//  GUI.h
//  
//
//  Created by Joseph Cutro on 12/8/18.
//

#ifndef GUI_h
#define GUI_h
#include "DisplayController.h"
const int multi_click_wait_time = 1000;
float multi_click_time_begin=0;

struct RelayMap{
    int relayPin;
    bool relayOn;
    bool toggleRelay;
};

struct SensorOutput{
    String name;
    float value;
    float target;
    float upperLimit;
    float lowerLimit;
};

struct GUIOutput{
    bool needToUpdate;
    bool isRelay;
    int relayIndex;
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
    RelayMap* _relays;
    GUIOutput _guiOutput;
    states _currentState;
    states _displayedState;
    void _processAndDisplay();
    bool _checkTimer();
    DisplayController display;
public:
    GUI(){
        _currentState = WELCOME;
        _displayedState = WELCOME;
    }
    
    GUIOutput* uploadUserInputs(SensorOutput s[], int sensorLength, ButtonOutput b[], int buttonLength, RelayMap r[]){
        _buttonInputs = b;
        _sensorInputs = s;
        _relays = r;
        _processAndDisplay();
        return &_guiOutput;
    }

    states getCurrentState(){return _currentState;}
    void setCurrentState(states s){_currentState = s;}
    
    states getDisplayedState(){return _displayedState;}
    void setDisplayedState(states s){_displayedState =s;}
    
};
bool GUI::_checkTimer(){
    if(millis() < multi_click_time_begin + multi_click_wait_time)
        return false;
    return true;
}
#endif /* GUI_h */
void GUI::_processAndDisplay(){
    String s;
    _guiOutput = {false,false,-1,-1,-150,-150,-150,false};

    switch(_currentState){
        case WELCOME:
            if(_buttonInputs[1].action == "isPressed" && _buttonInputs[2].action == "isPressed"){
                multi_click_time_begin = millis();
                Serial.println("BOTH BUTTONS PRESED WELCOME");
                _currentState = RELAY_1;
                if(_relays[0].relayOn) s = "ON"; else s = "OFF";
                display.changeDisplay("RELAY_1",s);
                
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

            _guiOutput.isRelay = false;
            _guiOutput.relayIndex = -1;
            _guiOutput.sensorIndex = 0;
            _guiOutput.newTarget = _sensorInputs[0].target;
            _guiOutput.newLowerLimit = _sensorInputs[0].lowerLimit;
            _guiOutput.newUpperLimit = _sensorInputs[0].upperLimit;
            
            if(_buttonInputs[1].action == "isPressed" && _buttonInputs[2].action == "isPressed"){
                if(!_checkTimer()){
                    Serial.println("sufficient time has not passed FLOOR");
                }else{
                    multi_click_time_begin = millis();
                    Serial.println("BOTH BUTTONS PRESED FLOOR");
                    _currentState = RELAY_1;
                    if(_relays[0].relayOn) s = "ON"; else s = "OFF";
                    display.changeDisplay("RELAY_1",s);
                }
            }
                    else if(_buttonInputs[0].action == "wasReleased"){
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
            
            _guiOutput.isRelay = false;
            _guiOutput.relayIndex = -1;
            _guiOutput.sensorIndex = 1;
            _guiOutput.newTarget = _sensorInputs[1].target;
            
            if(_buttonInputs[1].action == "isPressed" && _buttonInputs[2].action == "isPressed"){
                if(!_checkTimer()){
                    Serial.println("sufficient time has not passed CAB");
                }else{
                    multi_click_time_begin = millis();
                    Serial.println("BOTH BUTTONS PRESED CAB");
                    _currentState = RELAY_1;
                    if(_relays[0].relayOn) s = "ON"; else s = "OFF";
                    display.changeDisplay("RELAY_1",s);
                    
                }
            }
            else if(_buttonInputs[0].action == "wasReleased"){
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
            
            _guiOutput.isRelay = false;
            _guiOutput.relayIndex = -1;
            _guiOutput.sensorIndex = 2;
            _guiOutput.newTarget = _sensorInputs[2].target;
            _guiOutput.newLowerLimit = _sensorInputs[2].lowerLimit;
            _guiOutput.newUpperLimit = _sensorInputs[2].upperLimit;
            
            
            if(_buttonInputs[1].action == "isPressed" && _buttonInputs[2].action == "isPressed"){
                if(!_checkTimer()){
                    Serial.println("sufficient time has not passed ROOM");
                }else{
                    multi_click_time_begin = millis();
                    Serial.println("BOTH BUTTONS PRESED ROOM");
                    _currentState = RELAY_1;
                    if(_relays[0].relayOn) s = "ON"; else s = "OFF";
                    display.changeDisplay("RELAY_1",s);
                    
                }
            }
            else if(_buttonInputs[0].action == "wasReleased"){
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
        
            if(_buttonInputs[1].action == "isPressed" && _buttonInputs[2].action == "isPressed"){
                if(!_checkTimer()){
                    Serial.println("sufficient time has not passed MULTI");
                }else{
                    multi_click_time_begin = millis();
                    Serial.println("BOTH BUTTONS PRESED MULTI");
                    _currentState = RELAY_1;
                    if(_relays[0].relayOn) s = "ON"; else s = "OFF";
                    display.changeDisplay("RELAY_1",s);
                    
                }
            }
            else if(_buttonInputs[0].action == "wasReleased"){
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
        case RELAY_1:
            if(_relays[0].relayOn) s = "ON "; else s = "OFF";
            display.updateSensorDisplay(s);
            _guiOutput.isRelay = true;
            _guiOutput.relayIndex = 0;
            
            if(_buttonInputs[1].action == "isPressed" && _buttonInputs[2].action == "isPressed"){
                if(!_checkTimer()){
                    Serial.println("sufficient time has not passed RELAY_1");
                }else{
                    multi_click_time_begin = millis();
                    Serial.println("BOTH BUTTONS PRESED RELAY_1");
                    _currentState = FLOOR;
                    s = (String)_sensorInputs[0].value + " hold-"+(String)_sensorInputs[0].target;
                    display.changeDisplay(_sensorInputs[0].name,s);
                    _displayedState = FLOOR;
                    Serial.println(_sensorInputs[0].name);
                    Serial.println(s);
                }
            }
            else if(_buttonInputs[0].action == "wasReleased"){
                Serial.println("menu was pressed RELAY_1->FLOOR");
                _currentState = RELAY_2;
             //   s = (String)_sensorInputs[0].value + " hold-"+(String)_sensorInputs[0].target;
                if(_relays[1].relayOn) s = "ON "; else s = "OFF";
                display.changeDisplay("RELAY_2",s);
               // _displayedState = FLOOR;
               // Serial.println(_sensorInputs[0].name);
               // Serial.println(s);
            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up was released RELAY_1");
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down was released RELAY_1");
            }else if(_buttonInputs[3].action == "wasReleased"){
                Serial.println("warning was released RELAY_1");
                _guiOutput.needToUpdate = true;
                _guiOutput.toggleOnOff = true;
            }
            break;
        case RELAY_2:
            if(_relays[1].relayOn) s = "ON "; else s = "OFF";
            display.updateSensorDisplay(s);
            _guiOutput.isRelay = true;
            _guiOutput.relayIndex = 1;
            
            if(_buttonInputs[1].action == "isPressed" && _buttonInputs[2].action == "isPressed"){
                if(!_checkTimer()){
                    Serial.println("sufficient time has not passed RELAY_2");
                }else{
                    multi_click_time_begin = millis();
                    Serial.println("BOTH BUTTONS PRESED RELAY_2");
                    _currentState = FLOOR;
                    s = (String)_sensorInputs[0].value + " hold-"+(String)_sensorInputs[0].target;
                    display.changeDisplay(_sensorInputs[0].name,s);
                    _displayedState = FLOOR;
                    Serial.println(_sensorInputs[0].name);
                    Serial.println(s);
                }
            }
            else if(_buttonInputs[0].action == "wasReleased"){
                Serial.println("menu was pressed RELAY_2->FLOOR");
                _currentState = RELAY_3;
                if(_relays[2].relayOn) s = "ON"; else s = "OFF";
                display.changeDisplay("RELAY_3",s);
                
            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up was released RELAY_2");
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down was released RELAY_2");
            }else if(_buttonInputs[3].action == "wasReleased"){
                Serial.println("warning was released RELAY_2");
                _guiOutput.needToUpdate = true;
                _guiOutput.toggleOnOff = true;
            }
            break;
        case RELAY_3:
            if(_relays[2].relayOn) s = "ON "; else s = "OFF";
            display.updateSensorDisplay(s);
            _guiOutput.isRelay = true;
            _guiOutput.relayIndex = 2;
            
            if(_buttonInputs[1].action == "isPressed" && _buttonInputs[2].action == "isPressed"){
                if(!_checkTimer()){
                    Serial.println("sufficient time has not passed RELAY_3");
                }else{
                    multi_click_time_begin = millis();
                    Serial.println("BOTH BUTTONS PRESED RELAY_3");
                    _currentState = FLOOR;
                    s = (String)_sensorInputs[0].value + " hold-"+(String)_sensorInputs[0].target;
                    display.changeDisplay(_sensorInputs[0].name,s);
                    _displayedState = FLOOR;
                    Serial.println(_sensorInputs[0].name);
                    Serial.println(s);
                }
            }
            else if(_buttonInputs[0].action == "wasReleased"){
                Serial.println("menu was pressed RELAY_2->FLOOR");
                _currentState = RELAY_4;
                if(_relays[3].relayOn) s = "ON"; else s = "OFF";
                display.changeDisplay("RELAY_4",s);
                
            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up was released RELAY_3");
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down was released RELAY_3");
            }else if(_buttonInputs[3].action == "wasReleased"){
                Serial.println("warning was released RELAY_3");
                _guiOutput.needToUpdate = true;
                _guiOutput.toggleOnOff = true;
            }
            break;
        case RELAY_4:
            if(_relays[3].relayOn) s = "ON "; else s = "OFF";
            display.updateSensorDisplay(s);
            _guiOutput.isRelay = true;
            _guiOutput.relayIndex = 3;
            
            if(_buttonInputs[1].action == "isPressed" && _buttonInputs[2].action == "isPressed"){
                if(!_checkTimer()){
                    Serial.println("sufficient time has not passed RELAY_4");
                }else{
                    multi_click_time_begin = millis();
                    Serial.println("BOTH BUTTONS PRESED RELAY_4");
                    _currentState = FLOOR;
                    s = (String)_sensorInputs[0].value + " hold-"+(String)_sensorInputs[0].target;
                    display.changeDisplay(_sensorInputs[0].name,s);
                    _displayedState = FLOOR;
                    Serial.println(_sensorInputs[0].name);
                    Serial.println(s);
                }
            }
            else if(_buttonInputs[0].action == "wasReleased"){
                Serial.println("menu was pressed RELAY_4->RELAY_5");
                _currentState = RELAY_5;
                if(_relays[4].relayOn) s = "ON"; else s = "OFF";
                display.changeDisplay("RELAY_5",s);
                
            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up was released RELAY_4");
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down was released RELAY_4");
            }else if(_buttonInputs[3].action == "wasReleased"){
                Serial.println("warning was released RELAY_4");
                _guiOutput.needToUpdate = true;
                _guiOutput.toggleOnOff = true;
            }
            break;
        case RELAY_5:
            if(_relays[4].relayOn) s = "ON "; else s = "OFF";
            display.updateSensorDisplay(s);
            _guiOutput.isRelay = true;
            _guiOutput.relayIndex = 4;
            
            if(_buttonInputs[1].action == "isPressed" && _buttonInputs[2].action == "isPressed"){
                if(!_checkTimer()){
                    Serial.println("sufficient time has not passed RELAY_5");
                }else{
                    multi_click_time_begin = millis();
                    Serial.println("BOTH BUTTONS PRESED RELAY_5");
                    _currentState = FLOOR;
                    s = (String)_sensorInputs[0].value + " hold-"+(String)_sensorInputs[0].target;
                    display.changeDisplay(_sensorInputs[0].name,s);
                    _displayedState = FLOOR;
                    Serial.println(_sensorInputs[0].name);
                    Serial.println(s);
                }
            }
            else if(_buttonInputs[0].action == "wasReleased"){
                Serial.println("menu was pressed RELAY_5->RELAY_6");
                _currentState = RELAY_6;
                if(_relays[5].relayOn) s = "ON"; else s = "OFF";
                display.changeDisplay("RELAY_6",s);
                
            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up was released RELAY_5");
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down was released RELAY_5");
            }else if(_buttonInputs[3].action == "wasReleased"){
                Serial.println("warning was released RELAY_5");
                _guiOutput.needToUpdate = true;
                _guiOutput.toggleOnOff = true;
            }
            break;
        case RELAY_6:
            if(_relays[5].relayOn) s = "ON "; else s = "OFF";
            display.updateSensorDisplay(s);
            _guiOutput.isRelay = true;
            _guiOutput.relayIndex = 5;
            
            if(_buttonInputs[1].action == "isPressed" && _buttonInputs[2].action == "isPressed"){
                if(!_checkTimer()){
                    Serial.println("sufficient time has not passed RELAY_6");
                }else{
                    multi_click_time_begin = millis();
                    Serial.println("BOTH BUTTONS PRESED RELAY_6");
                    _currentState = FLOOR;
                    s = (String)_sensorInputs[0].value + " hold-"+(String)_sensorInputs[0].target;
                    display.changeDisplay(_sensorInputs[0].name,s);
                    _displayedState = FLOOR;
                    Serial.println(_sensorInputs[0].name);
                    Serial.println(s);
                }
            }
            else if(_buttonInputs[0].action == "wasReleased"){
                Serial.println("menu was pressed RELAY_6->RELAY_7");
                _currentState = RELAY_7;
                if(_relays[6].relayOn) s = "ON"; else s = "OFF";
                display.changeDisplay("RELAY_7",s);
                
            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up was released RELAY_6");
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down was released RELAY_6");
            }else if(_buttonInputs[3].action == "wasReleased"){
                Serial.println("warning was released RELAY_6");
                _guiOutput.needToUpdate = true;
                _guiOutput.toggleOnOff = true;
            }
            break;
        case RELAY_7:
            if(_relays[6].relayOn) s = "ON "; else s = "OFF";
            display.updateSensorDisplay(s);
            _guiOutput.isRelay = true;
            _guiOutput.relayIndex = 6;
            
            if(_buttonInputs[1].action == "isPressed" && _buttonInputs[2].action == "isPressed"){
                if(!_checkTimer()){
                    Serial.println("sufficient time has not passed RELAY_7");
                }else{
                    multi_click_time_begin = millis();
                    Serial.println("BOTH BUTTONS PRESED RELAY_7");
                    _currentState = FLOOR;
                    s = (String)_sensorInputs[0].value + " hold-"+(String)_sensorInputs[0].target;
                    display.changeDisplay(_sensorInputs[0].name,s);
                    _displayedState = FLOOR;
                    Serial.println(_sensorInputs[0].name);
                    Serial.println(s);
                }
            }
            else if(_buttonInputs[0].action == "wasReleased"){
                Serial.println("menu was pressed RELAY_7->RELAY_1");
                _currentState = RELAY_1;
                if(_relays[0].relayOn) s = "ON"; else s = "OFF";
                display.changeDisplay("RELAY_1",s);
                
            }else if(_buttonInputs[1].action == "wasReleased"){
                Serial.println("up was released RELAY_7");
            }else if(_buttonInputs[2].action == "wasReleased"){
                Serial.println("down was released RELAY_7");
            }else if(_buttonInputs[3].action == "wasReleased"){
                Serial.println("warning was released RELAY_7");
                _guiOutput.needToUpdate = true;
                _guiOutput.toggleOnOff = true;
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
