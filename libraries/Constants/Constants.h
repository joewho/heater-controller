//
//  Constants.h
//  
//
//  Created by Joseph Cutro on 12/8/18.
//

#ifndef Constants_h
#define Constants_h


//const byte PROGMEM LCDfirstPin = 26;
const byte PROGMEM LCD_rs=22;//26;
const byte PROGMEM LCD_en=23;//27;
const byte PROGMEM LCD_d4=24;//28;
const byte PROGMEM LCD_d5=25;//29;
const byte PROGMEM LCD_d6=26;//30;
const byte PROGMEM LCD_d7=27;//31;
const byte PROGMEM LCD_RED=5;
const byte PROGMEM LCD_GREEN=6;
const byte PROGMEM LCD_BLUE=7;

//const byte PROGMEM buttonFirstPin = 4;
const byte PROGMEM button1Pin = 28;//7;
const byte PROGMEM button2Pin = 29;//6;
const byte PROGMEM button3Pin = 30;//5;
const byte PROGMEM button4Pin = 31;//4;

const byte PROGMEM relayFirstPin = 39;
//batteries
const byte PROGMEM relay1Pin = 32;
const byte PROGMEM relay2Pin = 33;
const byte PROGMEM relay3Pin = 34;
const byte PROGMEM relay4Pin = 35;
//heater zones
const byte PROGMEM relay5Pin = 36;
const byte PROGMEM relay6Pin = 37;
const byte PROGMEM relay7Pin = 38;
//relays controlled by buttons & LCD
const byte PROGMEM relay8Pin = 39;
const byte PROGMEM relay9Pin = 40;
const byte PROGMEM relay10Pin = 41;
const byte PROGMEM relay11Pin = 42;
const byte PROGMEM relay12Pin = 43;
const byte PROGMEM relay13Pin = 44;
const byte PROGMEM relay14Pin = 45;
//
const byte PROGMEM relay15Pin = 46;
const byte PROGMEM relay16Pin = 47;
const byte PROGMEM relay17Pin = 48;
const byte PROGMEM relay18Pin = 49;
const byte PROGMEM relay19Pin = 50;
const byte PROGMEM relay20Pin = 51;
const byte PROGMEM relay21Pin = 52;
const byte PROGMEM relay22Pin = 53;

const byte battery1 = relay1Pin;
const byte battery2 = relay2Pin;
const byte battery3 = relay3Pin;
const byte battery4 = relay4Pin;
const byte batteryCount = 4;

/*
const byte PROGMEM relay23Pin = 52;
const byte PROGMEM relay24Pin = 53;
*/
#endif /* Constants_h */
