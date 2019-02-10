//
//  HydronicSystemObject.h
//  
//
//  Created by Joseph Cutro on 1/28/19.
//

#ifndef HydronicSystemObject_h
#define HydronicSystemObject_h

#include "Sensor.h"
#include "Actuator.h"
//#include "Timer.h"
#include "HydronicSystemConstants.h"
#include "HydronicSystemStructs.h"

float floorTargetTemp = 88; //default floor zone target temp
float cabinetTargetTemp = 55; //default cabinet zone target temp
float roomTargetTemp = 60; //default room zone target temp
float finTargetTemp = roomTargetTemp;
int defaultThreshold = 3; //default number of degrees used for upper and lower limits of target temp

//instantiate sensors
Sensor floorZoneTemp,floorReturnTemp;
Sensor cabinetZoneTemp, cabinetFinTemp, cabinetReturnTemp;
Sensor roomZoneTemp, roomFinTemp, roomReturnTemp;
Sensor waterSupplyTemp;

//instantiate actuators
Actuator secondaryPump, secondaryPumpBypassValve;
Actuator floorControlValve, floorBackflowValve;
Actuator cabinetControlValve, cabinetBackflowValve, cabinetFinFan;
Actuator roomControlValve, roomBackflowValve, roomFinFan, roomCircFan;

//instantiate zoneGroups
ZoneGroup floorZone, cabinetZone, roomZone;

class HydronicSystemObject{
private:
    ZoneGroup* _zoneGroups[3];
    int _zoneCount;
    Actuator _secondaryPump;
    Actuator _secondaryPumpBypassValve;
    Sensor _waterSupplyTemp;
    bool _systemRunning;
    bool _alarmState;
    String _alarmMessage;
    unsigned long _startTime;
    unsigned long _runningTime;
    Sensor* _sensorArray[9];
    int _sensorCount;
    
    void _analyzeSensorReading(int);
    void _mimicSensorRead(int);

public:
    HydronicSystemObject(){
        _zoneCount = 0;
        _systemRunning = false;
        _alarmState = false;
        _startTime = millis();
        _runningTime = millis() - _startTime;
        
    }
    
    void initiate();
    void updateSystem(); //gets new sensor values and updates memeber objects
    HydronicSystemMessage getSystemStatus(); //outputs string array of stringified memberfucntions and zoneGroups
    void stop();
    void kill();
    void editSettings(HydronicDisplayData);
    unsigned long getRunningTime(){
        _runningTime = millis() - _startTime;
        return _runningTime;
    }

};

void HydronicSystemObject::_mimicSensorRead(int index){
    /*
     _sensorArray[0] = &floorZone.zoneTemp;
     _sensorArray[1] = &floorZone.returnWaterTemp;
     _sensorArray[2] = &cabinetZone.zoneTemp;
     _sensorArray[3] = &cabinetZone.finTemp;
     _sensorArray[4] = &cabinetZone.returnWaterTemp;
     _sensorArray[5] = &roomZone.zoneTemp;
     _sensorArray[6] = &roomZone.finTemp;
     _sensorArray[7] = &roomZone.returnWaterTemp;
     _sensorArray[8] = &waterSupplyTemp;
     */
    switch(index){
        case 0: //floor zone temp
            if(_zoneGroups[0]->waterFlowOpen)//(floorZone.waterFlowOpen)
                _zoneGroups[0]->zoneTemp.read(1);
            else
                _zoneGroups[0]->zoneTemp.read(-1);
            break;
        case 1:
            break;
        case 2://cabinet zone temp
            if(cabinetZone.waterFlowOpen)
                _zoneGroups[1]->zoneTemp.read(1);
            else
                _zoneGroups[1]->zoneTemp.read(-1);
            break;
        case 3:
            break;
        case 4:
            break;
        case 5: //room zone temp
            if(roomZone.waterFlowOpen)
                _zoneGroups[2]->zoneTemp.read(1);
            else
                _zoneGroups[2]->zoneTemp.read(-1);
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
    }
    
}
void HydronicSystemObject::_analyzeSensorReading(int index){
    /*
     _sensorArray[0] = &floorZone.zoneTemp;
     _sensorArray[1] = &floorZone.returnWaterTemp;
     _sensorArray[2] = &cabinetZone.zoneTemp;
     _sensorArray[3] = &cabinetZone.finTemp;
     _sensorArray[4] = &cabinetZone.returnWaterTemp;
     _sensorArray[5] = &roomZone.zoneTemp;
     _sensorArray[6] = &roomZone.finTemp;
     _sensorArray[7] = &roomZone.returnWaterTemp;
     _sensorArray[8] = &waterSupplyTemp;
     */
    
    float sensorValue = _sensorArray[index]->getValue();
    
    //input index is reference to the _sensorArray
    switch(index){
        case 0: //floor zone temp
            //check if zone control is turned off

            if(!floorZone.zoneControlOn){
                //Serial.println("FLOOR OFF");
                //turn off all valves, set waterFLow false, set alarmState false
                floorZone.controlValve.setPowerOn(false);
                floorZone.backflowValve.setPowerOn(false);
                floorZone.waterFlowOpen = false;
                floorZone.alarmState = false;
                floorZone.alarmMessage = zoneOff;
            }else{ //zone control is on
                if(sensorValue >= floorZone.lowTemp && sensorValue <= floorZone.highTemp){
                    floorZone.alarmState = false;
                    if(floorZone.waterFlowOpen)
                        floorZone.alarmMessage = zoneRising;
                    else
                        floorZone.alarmMessage = zoneFalling;
                }
                 else if(sensorValue < floorZone.lowTemp){
                     //Serial.println("FLOOR LOW");
                    //turn on heat
                    floorZone.controlValve.setPowerOn(true);
                    floorZone.backflowValve.setPowerOn(true);
                    floorZone.waterFlowOpen = true;
                    
                    floorZone.alarmState = false;
                    floorZone.alarmMessage = zoneCold;
                    //if less than alarm temp turn on alarm if not turn alarm off
                    if(sensorValue < floorZone.alarmLowTemp){
                        //Serial.println("FLOOR ALARM LOW");
                        floorZone.alarmState = true;
                        //_alarmState = true;
                        floorZone.alarmMessage = zoneColdAlarm;
                       // _alarmMessage = floorZone.alarmMessage
                    }
                }
                else if(sensorValue > floorZone.highTemp){
                    //Serial.println("FLOOR HIGH");
                    //turn off heat
                    floorZone.controlValve.setPowerOn(false);
                    floorZone.backflowValve.setPowerOn(false);
                    floorZone.waterFlowOpen = false;
                    floorZone.alarmState = false;
                    floorZone.alarmMessage = zoneHot;
                    //if higher than alarm temp turn on alarm if not turn alarm off
                    if(sensorValue > floorZone.alarmHighTemp){
                        //Serial.println("FLOOR ALARM HIGH");
                        floorZone.alarmState = true;
                        floorZone.alarmMessage = zoneHotAlarm;
                    }
                }
            }
            break;
        case 1: //floor return water temp
            break;
        case 2: //cabinet zone temp
            //check if zone control is turned off
            if(!cabinetZone.zoneControlOn){
                //Serial.println("CABINET OFF");
                //turn off all valves, set waterFLow false, set alarmState false
                cabinetZone.controlValve.setPowerOn(false);
                cabinetZone.backflowValve.setPowerOn(false);
                cabinetZone.waterFlowOpen = false;
                cabinetZone.alarmState = false;
                cabinetZone.alarmMessage = "empty";
            }else{ //zone control is on
                if(sensorValue >= cabinetZone.lowTemp && sensorValue <= cabinetZone.highTemp){
                    cabinetZone.alarmState = false;
                    if(cabinetZone.waterFlowOpen)
                        cabinetZone.alarmMessage = zoneRising;
                    else
                        cabinetZone.alarmMessage = zoneFalling;
                }
                else if(sensorValue < cabinetZone.lowTemp){
                    //Serial.println("Cabinet LOW");
                    //turn on heat
                    cabinetZone.controlValve.setPowerOn(true);
                    cabinetZone.backflowValve.setPowerOn(true);
                    cabinetZone.waterFlowOpen = true;
                    cabinetZone.alarmState = false;
                    cabinetZone.alarmMessage = zoneCold;
                    //if less than alarm temp turn on alarm if not turn alarm off
                    if(sensorValue < cabinetZone.alarmLowTemp){
                        //Serial.println("Cabinet ALARM LOW");
                        cabinetZone.alarmState = true;
                        cabinetZone.alarmMessage = zoneColdAlarm;
                    }
                }
                else if(sensorValue > cabinetZone.highTemp){
                    //Serial.println("Cabinet HIGH");
                    //turn off heat
                    cabinetZone.controlValve.setPowerOn(false);
                    cabinetZone.backflowValve.setPowerOn(false);
                    cabinetZone.waterFlowOpen = false;
                    cabinetZone.alarmState = false;
                    cabinetZone.alarmMessage = zoneHot;
                    //if higher than alarm temp turn on alarm if not turn alarm off
                    if(sensorValue > cabinetZone.alarmHighTemp){
                        //Serial.println("Cabinet ALARM HIGH");
                        cabinetZone.alarmState = true;
                        cabinetZone.alarmMessage = zoneHotAlarm;
                    }
                }
            }
            break;
        case 3: //cabinet fin temp
            break;
        case 4: //cabinet return water temp
            break;
        case 5: //room zone temp
            //check if zone control is turned off
            if(!roomZone.zoneControlOn){
                //Serial.println("ROOM OFF");
                //turn off all valves, set waterFLow false, set alarmState false
                roomZone.controlValve.setPowerOn(false);
                roomZone.backflowValve.setPowerOn(false);
                roomZone.waterFlowOpen = false;
                roomZone.alarmState = false;
                roomZone.alarmMessage = "empty";
            }else{ //zone control is on
                if(sensorValue >= roomZone.lowTemp && sensorValue <= roomZone.highTemp){
                    roomZone.alarmState = false;
                    if(roomZone.waterFlowOpen)
                        roomZone.alarmMessage = zoneRising;
                    else
                        roomZone.alarmMessage = zoneFalling;
                }
                else if(sensorValue < roomZone.lowTemp){
                    //Serial.println("Room LOW");
                    //turn on heat
                    roomZone.controlValve.setPowerOn(true);
                    roomZone.backflowValve.setPowerOn(true);
                    roomZone.waterFlowOpen = true;
                    roomZone.alarmState = false;
                    roomZone.alarmMessage = zoneCold;
                    //if less than alarm temp turn on alarm if not turn alarm off
                    if(sensorValue < roomZone.alarmLowTemp){
                        //Serial.println("Room ALARM LOW");
                        roomZone.alarmState = true;
                        roomZone.alarmMessage = zoneColdAlarm;
                    }
                }
                else if(sensorValue > roomZone.highTemp){
                    //Serial.println("Room HIGH");

                    //turn off heat
                    roomZone.controlValve.setPowerOn(false);
                    roomZone.backflowValve.setPowerOn(false);
                    roomZone.waterFlowOpen = false;
                    roomZone.alarmState = false;
                    roomZone.alarmMessage = zoneHot;
                    //if higher than alarm temp turn on alarm if not turn alarm off
                    if(sensorValue > roomZone.alarmHighTemp){
                        //Serial.println("Room ALARM HIGH");
                        roomZone.alarmState = true;
                        roomZone.alarmMessage = zoneHotAlarm;
                    }
                }
            }
            break;
        case 6: //room fin temp
            break;
        case 7: //room return water temp
            break;
        case 8: //water supply tmep
            break;
    }
}

void HydronicSystemObject::initiate(){
    //instatialize temp Sensors
    Sensor floorZoneTemp("Floor","TEMP",floorTempSensorPin);
    pinMode(floorTempSensorPin,INPUT);
    
    Sensor floorReturnTemp("FloorReturn","TEMP",floorReturnTempSensorPin);
    pinMode(floorReturnTempSensorPin,INPUT);

    Sensor cabinetZoneTemp("Cab","TEMP",cabTempSensorPin);
    pinMode(cabTempSensorPin,INPUT);

    Sensor cabinetFinTemp("CabFin","TEMP",cabFinTempSensorPin);
    pinMode(cabFinTempSensorPin,INPUT);

    Sensor cabinetReturnTemp("CabReturn","TEMP",cabReturnTempSensorPin);
    pinMode(cabReturnTempSensorPin,INPUT);
    
    Sensor roomZoneTemp("Room","TEMP",roomTempSensorPin);
    pinMode(roomTempSensorPin,INPUT);
    
    Sensor roomFinTemp("RoomFin","TEMP",roomFinTempSensorPin);
    pinMode(roomFinTempSensorPin,INPUT);
    
    Sensor roomReturnTemp("RoomReturn","TEMP",roomReturnTempSensorPin);
    pinMode(roomReturnTempSensorPin,INPUT);
    
    Sensor waterSupplyTemp("WaterSupply","TEMP",supplyTempSensorPin);
    pinMode(supplyTempSensorPin,INPUT);
    
    Sensor falseSensor("falseSensor","TEMP",100);
    
    
    //instatialize Actuators
    Actuator secondaryPump("SecondaryPump","PUMP",secondaryPumpPin,false);
    pinMode(secondaryPumpPin,OUTPUT);
    
    Actuator secondaryPumpBypassValve("SecondaryPumpBypass","SELENOID", secondaryPumpBypassValvePin,false);
    pinMode(secondaryPumpBypassValvePin,OUTPUT);
    
    Actuator floorControlValve("floorControlValve","SELENOID",floorControlValvePin,false);
    pinMode(floorControlValvePin,OUTPUT);
    
    Actuator floorBackflowValve("floorBackflowValve","SELENOID",floorBackflowValvePin,false);
    pinMode(floorBackflowValvePin,OUTPUT);
    
    Actuator cabinetControlValve("cabControlValve","SELENOID",cabControlValvePin,false);
    pinMode(cabControlValvePin,OUTPUT);
    
    Actuator cabinetBackflowValve("cabBackFlowValve","SELENOID",cabBackflowValvePin,false);
    pinMode(cabBackflowValvePin,OUTPUT);
    
    Actuator cabinetFinFan("cabFinFan","FAN",cabFinFanPin,false);
    pinMode(cabFinFanPin,OUTPUT);
    
    Actuator roomControlValve("roomControlValve","SELENOID",roomControlValvePin,false);
    pinMode(roomControlValvePin,OUTPUT);
    
    Actuator roomBackflowValve("roomBackflowValve","SELENOID",roomBackflowValvePin,false);
    pinMode(roomBackflowValvePin,OUTPUT);
    
    Actuator roomFinFan("roomFinFan","FAN",roomFinFanPin,false);
    pinMode(roomFinFanPin,OUTPUT);
    
    Actuator roomCircFan("roomCircFan","FAN",roomCircFanPin,false);
    pinMode(roomCircFanPin,OUTPUT);
    
    Actuator falseActuator("falseActuator","FAN",100,false);
    
    //instatialize ZoneGroups
    floorZone = {
    name: "Floor", //string
    targetTemp: floorTargetTemp, //float value
    lowTemp: floorTargetTemp - defaultThreshold, //float value
    highTemp: floorTargetTemp + defaultThreshold, //float value
    alarmLowTemp: floorTargetTemp - (2 * defaultThreshold), //float value
    alarmHighTemp: floorTargetTemp + (2 * defaultThreshold), //float value
    targetFinTemp: finTargetTemp,
    controlValve: floorControlValve, //Actuator
    backflowValve: floorBackflowValve, //Actuator
    circFan: falseActuator, //Actuator
    finFan: falseActuator, //Actuator
    finTemp: falseSensor, //Sensor
    returnWaterTemp: floorReturnTemp, //Sensor
    zoneTemp: floorZoneTemp, //Sensor
        //   waterFlowTimer: &floorTimer, //Timer
    waterFlowOpen: false, //bool
    zoneControlOn: true, //bool
    alarmState: false,//bool
    alarmMessage: "empty"//string
    };
    
    cabinetZone = {
    name: "Cabinet", //string
    targetTemp: cabinetTargetTemp, //float value
    lowTemp: cabinetTargetTemp - defaultThreshold, //float value
    highTemp: cabinetTargetTemp + defaultThreshold, //float value
    alarmLowTemp: cabinetTargetTemp - (2 * defaultThreshold), //float value
    alarmHighTemp: cabinetTargetTemp + (2 * defaultThreshold), //float value
    targetFinTemp: finTargetTemp,
    controlValve: cabinetControlValve, //Actuator
    backflowValve: cabinetBackflowValve, //Actuator
    circFan: falseActuator, //Actuator
    finFan: cabinetFinFan, //Actuator
    finTemp: cabinetFinTemp, //Sensor
    returnWaterTemp: cabinetReturnTemp, //Sensor
    zoneTemp: cabinetZoneTemp, //Sensor
        //   waterFlowTimer: &cabinetTimer, //Timer
    waterFlowOpen: false, //bool
    zoneControlOn: true, //bool
    alarmState: false,//bool
    alarmMessage: "empty"//string
    };
    
    roomZone = {
    name: "Room", //string
    targetTemp: roomTargetTemp, //float value
    lowTemp: roomTargetTemp - defaultThreshold, //float value
    highTemp: roomTargetTemp + defaultThreshold, //float value
    alarmLowTemp: roomTargetTemp - (2 * defaultThreshold), //float value
    alarmHighTemp: roomTargetTemp + (2 * defaultThreshold), //float value
    targetFinTemp: finTargetTemp,
    controlValve: roomControlValve, //Actuator
    backflowValve: roomBackflowValve, //Actuator
    circFan: roomCircFan, //Actuator
    finFan: roomFinFan, //Actuator
    finTemp: roomFinTemp, //Sensor
    returnWaterTemp: roomReturnTemp, //Sensor
    zoneTemp: roomZoneTemp, //Sensor
        //   waterFlowTimer: &roomTimer, //Timer
    waterFlowOpen: false, //bool
    zoneControlOn: true, //bool
    alarmState: false, //bool
    alarmMessage: "empty" //string
    };
    
    //set member functions
    _zoneGroups[0] = &floorZone;
    _zoneGroups[1] = &cabinetZone;
    _zoneGroups[2] = &roomZone;
    _zoneCount = 3;
    _secondaryPump = secondaryPump;
    _secondaryPumpBypassValve = secondaryPumpBypassValve;
    _waterSupplyTemp = waterSupplyTemp;
    _systemRunning = true;
    _alarmState = false;
    _runningTime = getRunningTime();
    
    _sensorArray[0] = &floorZone.zoneTemp;//&floorZoneTemp;
    _sensorArray[1] = &floorZone.returnWaterTemp;//floorReturnTemp;
    _sensorArray[2] = &cabinetZone.zoneTemp;//cabinetZoneTemp;
    _sensorArray[3] = &cabinetZone.finTemp;//cabinetFinTemp;
    _sensorArray[4] = &cabinetZone.returnWaterTemp;//cabinetReturnTemp;
    _sensorArray[5] = &roomZone.zoneTemp;//roomZoneTemp;
    _sensorArray[6] = &roomZone.finTemp;//roomFinTemp;
    _sensorArray[7] = &roomZone.returnWaterTemp;//roomReturnTemp;
    _sensorArray[8] = &waterSupplyTemp;
    _sensorCount = 9;
    
    floorZone.zoneTemp.setValue(floorTargetTemp - 10);
    cabinetZone.zoneTemp.setValue(cabinetTargetTemp - 10);
    roomZone.zoneTemp.setValue(roomTargetTemp - 10);
    
    updateSystem();
}

HydronicSystemMessage HydronicSystemObject::getSystemStatus(){

    return HydronicSystemMessage{
    zoneGroups: {_zoneGroups[0],_zoneGroups[1],_zoneGroups[2]},
    zoneCount: _zoneCount,
    secondaryPump: _secondaryPump,
    secondaryPumpBypassValve: _secondaryPumpBypassValve,
    systemRunning: _systemRunning,
    alarmState: _alarmState,
    alarmMessage: _alarmMessage,
    runningTime: getRunningTime()
    };
}
 
void HydronicSystemObject::updateSystem(){

    //getSensorValues
    for(int i=0;i<_sensorCount;i++){
        //get new value from sensor
        _mimicSensorRead(i);
        //compare sensor value to system temps
        _analyzeSensorReading(i);
    }
}

void HydronicSystemObject::stop(){
    //turn off all actuators
    //reset timers and counters
}

void HydronicSystemObject::kill(){
    //turn off all actuators
    //stop all timers and counters
    //delete all sensors, actuators, zoneGroups, reset member functions to empty values
}
void HydronicSystemObject::editSettings(HydronicDisplayData newData){
    if(newData.lowTemp >= newData.targetTemp)
        newData.lowTemp = newData.targetTemp-1;
    if(newData.highTemp <= newData.targetTemp)
        newData.highTemp = newData.targetTemp+1;
    _zoneGroups[newData.arrayIndex]->targetTemp = newData.targetTemp;
    _zoneGroups[newData.arrayIndex]->lowTemp = newData.lowTemp;
    _zoneGroups[newData.arrayIndex]->highTemp = newData.highTemp;
    _zoneGroups[newData.arrayIndex]->zoneControlOn = newData.zoneControlOn;
}

#endif /* HydronicSystemObject_h */
