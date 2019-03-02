//
//  RelayHandler.h
//  
//
//  Created by Joseph Cutro on 2/9/19.
//

#ifndef RelayHandler_h
#define RelayHandler_h
#include <Relay.h>

class RelayHandler{
private:
    const bool PROGMEM _inverseByDefault = true;
    const bool PROGMEM _powerOnByDefault = false;
    const byte PROGMEM _relayMaxCount = 24;
    byte _relayCount;
    Relay _relayArray[24];
    RelayMessage _output[24];
public:
    RelayHandler(){_relayCount=0;}
    int relayCount(){return _relayCount;}
    void initiate();
    void addRelay(String, byte);
    RelayMessage* getAllRelayStatus();
    RelayMessage getRelayStatus(byte);
    void editSettings(RelayMessage);
    void toggleByPin(byte pin){
        digitalWrite(pin,!digitalRead(pin));
    }
};

void RelayHandler::initiate(){
    _relayCount =0;
    
    addRelay("TableSaw",relay8Pin);
    addRelay("Elevator",relay9Pin);
    addRelay("Music",relay10Pin);
    addRelay("Shop Lights",relay11Pin);
    addRelay("Garage Lights",relay12Pin);
    addRelay("Dust Collector",relay13Pin);
    addRelay("Air Compressor",relay14Pin);
    
    
    for(int i=0;i<_relayCount;i++){
        pinMode(relayFirstPin+i,OUTPUT);
        digitalWrite(relayFirstPin+i,(_powerOnByDefault)? HIGH:LOW);
    }
    /*
    pinMode(relay1Pin,OUTPUT);
    pinMode(relay2Pin,OUTPUT);
    pinMode(relay3Pin,OUTPUT);
    pinMode(relay4Pin,OUTPUT);
    pinMode(relay5Pin,OUTPUT);
    pinMode(relay6Pin,OUTPUT);
    pinMode(relay7Pin,OUTPUT);
    digitalWrite(relay1Pin,(_powerOnByDefault)? HIGH:LOW);
    digitalWrite(relay2Pin,(_powerOnByDefault)? HIGH:LOW);
    digitalWrite(relay3Pin,(_powerOnByDefault)? HIGH:LOW);
    digitalWrite(relay4Pin,(_powerOnByDefault)? HIGH:LOW);
    digitalWrite(relay5Pin,(_powerOnByDefault)? HIGH:LOW);
    digitalWrite(relay6Pin,(_powerOnByDefault)? HIGH:LOW);
    digitalWrite(relay7Pin,(_powerOnByDefault)? HIGH:LOW);
     */
}

void RelayHandler::addRelay(String name, byte pin){
    _relayArray[_relayCount] = Relay(name,pin,_powerOnByDefault,_inverseByDefault);
    _relayCount++;
}

RelayMessage* RelayHandler::getAllRelayStatus(){
    for(int i=0;i<_relayCount;i++){
        _output[i].index = i;
        _output[i].name = _relayArray[i].getName();
        _output[i].pin = _relayArray[i].getPin();
        _output[i].powerOn = _relayArray[i].isPowerOn();
        _output[i].isInverse = _relayArray[i].isInverse();
        _output[i].lastChanged = _relayArray[i].lastChanged();
        //_output[i].hasChanged = _relayArray[i].hasChanged();
    }
    return _output;
}

RelayMessage RelayHandler::getRelayStatus(byte index){
    return RelayMessage{
    index: index,
    name: _relayArray[index].getName(),
    pin: _relayArray[index].getPin(),
    powerOn: _relayArray[index].isPowerOn(),
    isInverse: _relayArray[index].isInverse(),
    lastChanged: _relayArray[index].lastChanged(),
    //hasChanged: _relayArray[index].hasChanged()
    };
}

void RelayHandler::editSettings(RelayMessage newData){
    Serial.println("RELAYHANDLER::editSettings()");
    Serial.println(newData.name);
    Serial.println("Index: "+(String)newData.index);
    Serial.println("Pin: "+(String)newData.pin);
    Serial.print("PowerOn: ");
    Serial.println((newData.powerOn)?"On":"Off");
    Serial.println("isInverse: "+(String)newData.isInverse);
    Serial.println("lastChanged: "+(String)newData.lastChanged);
    Serial.println();
    _relayArray[newData.index].setPowerOn(newData.powerOn);
    //_relayArray[newData.index].lastChanged
}
    

    

#endif /* RelayHandler_h */
