//
//  ButtonController_12_7_18.h
//  
//
//  Created by Joe Cutro on 12/7/18.
//
//

#ifndef ButtonListener_h
#define ButtonListener_h
#include <JC_Button.h>

const int LONG_PRESS = 1000;

class ButtonListener{
private:
    int _pin;
    Button * _button;
    String _name;
    String _action;
    float _last_change;
    bool _has_changed;
public:
    ButtonListener(){_pin = -1; _button = nullptr; _name = "empty";_action="empty";_last_change=0;;_has_changed=false;}
    ButtonListener(String name, Button* b, int pin){_name =name; _button = b; _pin = pin; _action="empty";_last_change=0;_has_changed=false;}
    
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
    
    bool hasChanged();
    void setHasChanged(bool b);
    
    void beginListener(){
        _button->read();
    }
    
    void updateListener(){
        _button->read();
        if(_button->isPressed()) _action = "isPressed";
        else if(_button->isPressed()) _action = "isPressed";
        else if(_button->isReleased()) _action = "isReleased";
        else if(_button->wasPressed()) _action = "wasPressed";
        else if(_button->wasReleased()) _action = "wasReleased";
        else if(_button->pressedFor(LONG_PRESS)) _action = "pressedFor";
        else if(_button->releasedFor(LONG_PRESS)) _action = "releasedFor";
        
        _last_change = _button->lastChange();
    }
    
    String toString(){
        return "name: "+_name + " pin: "+(String)_pin+" action: "+_action+" lastChange: "+(String)_last_change;
    }
};
bool ButtonListener::hasChanged(){}
void ButtonListener::setHasChanged(bool b){}


#endif /* ButtonListener_h */
