//
//  ButtonController2.h
//  
//
//  Created by Joe Cutro on 12/7/18.
//
//

#ifndef ButtonController2_h
#define ButtonController2_h
#include <ButtonListener.h>

struct ButtonOutput{
    String name;
    int pin;
    String action;
    float lastChange;
    bool hasChanged;
};

class ButtonController2{
private:
    int _arrLength;
    ButtonListener _listenerArr[10];
    ButtonOutput _output[5];
public:
    ButtonController2(){_arrLength=0;}
    int arrayLength(){return _arrLength;}
    void initiate(); //begin all buttons of buttonListeners in array
    void listening(); //read all buttons of buttonListeners in array using updateListener
    void addButton(String name, int pin);//create ButtonListener, ButtonTalker and Button from inputs
    //ButtonOutput * getButtonOutputs(); //
    ButtonOutput * getButtonOutputs();
  //  String toString(); //prep string of member values for serial print
  //  String toStringPretty();
    ButtonListener * getListener(int index){
        Serial.println(_listenerArr[index].toString());
        return &_listenerArr[index];
    }
};
#endif /* ButtonController2_h */

void ButtonController2::initiate(){
    pinMode(button1Pin,INPUT);
    pinMode(button2Pin,INPUT);
    pinMode(button3Pin,INPUT);
    pinMode(button4Pin,INPUT);
    addButton("navButton",button1Pin);
    addButton("selectButton",button2Pin);
    addButton("upButton",button3Pin);
    addButton("downButton",button4Pin);

}

void ButtonController2::listening(){
    for(int i=0;i<_arrLength;i++){
        _listenerArr[i].updateListener();
    }
}

void ButtonController2::addButton(String name, int pin){
    //create button and add button listener
    
    //ButtonListener listener(name,&button,pin);
    //Serial.println(listener.toString());
    _listenerArr[_arrLength]= ButtonListener(name,Button(pin,75,false,false),pin);
//    Serial.println("ButtonController-addButton(): ");
//    Serial.println(_listenerArr[_arrLength].toString());
    
    _arrLength++;
    //Serial.println("addButton - _arrLength: "+(String)_arrLength);
}


ButtonOutput * ButtonController2::getButtonOutputs(){
    for(int i=0;i<_arrLength;i++){
        _output[i].name = _listenerArr[i].getName();
        _output[i].pin = _listenerArr[i].getPin();
        _output[i].action = _listenerArr[i].getAction();
        _output[i].lastChange = _listenerArr[i].getLastChange();
        _output[i].hasChanged = _listenerArr[i].hasChanged();
    }
    return _output;
}
/*
String ButtonController2::toStringPretty(){
    String str;
//    str += "ButtonController-toString() listenerArray:\n";
//    str += "arrayLength: "+(String)_arrLength+"\n";
    for(int i=0;i<_arrLength;i++){
        str+= "name:";
        str+= _listenerArr[i].getName();
       // str+= " pin: ";
       // str+= (String)_listenerArr[i].getPin();
        str+= " action:";
        str+= _listenerArr[i].getAction();
    //    str+= " lastChange: ";
    //    str+= (String)_listenerArr[i].getLastChange();
    //    str+= " hasChanged: ";
    //    str+= (String)_listenerArr[i].hasChanged();
        str+= "   ";
        
    }
    str+= "\n";
    return str;
}
String ButtonController2::toString(){
    String str="";
    str += "ButtonController-toString() listenerArray- "+(String)_arrLength+"\n";
    for(int i=0;i<_arrLength;i++){
        str+= "name:";
        str+= _listenerArr[i].getName();
        str+= " pin: ";
        str+= (String)_listenerArr[i].getPin();
        str+= " action:";
        str+= _listenerArr[i].getAction();
        str+= " lastChange: ";
        str+= (String)_listenerArr[i].getLastChange();
        str+= " hasChanged: ";
        str+= (String)_listenerArr[i].hasChanged();
        str+= "\n";
    }
    return str;
}
*/

