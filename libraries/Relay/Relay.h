//
//  Relay.h
//  
//
//  Created by Joseph Cutro on 2/9/19.
//

#ifndef Relay_h
#define Relay_h
class Relay{
private:
    String _name;
    byte _pin;
    bool _powerOn;
    bool _inverse;
    unsigned long _lastChanged;
public:
    Relay(){_name="EMPTY"; _pin=100; _powerOn = false; _inverse = false; _lastChanged = 0;}//DigitalWrite(_pin)}
    Relay(String name, byte pin, bool isOn, bool inverse):_name(name), _pin(pin), _powerOn(isOn), _inverse(inverse){ _lastChanged = 0; setPowerOn(_powerOn);}
    
    String getName(){return _name;}
    byte getPin(){return _pin;}
    bool isPowerOn(){return _powerOn;}
    void setPowerOn(bool b){
        Serial.println("Relay::setPowerOn");
        Serial.println(_name);
        Serial.print("PowerOn: ");
        Serial.println((b)?"On":"Off");
        Serial.println();
        if(_powerOn != b)
            _lastChanged = millis();
        _powerOn = b;
        if(_inverse)
            digitalWrite(_pin,!_powerOn);
        else
            digitalWrite(_pin,_powerOn);
        
        
    }
    bool isInverse(){return _inverse;}
    //bool hasChanged(){return _hasChanged;}
    long lastChanged(){return _lastChanged;}
    //bool setInverse(bool b){_inverse = b;}
};

#endif /* Relay_h */
