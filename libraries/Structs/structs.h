//
//  structs.h
//  
//
//  Created by Joseph Cutro on 1/19/19.
//

#ifndef structs_h
#define structs_h
//button struct
struct ButtonMessage{
    String name;
    int pin;
    String action;
    float lastChanged;
    bool hasChanged;
};

//relay struct
struct RelayMessage{
    byte index;
    String name;
    byte pin;
    bool powerOn;
    bool isInverse;
    float lastChanged;
    bool hasChanged;
};
struct HydronicDisplayData{
    int arrayIndex;
    String name;
    float currentTemp;
    float targetTemp;
    float lowTemp;
    float highTemp;
    bool zoneControlOn;
    bool waterFlowOpen;
    bool alarmState;
    String alarmMessage;
};
//gui_tx struct
//gui_rx struct

#endif /* structs_h */
