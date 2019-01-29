//
//  Actuator.h
//  
//
//  Created by Joseph Cutro on 1/20/19.
//

#ifndef Actuator_h
#define Actuator_h
class Actuator{
private:
    String _name;
    String _type; //only allowed: FAN || SELENOID || PUMP
    int _pin;
    bool _powerOn;
public:
    Actuator(){_name="EMPTY"; _type="EMPTY"; _pin=-1; _powerOn=false;}
    Actuator(String name, String type, int pin, bool isOn):_name(name), _type(type),_pin(pin), _powerOn(isOn){}
    
    String getName(){return _name;}
    void setName(String s){_name = s;}
    
    String getType(){return _type;}
    void setType(String s){_type = s;}//only allowed: FAN || SELENOID
    
    int getPin(){return _pin;}
    void setPin(int p){_pin = p;}
    
    bool isPowerOn(){return _powerOn;}
    void setPowerOn(bool b){_powerOn = b;}
    
    
    
};

#endif /* Actuator_h */
