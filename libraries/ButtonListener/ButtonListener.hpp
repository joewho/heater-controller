//
//  ButtonListener.hpp
//  
//
//  Created by Joe Cutro on 11/28/18.
//
//

#ifndef ButtonListener_hpp
#define ButtonListener_hpp

#include <JC_Button.h>

class ButtonListener{
private:
    String _name;
    Button _button;
    int _button_pin;
    String _action;
    unsigned long _last_change;
    
public:
    ButtonListener(String, Button, int);
    
    String getName();
    void setName(String);
    
    Button getButton();
    void setButton(Button);
    
    int getPin();
    void setPin(int);
    
    String getAction();
    
    long getLastChange();
    
    
};

#endif /* ButtonListener_hpp */
