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
#include <ButtonListener.hpp>

class ButtonController{
private:
    int _length;
    ButtonListener * _listeners;
    Button _getButton(int);
public:
    ButtonController();
    void addButton(String, int);//name,button,pin
    void start();//initiates button in listenerArray
    void listen();//reads from all buttons
    void getListener(int);//return listener in array at index
    void getListeners();//return _listener_array
    int getLength(){return _length;}
};

#endif /* ButtonController_hpp */
