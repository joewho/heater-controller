//
//  ButtonController.hpp
//  
//
//  Created by Joe Cutro on 11/28/18.
//
//

#ifndef ButtonController_hpp
#define ButtonController_hpp

#include <JC_Button.h>

class ButtonController{
private:
    int _length;
    Button* _listeners[];
    Button _getButton();
public:
    ButtonController();
    void addButton(String, Button, int);//name,button,pin
    void start();//initiates button in listenerArray
    void listen();//reads from all buttons
    void getListener(int)//return listener in array at index
    void getListeners();//return _listener_array
};

#endif /* ButtonController_hpp */
