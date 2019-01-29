//
//  HydronicSystemStructs.h
//  
//
//  Created by Joseph Cutro on 1/19/19.
//

#ifndef HydronicSystemStructs_h
#define HydronicSystemStructs_h
/*
//sensor struct
struct SensorMessage{
    String name;
    float value;
    //   float target;
    //    float upperLimit;
    //    float lowerLimit;
    float lastChanged;
    float hasChanged;
};

struct ActuatorMessage{
    String name;
    String type;
    //int pin;
    bool powerOn;
};
*/

//group of sensors, actuators, timers, tempature values, boolean values
//used to manage control of zone temperatures
struct ZoneGroup{
    String name;
    float targetTemp;
    float lowTemp;
    float highTemp;
    float alarmLowTemp;
    float alarmHighTemp;
    Actuator* controlValve;
    Actuator* backflowValve;
    Actuator* circFan;
    Actuator* finFan;
    Sensor* finTemp;
    Sensor* returnWaterTemp;
    Sensor* zoneTemp;
    //   Timer* waterFlowTimer;
    bool waterFlowOpen;
    bool toggleWaterFlow;
    bool zoneControlOn;
    bool alarmState;
    String alarmMessage;
};

struct HydronicSystemMessage{
    String* zoneGroups; //an array of arrays of stringified zone groups
    bool secondaryPumpOn;
    bool secondaryBypassOpen;
    int zoneCount;
    bool systemRunning;
    bool alarmState;
    unsigned long runningTime;
    
};

#endif /* HydronicSystemStructs_h */
