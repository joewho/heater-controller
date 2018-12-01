#include <JC_Button.h>

const byte menu_button_pin = 32;
const byte warning_button_pin = 34;
const byte up_button_pin = 53;
const byte down_button_pin = 51;
const int LONG_PRESS = 1000;
const int MAX_BUFFER = 10;



Button upButton(up_button_pin,25,false,false);
Button downButton(down_button_pin,25,false,false);
Button menuButton(menu_button_pin,25,false,false);
Button warningButton(warning_button_pin,25,false,false);

struct buttonListener{
  String _name;
  Button* button;
  byte button_pin;
  String action;
  String type;
  long  last_change;
};

struct outputMessage{
  String _name;
  byte input_pin;
  String type;
  String reading;
//  bool just_changed;
  long last_change;
};



buttonListener upListener{_name:"UP", button: &upButton, button_pin: up_button_pin};
buttonListener downListener{_name:"DOWN", button: &downButton, button_pin: down_button_pin};
buttonListener menuListener{_name:"MENU", button: &menuButton, button_pin: menu_button_pin};
buttonListener warningListener{_name:"WARNING", button: &warningButton, button_pin: warning_button_pin};

buttonListener* listenerArr[] = {&upListener, &downListener, &menuListener, &warningListener};
const int listenerArrLength = 4;

outputMessage outputAll[listenerArrLength];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  upButton.begin();
  downButton.begin();
  menuButton.begin();
  warningButton.begin();
}

void currentRead(int index){
  String act;
  if(listenerArr[index]->button->isPressed()) act = "isPressed";
  if(listenerArr[index]->button->isPressed()) act = "isPressed";
  if(listenerArr[index]->button->isReleased()) act = "isReleased";
  if(listenerArr[index]->button->wasPressed()) act = "wasPressed";
  if(listenerArr[index]->button->wasReleased()) act = "wasReleased";
  if(listenerArr[index]->button->pressedFor(LONG_PRESS)) act = "pressedFor";
  if(listenerArr[index]->button->releasedFor(LONG_PRESS)) act = "releasedFor";
  
  listenerArr[index]->action = act;
  listenerArr[index]->last_change = listenerArr[index]->last_change;
}


outputMessage listenAll(){

  
  for(int i=0;i<listenerArrLength;i++){
    currentRead(i);
      outputAll[i]._name = listenerArr[i]->_name;
      outputAll[i].input_pin = listenerArr[i]->button_pin;
      outputAll[i].reading = listenerArr[i]->action;
      outputAll[i].type = "button";
      outputAll[i].last_change = listenerArr[i]->last_change;

  
    
 //   Serial.print((String)listenerArr[i]->_name+" "
 //             +(String)listenerArr[i]->action+" "
 //             +(String)listenerArr[i]->last_change +"   ");
  }
  return outputAll;
 // Serial.println();
  
}
void loop() {
  // put your main code here, to run repeatedly:
  upButton.read();
  downButton.read();
  menuButton.read();
  warningButton.read();
  
  outputMessage myOutput[4] = listenAll();

  for(int i=0;i<listenerArrLength;i++){
      Serial.print((String)myOutput[i]._name+" "
                +(String)myOutput[i].reading+" "
                +(String)myOutput[i].last_change+"   ");
  }
  Serial.println();

}
