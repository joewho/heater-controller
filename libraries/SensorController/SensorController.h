//
//  SensorController.h
//  
//
//  Created by Joe Cutro on 12/11/18.
//
//

#ifndef SensorController_h
#define SensorController_h
#include "Sensor.h"

class SensorController{
private:
    Sensor* _sensor;
    float _target; //desired temp for the sensor to maintain
    int _threshold; // +/- degrees of variance
    //if _ceiling || _floor is null then use threshold for temp variance
    float _ceiling;// highest temp for upper variance
    float _floor; //lowest temp for lower variance
    byte _actuatorPin;
    byte _greenLedPin;
    byte _redLedPin;
    states _zoneID;
    byte _alarmID;
    Function _function;
    bool _actuatorOn;
    bool _toggleActuator;
    bool _zoneOn;
public:
    SensorController(){}
    SensorController
};

#endif /* SensorController_h */
