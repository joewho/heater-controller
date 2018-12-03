//
//  SensorHandler.h
//  
//
//  Created by Joseph Cutro on 12/2/18.
//

#ifndef SensorHandler_h
#define SensorHandler_h
//enum sensorType {TEMP, FLOW, FLOAT};
//enum interaction {CONTROL, READ};
class SensorHandler{
private:
    String _name;
    String _shortName;
    sensorType _sensorType;
    interaction _interaction;
    int _pin;
    float _reading;
    float _target;
    
    float _normalTol;
    float _warningTol;
    bool _callHeat; //true if reading is lower than normal tolerance
    bool _useAsCool; //if true some function will be inverted
    bool _isOn;//true if system is 'on' else false
    bool _alarm; //true when something is wrong
 /*   struct _alarm{
        bool alarmState;
        String name;
        int pin;
        struct message{
            String head;
            String body;
            float reading;
            float target;
            bool _callHeat;
            bool _useAsCool;
        };
    };
  */
public:
    SensorHandler();
    SensorHandler(String, String, int, float, sensorType, interaction,bool);
    
    bool stateChanged; //true if value for callHeat has just been changed
    
    String getName(){return _name;}
    void setName(String s){_name = s;}
    
    String getShortName(){return _shortName;}
    void setShortName(String s){_shortName = s;}
    
    sensorType getType(){return _sensorType;}
    void setType(sensorType s){_sensorType = s;}
    
    interaction getInteraction(){return _interaction;}
    void setInteraction(interaction i){_interaction = i;}
    
    int getPin(){return _pin;}
    void setPin(int i){_pin  = i;}
    
    float getReading(){return _reading;}
    void setReading(float f){_reading = f;}
    
    float getTarget(){return _target;}
    void setTarget(float f){_target = f;}
    
    void incTargetTemp(){_target++;}
    void decTargetTemp(){_target--;}
    
    float getNormTol(){return _normalTol;}
    void setNormTol(float f){_normalTol = f;}
    
    float getWarnTol(){return _warningTol;}
    void setWarnTol(float f){_warningTol = f;}
    
    bool isCallingHeat(){return _callHeat;}
    void setCallingHeat(bool b){_callHeat = b;}
    
    bool useAsCool(){return _useAsCool;}
    void setUseAsCool(bool b){_useAsCool = b;}
    
    bool isOn(){return _isOn;}
    void isOn(bool b){_isOn = b;}
    
    bool inAlarm(){return _alarm;}
    void setAlarm(bool b){_alarm = b;}
    
    void updateHandler();
    
    String toString();
    String toStringLong();
    
    //String getAlarm(){return _alarm;}
    //void setAlarm(String s){_alarm = s;}
 };
SensorHandler::SensorHandler(){
    _name = "Empty Sensor";
    _shortName = "Empty";
    _sensorType = TEMP;
    _interaction = READ;
    _pin = -1;
    _reading = -888;
    _target = -999;
    _normalTol = 3;
    _warningTol = 7;
    _callHeat = false;
    _useAsCool = false;
    _isOn = true;
    _alarm = false;
    stateChanged = false;
    //_alarm = {false,_name,_pin,{}};
}

SensorHandler::SensorHandler(String n, String sn, int p, float t,sensorType st, interaction i,bool cool){
    _name = n;
    _shortName = sn;
    _sensorType = st;
    _interaction = i;
    _pin = p;
    _target = t;
    _reading = t; //set duration creation to have a starting value
    _normalTol = 3;
    _warningTol = 7;
    _callHeat = false;
    _useAsCool = cool;
    _isOn = true;
    _alarm = false;
    stateChanged = false;
    //_alarm = {false,_name,_pin,{}};
}

void SensorHandler::updateHandler(){
    //for evalution and testing update creates a random number to update the sensors
    //instead of getting a reading from the sensor pin
    
    float rnd = (float)random(11)/10;
    if(!_isOn){
        _reading -= rnd;
        _callHeat = false;
    }else{
        if(_callHeat){//calling for heat
            _reading += rnd;//add rnd number to _reading to sim sensor reading
            if(_reading >=(_target + _normalTol)){ //callHeat && temp at/above normal tolerance
                _callHeat = false; //turn off heat
                stateChanged = true; //just changed from needed heat to not, set to true
            }else stateChanged = false; //reading is still lower than normal high tolerance
        }else if (!_callHeat){ //not calling for heat
            _reading -= rnd; //subtract rnd number form _reading to sim sensor reading
            if(_reading <= (_target - _normalTol)){//!callHeat && temp at/below normal tolerance
                _callHeat = true; //turn on heat
                stateChanged = true; //just changed from not needing heat to needing, set to true
            }else stateChanged = false; //reading is above normal low tolerance
        }
    }
}

String SensorHandler::toString(){
    return "Name: "+_name+" Current Value: "+(String)_reading;
}

String SensorHandler::toStringLong(){
    return "name: "+ _name+"\n"
    + "shortName: "+ _shortName+"\n"
    + "sensorType: "+(String)_sensorType+"\n"
    + "interaction: "+(String)_interaction+"\n"
    + "pinNumber: "+ (String)_pin+"\n"
    + "readingTemp: "+(String)_reading+"\n"
    + "targetTemp: "+(String)_target+"\n"
    + "normalTolerance: "+(String)_normalTol+"\n"
    + "warningTolerance: "+(String)_warningTol+"\n"
    + "callHeat: "+(String)_callHeat+"\n"
    + "useAsCool: "+(String)_useAsCool+"\n"
    + "alarm: "+(String)_alarm;
}
#endif /* SensorHandler_h */
