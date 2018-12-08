//
//  ButtonController2.h
//  
//
//  Created by Joe Cutro on 12/7/18.
//
//

#ifndef ButtonController2_h
#define ButtonController2_h
#include <ButtonListener.h>

struct ButtonOutput{
    String name;
    int pin;
    String action;
    float lastChange;
    bool hasChanged;
};

class ButtonController2{
private:
    int _arrLength;
    ButtonListener *_listenerArr[10];
    ButtonOutput _output[5];
public:
    ButtonController2(){_arrLength=0;}
    int arrayLength(){return _arrLength;}
    void beginSequence(); //begin all buttons of buttonListeners in array
    void listening(); //read all buttons of buttonListeners in array using updateListener
    void addButton(String name, int pin);//create ButtonListener, ButtonTalker and Button from inputs
    ButtonOutput * getButtonOutputs(); //
    String toString(); //prep string of member values for serial print
    
};
#endif /* ButtonController2_h */

void ButtonController2::beginSequence(){
    for(int i=0;i<_arrLength;i++){
        _listenerArr[i]->beginListener();
    }
}

void ButtonController2::listening(){
    for(int i=0;i<_arrLength;i++){
        _listenerArr[i]->updateListener();
    }
}

void ButtonController2::addButton(String name, int pin){
    //create button and add button listener
    _listenerArr[_arrLength]= &ButtonListener(name,&Button(pin,25,false,false),pin);
    _arrLength++;
}

ButtonOutput * ButtonController2::getButtonOutputs(){
    for(int i=0;i<_arrLength;i++){
        _output[i] = { _listenerArr[i]->getName(),
                    _listenerArr[i]->getPin(),
                    _listenerArr[i]->getAction(),
                    _listenerArr[i]->getLastChange(),
                    _listenerArr[i]->hasChanged()};
        
        
    }
    return _output;
}

String ButtonController2::toString(){
    return
}


