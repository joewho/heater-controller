//
//  ButtonController.hpp
//  
//
//  Created by Joe Cutro on 11/28/18.
//
//

#ifndef ButtonController_hpp
#define ButtonController_hpp
#include <String.h>
#include <Vector.h>
using namespace std;
#include <ButtonListener.hpp>
const int MAX = 16;
class ButtonController{
private:
    int _length = -1;
    //ButtonListener *_listeners[MAX];
    std::vector<ButtonListener> _listeners(MAX);
//    Button & _getButton(int);
public:
    ButtonController();
    void addButton(String, int);//name,button,pin
    void start();//initiates button in listenerArray
    void listen();//reads from all buttons
    ButtonListener getListener(int);//return listener in array at index
    vector<ButtonListener> getListeners();//return _listener_array
    int getLength(){return _length;}
    String toString();
};

#endif /* ButtonController_hpp */
