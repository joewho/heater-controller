//
//  HydronicSystem.h
//  
//
//  Created by Joseph Cutro on 1/19/19.
//

#ifndef HydronicSystem_h
#define HydronicSystem_h

#include "Sensor.h"
#include "Actuator.h"
//#include "Timer.h"
#include "HydronicSystemConstants.h"
#include "HydronicSystemStructs.h"






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

//instatialize pressure Sensors
//Sensor pumpPressure("Pressure", "PRESSURE",mainPumpPressureSensorPin);

//instatialize Actuators
//Actuator mainPump("MainPump","PUMP",mainPumpPin,false);
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

//instatialize Timers

//instatialize ZoneGroups
ZoneGroup floorZone{
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
    zoneControlOn: true //bool
};

ZoneGroup cabinetZone{
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
    zoneControlOn: true //bool
};

ZoneGroup roomZone{
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
    zoneControlOn: true //bool
};

//ZoneGroup * zoneGroupArray[3] = {&floorZone, &cabinetZone, &roomZone};
//int zoneGroupArrayLength = 3;
class HydronicSystem{
private:
    ZoneGroup* _zoneLoops;
    int _zoneCount;
    //Actuator _primaryPump;
    Actuator _secondaryPump;
    Actuator _secondaryPumpBypassValve;
    //Sensor _primaryPressureSensor;
    Sensor _waterSupplyTemp;
    
public:
    bool systemRunning;
    unsigned long runningTime;
    bool alarmState;
    HydronicSystem(){
        _zoneLoops = {&floorZone, &cabinetZone, &roomZone};
        _zoneCount = 3;
        _primaryPump = mainPump;
        _secondaryPump = secondaryPump;
        _secondaryPumpBypassValve = secondaryPumpBypassValve;
        _primaryPressureSensor = pumpPressure;
        _waterSupplyTemp = waterSupplyTemp;
        //save memory
        delete mainPump;
        delete secondaryPump;
        delete secondaryPumpBypassValve;
        delete pumpPressure;
    }
    
    void initialize();
    void stop();
    
}



#endif /* HydronicSystem_h */
