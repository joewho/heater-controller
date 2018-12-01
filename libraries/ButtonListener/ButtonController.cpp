//
//  ButtonController.cpp
//  
//
//  Created by Joe Cutro on 11/28/18.
//
//

#include "ButtonController.hpp"
#include "ButtonListener.hpp"
#include <JC_Button.h>

ButtonController::ButtonController(){
    _length = 0;
    //need vector !array to add to end and dlete from end
    _listeners = {};
}

Button ButtonController::_getButton(int index){
    if(index != NULL){
        return _listeners[index].getButton();
    }
}

void ButtonController::addButton(String name, int pin){
    //create ButtonListener from Button b
    ButtonListener bL(name, pin);
    
    //add ButtonListener to _listenerArray
    
}
