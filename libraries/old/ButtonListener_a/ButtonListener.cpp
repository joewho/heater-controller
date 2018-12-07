//
//  ButtonListener.cpp
//  
//
//  Created by Joe Cutro on 11/28/18.
//
//

#include "ButtonListener.hpp"
const int LONG_PRESS = 1000;
ButtonListener::ButtonListener(){
    _name = "none";
    //_button = nullpt;
    _button_pin = -1;
    _action = "none";
    _last_change = 0;
    
}
ButtonListener::ButtonListener(String n, int pin){
    Button b(pin,25,false,false);
    _name = n;
    _button = &b;
    _button_pin = pin;
    _action = "none";
    _last_change = 0;
}

void ButtonListener::updateAction(){
    String act;
    if(_button->isPressed()) act = "isPressed";
    else if(_button->isReleased()) act = "isReleased";
    else if(_button->wasPressed()) act = "wasPressed";
    else if(_button->wasReleased()) act = "wasReleased";
    else if(_button->pressedFor(LONG_PRESS)) act = "pressedFor";
    else if(_button->releasedFor(LONG_PRESS)) act = "releasedFor";
    //if(listenerArr[index]->button->isPressed()) act = "isPressed";
    //if(listenerArr[index]->button->isReleased()) act = "isReleased";
    //if(listenerArr[index]->button->wasPressed()) act = "wasPressed";
    //if(listenerArr[index]->button->wasReleased()) act = "wasReleased";
    //if(listenerArr[index]->button->pressedFor(LONG_PRESS)) act = "pressedFor";
    //if(listenerArr[index]->button->releasedFor(LONG_PRESS)) act = "releasedFor";
    
    _action = act;
   _last_change = _button->lastChange();
}


