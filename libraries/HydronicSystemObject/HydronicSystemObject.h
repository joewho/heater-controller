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
    ZoneGroup* _zonesGroups[3];
    int _zoneCount;
    Actuator _secondaryPump;
    Actuator _secondaryPumpBypassValve;
    Sensor _waterSupplyTemp;
    bool _systemRunning;
    bool _alarmState;
    String _alarmMessage;
    unsigned long _runningTime;
    Sensor* _sensorArray[9];
    int _sensorCount;
    
    //SensorMessage* _getAllSensorValues();
    //ActuatorMessage* _getAllActuatorValues();
    void _analyzeSensorReading(int);
    void _mimicSensorRead(int);
    
public:
    HydronicSystemObject(){
        //_zonesGroups = NULL;
        _zoneCount = 0;
        //_secondaryPump = NULL;
        //_secondaryPumpBypassValve = NULL;
        //_waterSupplyTemp = NULL;
        _systemRunning = false;
        _alarmState = false;
        _runningTime = 0;
        
        //save memory
        //delete secondaryPump;
        //delete secondaryPumpBypassValve;
        //delete pumpPressure;
    }
    
    void initiate();
    void updateSystem(); //gets new sensor values and updates memeber objects
    HydronicSystemMessage getSystemStatus(); //outputs string array of stringified memberfucntions and zoneGroups
    void stop();
    void kill();
    void editSettings();
    /*
    string* getSystemStatus(); //outputs string array full of sensor/actuator/value of member functions && specific zone values
    string* getSystemZoneData(); //outputs string array full of all zoneGroup data
    string* getSystemZoneData(int index); //ouputs string array of one zoneGroup
    */
};

//SensorMessage* HydronicSystemObject::_getAllSensorValues(){}

//ActuatorMessage* HydronicSystemObject::_getAllActuatorValues(){}
void HydronicSystemObject::_mimicSensorRead(int index){
    switch(index){
        case 0: //floor zone temp
            if(floorZone.waterFlowOpen)
                _sensorArray[index]->read(1);
            else
                _sensorArray[index]->read(-1);
            break;
        case 1:
            break;
        case 2://cabinet zone temp
            if(cabinetZone.waterFlowOpen)
                _sensorArray[index]->read(1);
            else
                _sensorArray[index]->read(-1);
            break;
        case 3:
            break;
        case 4:
            break;
        case 5: //room zone temp
            if(roomZone.waterFlowOpen)
                _sensorArray[index]->read(1);
            else
                _sensorArray[index]->read(-1);
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
     _sensorArray[0] = &floorZoneTemp;
     _sensorArray[1] = &floorReturnTemp;
     _sensorArray[2] = &cabinetZoneTemp;
     _sensorArray[3] = &cabinetFinTemp;
     _sensorArray[4] = &cabinetReturnTemp;
     _sensorArray[5] = &roomZoneTemp;
     _sensorArray[6] = &roomFinTemp;
     _sensorArray[7] = &roomReturnTemp;
     _sensorArray[8] = &waterSupplyTemp;
     */
    
    float sensorValue = _sensorArray[index]->getValue();

    //input index is reference to the _sensorArray
    switch(index){
        case 0: //floor zone temp
            Serial.print("SensorIndex: "+(String)index);
            Serial.println(" SensorValue: "+(String)sensorValue);
            //check if zone control is turned off
            if(!floorZone.zoneControlOn){
                Serial.println("FLOOR OFF");
                //turn off all valves, set waterFLow false, set alarmState false
                floorZone.controlValve->setPowerOn(false);
                floorZone.backflowValve->setPowerOn(false);
                floorZone.waterFlowOpen = false;
                floorZone.alarmState = false;
                floorZone.alarmMessage = "empty";
            }else{ //zone control is on
                 if(sensorValue < floorZone.lowTemp){
                     Serial.println("FLOOR LOW");
                    //turn on heat
                    floorZone.controlValve->setPowerOn(true);
                    floorZone.backflowValve->setPowerOn(true);
                    floorZone.waterFlowOpen = true;
                    //if less than alarm temp turn on alarm if not turn alarm off
                    if(sensorValue < floorZone.alarmLowTemp){
                        Serial.println("FLOOR ALARM LOW");
                        floorZone.alarmState = true;
                        floorZone.alarmMessage = "Floor Zone too Cold";
                    }
                }
                else if(sensorValue > floorZone.highTemp){
                    Serial.println("FLOOR HIGH");
                    //turn off heat
                    floorZone.controlValve->setPowerOn(false);
                    floorZone.backflowValve->setPowerOn(false);
                    floorZone.waterFlowOpen = false;
                    //if higher than alarm temp turn on alarm if not turn alarm off
                    if(sensorValue > floorZone.alarmHighTemp){
                        Serial.println("FLOOR ALARM HIGH");
                        floorZone.alarmState = true;
                        floorZone.alarmMessage = "Floor Zone too Hot";
                    }
                }
            }
            break;
        case 1: //floor return water temp
            break;
        case 2: //cabinet zone temp
            Serial.print("SensorIndex: "+(String)index);
            Serial.println(" SensorValue: "+(String)sensorValue);
            //check if zone control is turned off
            if(!cabinetZone.zoneControlOn){
                Serial.println("CABINET OFF");
                //turn off all valves, set waterFLow false, set alarmState false
                cabinetZone.controlValve->setPowerOn(false);
                cabinetZone.backflowValve->setPowerOn(false);
                cabinetZone.waterFlowOpen = false;
                cabinetZone.alarmState = false;
                cabinetZone.alarmMessage = "empty";
            }else{ //zone control is on
                if(sensorValue < cabinetZone.lowTemp){
                    Serial.println("Cabinet LOW");
                    //turn on heat
                    cabinetZone.controlValve->setPowerOn(true);
                    cabinetZone.backflowValve->setPowerOn(true);
                    cabinetZone.waterFlowOpen = true;
                    //if less than alarm temp turn on alarm if not turn alarm off
                    if(sensorValue < cabinetZone.alarmLowTemp){
                        Serial.println("Cabinet ALARM LOW");
                        cabinetZone.alarmState = true;
                        cabinetZone.alarmMessage = "Cabinet Zone too Cold";
                    }
                }
                else if(sensorValue > cabinetZone.highTemp){
                    Serial.println("Cabinet HIGH");
                    //turn off heat
                    cabinetZone.controlValve->setPowerOn(false);
                    cabinetZone.backflowValve->setPowerOn(false);
                    cabinetZone.waterFlowOpen = false;
                    //if higher than alarm temp turn on alarm if not turn alarm off
                    if(sensorValue > cabinetZone.alarmHighTemp){
                        Serial.println("Cabinet ALARM HIGH");
                        cabinetZone.alarmState = true;
                        cabinetZone.alarmMessage = "Cabinet Zone too Hot";
                    }
                }
            }
            break;
        case 3: //cabinet fin temp
            break;
        case 4: //cabinet return water temp
            break;
        case 5: //room zone temp
            Serial.print("SensorIndex: "+(String)index);
            Serial.println(" SensorValue: "+(String)sensorValue);
            //check if zone control is turned off
            if(!roomZone.zoneControlOn){
                Serial.println("ROOM OFF");
                //turn off all valves, set waterFLow false, set alarmState false
                roomZone.controlValve->setPowerOn(false);
                roomZone.backflowValve->setPowerOn(false);
                roomZone.waterFlowOpen = false;
                roomZone.alarmState = false;
                roomZone.alarmMessage = "empty";
            }else{ //zone control is on
                if(sensorValue < roomZone.lowTemp){
                    Serial.println("Room LOW");

                    //turn on heat
                    roomZone.controlValve->setPowerOn(true);
                    roomZone.backflowValve->setPowerOn(true);
                    roomZone.waterFlowOpen = true;
                    //if less than alarm temp turn on alarm if not turn alarm off
                    if(sensorValue < roomZone.alarmLowTemp){
                        Serial.println("Room ALARM LOW");
                        roomZone.alarmState = true;
                        roomZone.alarmMessage = "Room Zone too Cold";
                    }
                }
                else if(sensorValue > roomZone.highTemp){
                    Serial.println("Room HIGH");

                    //turn off heat
                    roomZone.controlValve->setPowerOn(false);
                    roomZone.backflowValve->setPowerOn(false);
                    roomZone.waterFlowOpen = false;
                    //if higher than alarm temp turn on alarm if not turn alarm off
                    if(sensorValue > roomZone.alarmHighTemp){
                        Serial.println("Room ALARM HIGH");
                        roomZone.alarmState = true;
                        roomZone.alarmMessage = "Room Zone too Hot";
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
    Sensor floorReturnTemp("FloorReturn","TEMP",floorReturnTempSensorPin);
    
    Sensor cabinetZoneTemp("Cab","TEMP",cabTempSensorPin);
    Sensor cabinetFinTemp("CabFin","TEMP",cabFinTempSensorPin);
    Sensor cabinetReturnTemp("CabReturn","TEMP",cabReturnTempSensorPin);
    
    Sensor roomZoneTemp("Room","TEMP",roomTempSensorPin);
    Sensor roomFinTemp("RoomFin","TEMP",roomFinTempSensorPin);
    Sensor roomReturnTemp("RoomReturn","TEMP",roomReturnTempSensorPin);
    
    Sensor waterSupplyTemp("WaterSupply","TEMP",supplyTempSensorPin);
    
    //instatialize Actuators
    Actuator secondaryPump("SecondaryPump","PUMP",secondaryPumpPin,false);
    Actuator secondaryPumpBypassValve("SecondaryPumpBypass","SELENOID", secondaryPumpBypassValvePin,false);
    
    Actuator floorControlValve("floorControlValve","SELENOID",floorControlValvePin,false);
    Actuator floorBackflowValve("floorBackflowValve","SELENOID",floorBackflowValvePin,false);
    
    Actuator cabinetControlValve("cabControlValve","SELENOID",cabControlValvePin,false);
    Actuator cabinetBackflowValve("cabBackFlowValve","SELENOID",cabBackflowValvePin,false);
    Actuator cabinetFinFan("cabFinFan","FAN",cabFinFanPin,false);
    
    Actuator roomControlValve("roomControlValve","SELENOID",roomControlValvePin,false);
    Actuator roomBackflowValve("roomBackflowValve","SELENOID",roomBackflowValvePin,false);
    Actuator roomFinFan("roomFinFan","FAN",roomFinFanPin,false);
    Actuator roomCircFan("roomCircFan","FAN",roomCircFanPin,false);
    
    //instatialize ZoneGroups
    floorZone = {
    name: "Floor", //string
    targetTemp: floorTargetTemp, //float value
    lowTemp: floorTargetTemp - defaultThreshold, //float value
    highTemp: floorTargetTemp + defaultThreshold, //float value
    alarmLowTemp: floorTargetTemp - (2 * defaultThreshold), //float value
    alarmHighTemp: floorTargetTemp + (2 * defaultThreshold), //float value
    controlValve: &floorControlValve, //Actuator
    backflowValve: &floorBackflowValve, //Actuator
    circFan: NULL, //Actuator
    finFan: NULL, //Actuator
    finTemp: NULL, //Sensor
    returnWaterTemp: &floorReturnTemp, //Sensor
    zoneTemp: &floorZoneTemp, //Sensor
        //   waterFlowTimer: &floorTimer, //Timer
    waterFlowOpen: false, //bool
    toggleWaterFlow: false, //bool
    zoneControlOn: true, //bool
    alarmState: false,
    alarmMessage: "empty"
    };
    
    cabinetZone = {
    name: "Cabinet", //string
    targetTemp: cabinetTargetTemp, //float value
    lowTemp: cabinetTargetTemp - defaultThreshold, //float value
    highTemp: cabinetTargetTemp + defaultThreshold, //float value
    alarmLowTemp: cabinetTargetTemp - (2 * defaultThreshold), //float value
    alarmHighTemp: cabinetTargetTemp + (2 * defaultThreshold), //float value
    controlValve: &cabinetControlValve, //Actuator
    backflowValve: &cabinetBackflowValve, //Actuator
    circFan: NULL, //Actuator
    finFan: &cabinetFinFan, //Actuator
    finTemp: &cabinetFinTemp, //Sensor
    returnWaterTemp: &cabinetReturnTemp, //Sensor
    zoneTemp: &cabinetZoneTemp, //Sensor
        //   waterFlowTimer: &cabinetTimer, //Timer
    waterFlowOpen: false, //bool
    toggleWaterFlow: false, //bool
    zoneControlOn: true, //bool
    alarmState: false,
    alarmMessage: "empty" //bool
    };
    
    roomZone = {
    name: "Room", //string
    targetTemp: roomTargetTemp, //float value
    lowTemp: roomTargetTemp - defaultThreshold, //float value
    highTemp: roomTargetTemp + defaultThreshold, //float value
    alarmLowTemp: roomTargetTemp - (2 * defaultThreshold), //float value
    alarmHighTemp: roomTargetTemp + (2 * defaultThreshold), //float value
    controlValve: &roomControlValve, //Actuator
    backflowValve: &roomBackflowValve, //Actuator
    circFan: &roomCircFan, //Actuator
    finFan: &roomFinFan, //Actuator
    finTemp: &roomFinTemp, //Sensor
    returnWaterTemp: &roomReturnTemp, //Sensor
    zoneTemp: &roomZoneTemp, //Sensor
        //   waterFlowTimer: &roomTimer, //Timer
    waterFlowOpen: false, //bool
    toggleWaterFlow: false, //bool
    zoneControlOn: true, //bool
    alarmState: false,
    alarmMessage: "empty" //bool
    };
    
    //set member functions
    _zonesGroups[0] = &floorZone;
    _zonesGroups[1] = &cabinetZone;
    _zonesGroups[2] = &roomZone;
    _zoneCount = 3;
    _secondaryPump = secondaryPump;
    _secondaryPumpBypassValve = secondaryPumpBypassValve;
    _waterSupplyTemp = waterSupplyTemp;
    _systemRunning = false;
    _alarmState = false;
    _runningTime = 0;
    _sensorArray[0] = &floorZoneTemp;
    _sensorArray[1] = &floorReturnTemp;
    _sensorArray[2] = &cabinetZoneTemp;
    _sensorArray[3] = &cabinetFinTemp;
    _sensorArray[4] = &cabinetReturnTemp;
    _sensorArray[5] = &roomZoneTemp;
    _sensorArray[6] = &roomFinTemp;
    _sensorArray[7] = &roomReturnTemp;
    _sensorArray[8] = &waterSupplyTemp;
    _sensorCount = 9;
    
    floorZoneTemp.setValue(floorTargetTemp - 10);
    cabinetZoneTemp.setValue(cabinetTargetTemp -10);
    roomZoneTemp.setValue(roomTargetTemp - 10);
    
    //compare sensor values to system settings and initialize proper actuator position && system values

}

HydronicSystemMessage HydronicSystemObject::getSystemStatus(){
    //stringify all memeber objects
}
void HydronicSystemObject::updateSystem(){
    //getSensorValues
    for(int i=0;i<_sensorCount;i++){
        //get new value from sensor
        
        //_sensorArray[i]->read();
        _mimicSensorRead(i);
        
        //compare sensor value to system temps
        //update zoneGroups to reflect all updates of system
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
void HydronicSystemObject::editSettings(){}

#endif /* HydronicSystemObject_h */
