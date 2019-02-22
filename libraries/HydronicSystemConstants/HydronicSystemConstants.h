//
//  HydronicSystemConstants.h
//  
//
//  Created by Joseph Cutro on 1/19/19.
//

#ifndef HydronicSystemConstants_h
#define HydronicSystemConstants_h

//enum Function {HEATING, COOLING};

const byte floorTempSensorPin PROGMEM = 100;
const byte floorPumpPin PROGMEM = 100;
const byte floorReturnTempSensorPin PROGMEM = 100;
const byte floorControlValvePin PROGMEM = 36;
const byte floorBackflowValvePin PROGMEM = 100;
//const byte floorPumpTempSensorPin = 100;
//const byte floorFlowSensorPin = 100;
//const byte floorGreenLedPin = 10;
//const byte floorRedLedPin = 7;

const byte cabTempSensorPin PROGMEM = 100;
const byte cabPumpPin PROGMEM = 100;
const byte cabFinTempSensorPin PROGMEM = 100;
const byte cabFinFanPin PROGMEM = 100;
const byte cabReturnTempSensorPin PROGMEM = 100;
const byte cabControlValvePin PROGMEM = 37;
const byte cabBackflowValvePin PROGMEM = 100;
//const byte cabPumpTempSensorPin = 100;
//const byte cabFlowSensorPin = 100;
//const byte cabGreenLedPin = 11;
//const byte cabRedLedPin = 8;

const byte roomTempSensorPin PROGMEM = 100;
const byte roomPumpPin PROGMEM = 100;
const byte roomFinTempSensorPin PROGMEM = 100;
const byte roomFinFanPin PROGMEM = 100;
const byte roomCircFanPin PROGMEM = 100;
const byte roomReturnTempSensorPin PROGMEM = 100;
const byte roomControlValvePin PROGMEM = 100;
const byte roomBackflowValvePin PROGMEM = 100;
//const byte roomPumpTempSensorPin = 100;
//const byte roomFlowSensorPin = 100;
//const byte roomGreenLedPin = 13;
//const byte roomRedLedPin = 9;


const byte supplyTempSensorPin PROGMEM = 100;
const byte mainPumpPressureSensorPin PROGMEM = 100;
const byte mainPumpPin PROGMEM = 100;
const byte secondaryPumpPin PROGMEM = 100;
const byte secondaryPumpBypassValvePin PROGMEM = 100;

const String zoneOff  = "zone Off";
const String zoneOK  = "zone Ok";
const String zoneRising  = "zone Rising OK";
const String zoneFalling  = "zone Falling OK";
const String zoneCold  = "zone Cold";
const String zoneHot  = "zone Hot";
const String zoneColdAlarm = "zone TOO COLD";
const String zoneHotAlarm = "zone TOO HOT";






#endif /* HydronicSystemConstants_h */
