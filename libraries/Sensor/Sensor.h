//
//  Sensor.h
//  
//
//  Created by Joe Cutro on 12/4/18.
//
//

#ifndef Sensor_h
#define Sensor_h
class Sensor{
private:
    String _name;
    String _type; //only allowed: TEMP || PRESSURE
    int _pin;
    float _value;
public:
    Sensor(){_name="EMPTY"; _type = "NONE"; _pin = 100; _value = -8;}
    Sensor(String name, String type, int pin):_name(name), _type(type), _pin(pin){_value = -8;}
    //~Sensor();
    
    String getName(){return _name;}
    //void setName(String s){_name = s;}
    
    //String getType(){return _type;}
    //void setType(String s){_type = s;}
    
    int getPin(){return _pin;}
    //void setPin(int  i){_pin = i;}
    
    //doesnt cause the sensor to read the value of the pin
    //just returns the currrent value of _value
    float getValue(){return _value;}
    void setValue(float f){_value = f;}
    
    //use this for probing sensors for current reading
    //calculate temp and set to _value
    void read(){}
    
    //input value is for testing purposes only
    //input should be -1 or +1 depending on heating or cooling at the moment
    void read(int plusMinus){
        //here it should call an analog read function
//        Serial.println("read-value: "+(String)_value);
        float rnd = (float)random(10)/10;
//        Serial.println("read-rnd: "+(String)rnd);

        _value += (plusMinus * rnd);
//        Serial.println("read-value: "+(String)_value);

    }
    
};

#endif /* Sensor_h */
