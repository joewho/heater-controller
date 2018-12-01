//
//  ButtonController.cpp
//  
//
//  Created by Joe Cutro on 11/28/18.
//
//

#include "ButtonController.h"
#include "ButtonListener.h"

ButtonController::ButtonController(){
    _length = 0;
    //need vector !array to add to end and dlete from end
    _listener_array = {};
}

ButtonController::_getButton(int index){
    if(index != NULL && _listener_array[index] != NULL){
        return _listener_array[index].getButton();
    }
}

ButtonController::addButton(String name,Button b, int pin){
    //create ButtonListener from Button b
    ButtonListener bL(name, b, pin);
    
    //add ButtonListener to _listenerArray
    
}
