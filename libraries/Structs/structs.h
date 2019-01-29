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
    int relayPin;
    bool relayOn;
    float lastChanged;
    bool hasChanged;
};

//gui_tx struct
//gui_rx struct

#endif /* structs_h */
