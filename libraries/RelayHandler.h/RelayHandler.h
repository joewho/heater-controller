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
    byte _relayCount;
    Relay _relayArray[10];
    RelayMessage _output[10];
public:
    RelayHandler(){_relayCount=0;}
    int relayCount(){return _relayCount;}
    void initiate();
    void addRelay(String, byte);
    RelayMessage* getAllRelayStatus();
    RelayMessage getRelayStatus(byte);
    void editSettings(RelayMessage);
};

void RelayHandler::initiate(){
    
    pinMode(relay1Pin,OUTPUT);
    pinMode(relay2Pin,OUTPUT);
    pinMode(relay3Pin,OUTPUT);
    pinMode(relay4Pin,OUTPUT);
    pinMode(relay5Pin,OUTPUT);
    pinMode(relay6Pin,OUTPUT);
    pinMode(relay7Pin,OUTPUT);
    digitalWrite(relay1Pin,LOW);
    digitalWrite(relay2Pin,LOW);
    digitalWrite(relay3Pin,LOW);
    digitalWrite(relay4Pin,LOW);
    digitalWrite(relay5Pin,LOW);
    digitalWrite(relay6Pin,LOW);
    digitalWrite(relay7Pin,LOW);
    
    addRelay("TableSaw",relay1Pin);
    addRelay("Elevator",relay2Pin);
    addRelay("Music",relay3Pin);
    addRelay("Shop Lights",relay4Pin);
    addRelay("Garage Lights",relay5Pin);
    addRelay("Dust Collector",relay6Pin);
    addRelay("Exhaust Fan",relay7Pin);
    
}

void RelayHandler::addRelay(String name, byte pin){
    _relayArray[_relayCount] = Relay(name,pin,false,true);
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
