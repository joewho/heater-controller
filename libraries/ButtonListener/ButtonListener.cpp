//
//  ButtonListener.cpp
//  
//
//  Created by Joe Cutro on 11/28/18.
//
//

#include "ButtonListener.hpp"
#include <JC_Button.h>

ButtonListener::ButtonListener(String n, int pin){
    _name = n;
    _button = Button(pin,25,false,false);
    _button_pin = pin;
    _action = "none";
    _last_change = 0;
}
