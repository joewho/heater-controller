//
//  ButtonController_12_7_18.h
//  
//
//  Created by Joe Cutro on 12/7/18.
//
//

#ifndef ButtonController_h
#define ButtonController_h
#include <JC_Button.h>
class ButtonListener{
private:
    int _pin;
    Button * _button;
    String _name;
    String _action;
    float _last_change;
public:
    ButtonListener(){_pin = -1; _button = nullptr; _name = "empty";_action="empty";_last_change=0;}
    ButtonListener(String name, Button* b, int pin){_name =name; _button = b; _pin = pin; _action="empty";_last_change=0;}
    
    int getPin(){return _pin;};
    void setPin(int i){_pin = 1;}
    
    Button getButton(){return _button;}
    void setButton(Button* b){_button = b;}
    
    String getName(){return _name;}
    void setName(String s){_name = s;}
    
    String getAction(){return _action;}
    void setAction(String s){_action = s;}
    
    float getLastChange(){return _last_change;}
    void setLastChange(float f){_last_change = f;}
    
    String toString(){
        return "name: "+_name + " pin: "+(String)_pin+" action: "+_action+" lastChange: "+(String)_last_change;
    }
};

#endif /* ButtonListener_h */
