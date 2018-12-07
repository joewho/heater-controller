//
//  test.hpp
//  
//
//  Created by Joseph Cutro on 12/2/18.
//

#ifndef ButtonController_h
#define ButtonController_h

//#include <String.h>
#include <JC_Button.h>
const byte menu_button_pin = 32;
const byte warning_button_pin = 34;
const byte up_button_pin = 53;
const byte down_button_pin = 51;
const int LONG_PRESS = 1000;
const int MAX_BUFFER = 10;

Button upButton(up_button_pin,25,false,false);
Button downButton(down_button_pin,25,false,false);
Button menuButton(menu_button_pin,25,false,false);
Button warningButton(warning_button_pin,25,false,false);

struct buttonListener{
    String _name;
    Button* button;
    byte button_pin;
    String action;
    String type;
    String  last_change;
};

struct buttonOutput{
    String _name;
    byte input_pin;
    String type;
    String reading;
    String last_change;
};

buttonListener upListener{_name:"UP", button: &upButton, button_pin: up_button_pin};
buttonListener downListener{_name:"DOWN", button: &downButton, button_pin: down_button_pin};
buttonListener menuListener{_name:"MENU", button: &menuButton, button_pin: menu_button_pin};
buttonListener warningListener{_name:"WARNING", button: &warningButton, button_pin: warning_button_pin};

buttonListener* listenerArr[] = {&upListener, &downListener, &menuListener, &warningListener};
const int listenerArrLength = 4;

void setupButtons(){
    for(int i=0;i<listenerArrLength;i++){
        listenerArr[i]->button->begin();
    }
}

void readButtons(){
    for(int i=0;i<listenerArrLength;i++){
        listenerArr[i]->button->read();
    }
}

void currentRead(int index){
    String act;
    if(listenerArr[index]->button->isPressed()) act = "isPressed";
    if(listenerArr[index]->button->isPressed()) act = "isPressed";
    if(listenerArr[index]->button->isReleased()) act = "isReleased";
    if(listenerArr[index]->button->wasPressed()) act = "wasPressed";
    if(listenerArr[index]->button->wasReleased()) act = "wasReleased";
    if(listenerArr[index]->button->pressedFor(LONG_PRESS)) act = "pressedFor";
    if(listenerArr[index]->button->releasedFor(LONG_PRESS)) act = "releasedFor";
    
    listenerArr[index]->action = act;
    listenerArr[index]->last_change = (long)listenerArr[index]->button->lastChange();
}


buttonOutput * listenToAllButtons(){
    static buttonOutput output[listenerArrLength];
    readButtons();

    for(int i=0;i<listenerArrLength;i++){
        currentRead(i);
        output[i] = { listenerArr[i]->_name,
            listenerArr[i]->button_pin,
            "button",
            listenerArr[i]->action,
            listenerArr[i]->last_change
        };
        
    }
    return output;
}

void serialPrintButtonOutput(buttonOutput *output){
    for(int i=0;i<listenerArrLength;i++){
        Serial.print((String)output[i]._name+" "
                     +(String)output[i].input_pin+" "
                     +(String)output[i].type+" "
                     +(String)output[i].reading+" "
                     +(String)output[i].last_change+"   ");
    }
    Serial.println();
}
#endif /* ButtonController_h */
