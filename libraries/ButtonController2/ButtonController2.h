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

const int LONG_PRESS = 1000;

struct buttonOutput{
    String name;
    int pin;
    String action;
    String last_change;
};

class ButtonController2{
private:
    int _arrayLength;
    ButtonListener *_listenerArr[10];
    buttonOutput _outputMessage;
public:
    ButtonController2(){}
    
    void startButtons(); //begin all buttons in array
    void readButtons(); //read all buttons in array
    
    
};
#endif /* ButtonController2_h */
