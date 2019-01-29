#include "HydronicSystemConstants.h"
#include "HydronicSystemStructs.h"
#include "Sensor.h"
#include "Actuator.h"
#include "Timer.h"

float floorTargetTemp = 88; //default floor zone target temp
float cabinetTargetTemp = 55; //default cabinet zone target temp
float roomTargetTemp = 60; //default room zone target temp

unsigned long timeDelay = 1000; //milliseconds to use for delay reading of sensors
unsigned long lastTimeCheck = 0;
unsigned long currentTime = 0;

//instatiate temp sensors
Sensor floorTemp("Floor","TEMP",floorTempSensorPin);
Sensor floorReturnTemp("FloorReturn","TEMP",floorReturnTempSensorPin);
Sensor cabinetTemp("Cab","TEMP",cabTempSensorPin);
Sensor cabinetFinTemp("CabFin","TEMP",cabFinTempSensorPin);
Sensor cabinetReturnTemp("CabReturn","TEMP",cabReturnTempSensorPin);
Sensor roomTemp("Room","TEMP",roomTempSensorPin);
Sensor roomFinTemp("RoomFin","TEMP",roomFinTempSensorPin);
Sensor roomReturnTemp("RoomReturn","TEMP",roomReturnTempSensorPin);
Sensor waterSupplyTemp("WaterSupply","TEMP",supplyTempsensorPin);

//initiate pressure sensors
Sensor pumpPressure("Pressure" "PRESSURE",pumpPressureSensorPin);

//group of sensors, actuators, timers, tempature values, boolean values 
//used to manage control of zone temperatures
struct ZoneGroup{
    float targetTemp;
    float lowTemp;
    float highTemp;
    float alarmLowTemp;
    float alarmHighTemp;
    Actuator* controlValve;
    Actuator* backflowValve;
    Actuator* finFan;
    Sensor* finTemp;
    Sensor* returnWaterTemp;
    Sensor* zoneTemp;
    Timer* waterFlowTimer;
    bool waterFlowOpen;
    bool toggleWaterFlow;
    bool zoneControlOn;
};

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
