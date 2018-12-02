//
//  ZoneController.h
//  
//
//  Created by Joseph Cutro on 12/2/18.
//

#ifndef ZoneController_h
#define ZoneController_h
#include "SensorHandler.h"

class ZoneController{
private:
    String _name;
    int _pumpPin;//output
    int _pumpTempPin;//in
    int _pumpLEDPin;//ouput

    int _supplyTempPin;//in
    int _returnTempPin;//in

    int _flowSensorPin;//in
    int _flowLEDPin;//out

    int _zoneTempPin;//in
    int _zoneManualOffPin;//in
    
    bool _fanAssist;
    int _fanPin; //output pin of the fan motor
    int _fanTempPin; //in pin of the temp sensor turning on/off the fan
    
    unsigned long _pumpOnTimer, _pumpOffTimer;
    
    bool _alarm;
    
public:
    ZoneController();
    
    bool stateChanged;
    
    String getName(){return _name;}
    void setName(String s){_name = s;}
    
    int getPumpPin(){return _pumpPin;}
    void setPumpPin(int i){_pumpPin  = i;}

    int getPumpTempPin(){return _pumpTempPin;}
    void setPumpTempPin(int i){_pumpTempPin  = i;}
    
    int getPumpLEDPin(){return _pumpLEDPin;}
    void setPumpLEDPin(int i){_pumpLEDPin  = i;}
    
    int getSupplyTempPin(){return _supplyTempPin;}
    void setSupplyTempPin(int i){_supplyTempPin  = i;}
    
    int getReturnPin(){return _returnTempPin;}
    void setReturnPin(int i){_returnTempPin  = i;}

    int getFlowSensorPin(){return _flowSensorPin;}
    void setFLowSensorPin(int i){_flowSensorPin  = i;}
    
    int getFlowLEDPin(){return _flowLEDPin;}
    void setFLowLEDPin(int i){_flowLEDPin  = i;}
    
    int getZoneTempPin(){return _zoneTempPin;}
    void setZoneTempPin(int i){_zoneTempPin  = i;}
    
    int getZoneManualOffPin(){return _zoneManualOffPin;}
    void setZoneManualOffPin(int i){_zoneManualOffPin  = i;}
    
    bool getFanAssist(){return _fanAssist;}
    void setFanAssist(bool i){_fanAssist  = i;}
    
    int getFanPin(){return _fanPin;}
    void setFanPin(int i){_fanPin  = i;}
    
    int getFanTempPin(){return _fanTempPin;}
    void setFanTempPin(int i){_fanTempPin  = i;}
    
    float getPumpOnTimer(){return _pumpOnTimer;}
    void setPumpOnTimer(float f){_pumpOnTimer = f;}
    
    float getPumpOffTimer(){return _pumpOffTimer;}
    void setPumpOffTimer(float f){_pumpOffTimer = f;}
    
    bool inAlarm(){return _alarm;}
    void setAlarm(bool b){_alarm = b;}
    
};

ZoneController::ZoneController(){
    _name  = "EMPTY ZONE-CONTROLLER";
    _pumpPin = -1;//output
    _pumpTempPin = -1;//in
    _pumpLEDPin = -1;
    
    _supplyTempPin = -1;//in
    _returnTempPin = -1;//in
    
    _flowSensorPin = -1;//in
    _flowLEDPin = -1;//out
    
    _zoneTempPin = -1;//in
    _zoneManualOffPin = -1;//in
    
    _fanAssist = false;
    _fanPin = -1; //pin of the fan motor
    _fanTempPin = -1; //pin of the temp sensor turning on/off the fan
    
    _pumpOnTimer = -1;
    _pumpOffTimer = -1;
    
    _alarm = false;
}

ZoneController zone1, zone2, zone3;

const byte floorPumpLED = 22;
const byte floorFlowLED = 23;

const byte cabinetPumpLED = 24;
const byte cabinetFlowLED = 25;

const byte roomPumpLED = 26;
const byte roomFlowLED = 27;

//Zone1 sensors
//SensorHandler pumpTemp1("Pump1","Pump1",-1, -1, TEMP,CONTROL,true);//controll cool
//SensorHandler supplyTemp1("Supply1","Supply1",-1, -1, TEMP,READ,false);
//SensorHandler returnTemp1("Return1","Return1",-1, -1, TEMP,READ,false);
SensorHandler zoneTemp1("Zone1","Zone1",floorPumpLED, 88, TEMP,CONTROL,false);
//SensorHandler fanTemp1("Fan1","Fan1",-1,-1,TEMP,CONTROL,true);//control cool
//SensorHandler flowSensor1("Flow1","Flow1",floorFlowLED,-1,FLOW,READ,false);

//Zone2 sensors
//SensorHandler pumpTemp2("Pump2","Pump2",-1, -1, TEMP,CONTROL,true);//controll cool
//SensorHandler supplyTemp2("Supply2","Supply2",-1, -1, TEMP,READ,false);
//SensorHandler returnTemp2("Return2","Return2",-1, -1, TEMP,READ,false);
SensorHandler zoneTemp2("Zone2","Zone2",cabinetPumpLED, 88, TEMP,CONTROL,false);
//SensorHandler fanTemp2("Fan2","Fan2",-1,-1,TEMP,CONTROL,true);//control cool
//SensorHandler flowSensor2("Flow2","Flow2",cabinetFlowLED,-1,FLOW,READ,false);

//Zone 3 sensors
//SensorHandler pumpTemp3("Pump3","Pump3",-1, -1, TEMP,CONTROL,true);//controll cool
//SensorHandler supplyTemp3("Supply3","Supply3",-1, -1, TEMP,READ,false);
//SensorHandler returnTemp3("Return3","Return3",-1, -1, TEMP,READ,false);
SensorHandler zoneTemp3("Zone3","Zone3",roomPumpLED, 88, TEMP,CONTROL,false);
//SensorHandler fanTemp3("Fan3","Fan3",-1,-1,TEMP,CONTROL,true);//control cool
//SensorHandler flowSensor3("Flow3","Flow3",roomFlowLED,-1,FLOW,READ,false);

const int sensorArrLength = 1;
const int zoneArrLength = 3;
SensorHandler * zone1Sensors[1] = {&zoneTemp1};
SensorHandler * zone2Sensors[1] = {&zoneTemp2};
SensorHandler * zone3Sensors[1] = {&zoneTemp3};

ZoneController * zoneArr[3] = {&zone1, &zone2, &zone3};

#endif /* ZoneController_h */
