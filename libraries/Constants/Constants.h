//
//  Constants.h
//  
//
//  Created by Joseph Cutro on 12/8/18.
//

#ifndef Constants_h
#define Constants_h

enum states {WELCOME, FLOOR, FLOOR_TARGET, FLOOR_LOWER, FLOOR_UPPER,
    CAB, CAB_TARGET, CAB_LOWER, CAB_UPPER,
    ROOM, ROOM_TARGET, ROOM_LOWER, ROOM_UPPER, MULTI,
    RELAY_1,RELAY_2,RELAY_3,RELAY_4,RELAY_5,RELAY_6,RELAY_7};  // states for ZONE ID and the state machine/display

const byte button1Pin = 25;//5;
const byte button2Pin = 24;//6;
const byte button3Pin = 23;//3;
const byte button4Pin = 22;//4;

const byte relay1Pin = 31;
const byte relay2Pin = 32;
const byte relay3Pin = 33;
const byte relay4Pin = 34;
const byte relay5Pin = 35;
const byte relay6Pin = 36;
const byte relay7Pin = 37;

#endif /* Constants_h */
