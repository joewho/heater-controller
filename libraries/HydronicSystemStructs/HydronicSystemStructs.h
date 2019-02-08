//
//  HydronicSystemStructs.h
//  
//
//  Created by Joseph Cutro on 1/19/19.
//

#ifndef HydronicSystemStructs_h
#define HydronicSystemStructs_h


//group of sensors, actuators, timers, tempature values, boolean values
//used to manage control of zone temperatures
struct ZoneGroup{
    String name;
    float targetTemp;
    float lowTemp;
    float highTemp;
    float alarmLowTemp;
    float alarmHighTemp;
    float targetFinTemp;
    Actuator controlValve;
    Actuator backflowValve;
    Actuator circFan;
    Actuator finFan;
    Sensor finTemp;
    Sensor returnWaterTemp;
    Sensor zoneTemp;
    //   Timer* waterFlowTimer;
    bool waterFlowOpen;
    bool zoneControlOn;
    bool alarmState;
    String alarmMessage;
};

struct HydronicSystemMessage{
    ZoneGroup* zoneGroups[3]; //an array of arrays of stringified zone groups
    int zoneCount;
    Actuator secondaryPump;
    Actuator secondaryPumpBypassValve;
    bool systemRunning;
    bool alarmState;
    String alarmMessage;
    unsigned long runningTime;
};

#endif /* HydronicSystemStructs_h */
