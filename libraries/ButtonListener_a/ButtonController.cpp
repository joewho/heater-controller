//
//  ButtonController.cpp
//  
//
//  Created by Joe Cutro on 11/28/18.
//
//
//#include <string.h>
#include "ButtonController.hpp"
#include "ButtonListener.hpp"
//#include <JC_Button.h>

ButtonController::ButtonController(){
    _length = 0;
    //need vector !array to add to end and dlete from end
    //_listeners[100] = {};
}
void ButtonController::addButton(String name, int pin){
    //create ButtonListener from Button b

    
    //add ButtonListener to _listenerArray
    _listeners[_length] = &ButtonListener(name, pin);// new ButtonListener(name, pin);
    //_listeners[_length] = name;
    //increment length
    _length++;
}
void ButtonController::start(){
    for(int i=0;i<_length;i++){
        _listeners[i]->getButton()->begin();
    }
}
void ButtonController::listen(){
    for(int i=0;i<_length;i++){
        _listeners[i]->getButton()->read();
        _listeners[i]->updateAction();
    }
}
ButtonListener ButtonController::getListener(int index){
    return *_listeners[index];
}
ButtonListener ButtonController::getListeners(){
    return _listeners;
}

String ButtonController::toString(){
    String str;
    
    str = "Length: "+(String)_length+"\n";
    str +=" Listener Array{ \n";
    for(int i=0;i<_length;i++){
        str += "[Index: "+(String)i+"]  "
        + _listeners[i]->toStringSimple();
//        + "name: " +(String)_listeners[i].getName()+"  "
 //       + "pin: " +(String)_listeners[i].getPin()+"  "
  //      + "action: "+_listeners[i].getAction()+"  "
   //     + "last_change: "+_listeners[i].getLastChange()+"\n";
        
    }
    str+="}\n";
    return str;

}

