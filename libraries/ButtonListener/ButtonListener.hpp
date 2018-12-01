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
    ButtonListener(String, int);
    
    String getName(){return _name;}
    void setName(String s){_name = s;}
    
    Button getButton(){return _button;}
    void setButton(Button b){_button = b;}
    
    int getPin(){return _button_pin;}
    void setPin(int i){_button_pin = i;}
    
    String getAction(){return _action;}
    
    long getLastChange(){return _last_change;}
    
    
};

#endif /* ButtonListener_hpp */
