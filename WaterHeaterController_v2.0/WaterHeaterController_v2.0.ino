#include <TempSensorMonitor.h>
#include <Sensor.h>
#include <JC_Button.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(52, 50, 48, 46, 44, 42);

const byte menu_button_pin = 32;
const byte warning_button_pin = 34;

const byte floor_led_pin = 22;
const byte floor_warning_led_pin = 23;

const byte cabinet_led_pin = 24;
const byte cabinet_warning_led_pin = 25;

const byte room_led_pin = 26;
const byte room_warning_led_pin = 27;

const byte up_button_pin = 53;
const byte down_button_pin = 51;

const byte speaker_pin = 28;

const unsigned long LONG_PRESS(1000); //define a "long press" to be 1000 milliseconds

TempSensorMonitor floor_monitor("Floor Temp", "Floor", floor_led_pin, 88.00);
TempSensorMonitor cabinet_monitor("Cabinet Temp", "Cab", cabinet_led_pin, 65.00);
TempSensorMonitor room_monitor("Room Temp", "Room", room_led_pin, 45.00);

TempSensorMonitor * monitor_arr[] = {&floor_monitor, &cabinet_monitor, &room_monitor};
int monitor_arr_length = 3;

String monitor_display_names = floor_monitor.GetDisplayName() + " " +
                               cabinet_monitor.GetDisplayName() + " " +
                               room_monitor.GetDisplayName();

Button upButton(up_button_pin,25,false,false);
Button downButton(down_button_pin,25,false,false);
Button menuButton(menu_button_pin,25,false,false);
Button warningButton(warning_button_pin,25,false,false);

unsigned long time_delay = 880; //milliseconds of delay to check time
unsigned long last_time_check = 0; //time at instance
unsigned long current_time = 0;

enum states {WELCOME, FLOOR, FLOOR_EDIT, CAB, CAB_EDIT, ROOM, ROOM_EDIT, MULTI};  // states for the state machine
static states CURRENT_STATE = WELCOME; // current state machine state
static states DISPLAYED_STATE = WELCOME;

enum modes {NORMAL, EDIT};
static modes CURRENT_MODE;

String displayLine1;
String displayLine2;

void setup() {
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Heating Control");
  lcd.setCursor(0, 1);
  lcd.print("Simulation v1.0");
  
  upButton.begin();
  downButton.begin();
  menuButton.begin();
  warningButton.begin();

  
  pinMode(floor_led_pin, OUTPUT);
  pinMode(floor_warning_led_pin, OUTPUT);
  pinMode(cabinet_led_pin, OUTPUT);
  pinMode(cabinet_warning_led_pin, OUTPUT);
  pinMode(room_led_pin, OUTPUT);
  pinMode(room_warning_led_pin, OUTPUT);
}

void turnOffHeat(int index) {
//  Serial.println("Heat OFF");
  digitalWrite(monitor_arr[index]->GetPin(), LOW);
  monitor_arr[index]->SetCallingForHeat(false);
}

void turnOnHeat(int index) {
//  Serial.println("Heat ON");
  digitalWrite(monitor_arr[index]->GetPin(), HIGH);
  monitor_arr[index]->SetCallingForHeat(true);
}

void checkAllMonitors(){
  for (int i = 0; i < monitor_arr_length; i++) {
    monitor_arr[i]->Update();
 //   Serial.println(monitor_arr[i]->ToStringShort());
    if (monitor_arr[i]->IsCallingForHeat() && monitor_arr[i]->stateChanged)
      turnOnHeat(i);
    else if (!monitor_arr[i]->IsCallingForHeat() && monitor_arr[i]->stateChanged) {
      turnOffHeat(i);
    }//else if
  }//for
}//checkAllMonitors

void updateLCD(){
  if(CURRENT_STATE != WELCOME){
    if(CURRENT_STATE != DISPLAYED_STATE){
      //state changed
      //update first line of lcd
      lcd.clear();
      lcd.setCursor(0,0);
      
      Serial.println("UPDATE LCD state change: "+displayLine1);
      lcd.print(displayLine1);
      DISPLAYED_STATE = CURRENT_STATE;
    }
    
    if(CURRENT_STATE == MULTI){
        displayLine2 = (String)floor_monitor.GetReadingTemp()+" "+
                       (String)cabinet_monitor.GetReadingTemp()+" "+
                       (String)room_monitor.GetReadingTemp();
    }//else{
    //  displayLine2 = (String)monitor_arr[CURRENT_STATE]->GetReadingTemp()
    //                + " HOLD"+ (String)monitor_arr[CURRENT_STATE]->GetTargetTemp();
    //}
      lcd.setCursor(0,1);
      lcd.print(displayLine2);
    
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  upButton.read();
  downButton.read();
  menuButton.read();
  warningButton.read();

  current_time = millis();
  if((current_time - last_time_check)>= time_delay){
    last_time_check = current_time;
    checkAllMonitors();
    updateLCD();
  }
  
  switch(CURRENT_STATE){
    case WELCOME:
      if(menuButton.wasReleased()){
        Serial.println("menu was pressed WELCOME->FLOOR");
        CURRENT_STATE = FLOOR;
        displayLine1 = floor_monitor.GetName();
        displayLine2 = floor_monitor.GetReadingTemp();
      }
      else if(upButton.wasReleased()){
        Serial.println("up was pressed WELCOME");        
      }
      else if(downButton.wasReleased()){
        Serial.println("down was pressed WELCOME");
      }
      else if(warningButton.wasReleased()){
        Serial.println("warning was pressed WELCOME");
      }
      break;
    case FLOOR:
     displayLine2 = floor_monitor.GetReadingTemp();
     if(menuButton.pressedFor(LONG_PRESS)){
          CURRENT_STATE = FLOOR_EDIT;
     }else if(menuButton.wasReleased()){
        Serial.println("menu was released FLOOR->CAB");
        CURRENT_STATE = CAB;
        displayLine1 = cabinet_monitor.GetName();   
        displayLine2 = cabinet_monitor.GetReadingTemp(); 
      }
     /*
     if(menuButton.wasReleased()){
        Serial.println("menu was released FLOOR->CAB");
        CURRENT_STATE = CAB;
        displayLine1 = cabinet_monitor.GetName();   
        displayLine2 = cabinet_monitor.GetReadingTemp();    
     }else if(menuButton.pressedFor(LONG_PRESS)){
        CURRENT_STATE = FLOOR_EDIT;
      
      }
      */else if(upButton.wasReleased()){
        Serial.println("up was released FLOOR");
        
      }
      else if(downButton.wasReleased()){
        Serial.println("down was released FLOOR");
      }
      else if(warningButton.wasReleased()){
        Serial.println("warning was released FLOOR");
      }
      break;
    case FLOOR_EDIT:
      if(menuButton.wasReleased()){
        displayLine1 = floor_monitor.GetName()+" EDIT";
        displayLine2 = "HOLD @ "+ (String)floor_monitor.GetTargetTemp();
      
        Serial.println("menu was released FLOOR EDIT");
      }else if(menuButton.wasPressed()){
        Serial.println("menu was pressed FLOOR EDIT");
        CURRENT_STATE = FLOOR;
      }else if(upButton.wasReleased()){
        Serial.println("FLOOR EDIT INC one");
        floor_monitor.SetTargetTemp(floor_monitor.GetTargetTemp()+1);
        displayLine2 = "HOLD @ "+ (String)floor_monitor.GetTargetTemp();

      }else if(downButton.wasReleased()){
        Serial.println("FLOOR EDIT DEC one");
        floor_monitor.SetTargetTemp(floor_monitor.GetTargetTemp()-1);
        displayLine2 = "HOLD @ "+ (String)floor_monitor.GetTargetTemp();

      }else if(warningButton.wasReleased()){
        Serial.println("FLOOR EDIT warning released");
      }
      break;
    case CAB:
      displayLine2 = cabinet_monitor.GetReadingTemp();    
      if(menuButton.wasReleased()){
        Serial.println("menu was released CAB->ROOM");
        CURRENT_STATE = ROOM;
        //displayLine1 = "Room";
        displayLine1 = room_monitor.GetName(); 
        displayLine2 = room_monitor.GetReadingTemp();     
      }
      else if(upButton.wasReleased()){
        Serial.println("up was released CAB");        
      }
      else if(downButton.wasReleased()){
        Serial.println("down was released CAB");
      }
      else if(warningButton.wasReleased()){
        Serial.println("warning was released CAB");
      }      
      break;
    case ROOM:
      displayLine2 = room_monitor.GetReadingTemp(); 
      if(menuButton.wasReleased()){
        Serial.println("menu was released ROOM->MULTI");
        CURRENT_STATE = MULTI;
        displayLine1 = "Floor Cab   Room";
        displayLine2 = (String)floor_monitor.GetReadingTemp()+" "+
                       (String)cabinet_monitor.GetReadingTemp()+" "+
                       (String)room_monitor.GetReadingTemp();
      }
      else if(upButton.wasReleased()){
        Serial.println("up was released ROOM");        
      }
      else if(downButton.wasReleased()){
        Serial.println("down was released ROOM");
      }
      else if(warningButton.wasPressed()){
        Serial.println("warning was pressed ROOM");
      }      
      break;
    case MULTI:
      if(menuButton.wasReleased()){
        Serial.println("menu was released MULTI->FLOOR");
        CURRENT_STATE = FLOOR;
        //displayLine1 = "Floor";
        displayLine1 = floor_monitor.GetName(); 
        displayLine2 = floor_monitor.GetReadingTemp();       
      }
      else if(upButton.wasReleased()){
        Serial.println("up was released MULTI");        
      }
      else if(downButton.wasReleased()){
        Serial.println("down was released MULTI");
      }
      else if(warningButton.wasReleased()){
        Serial.println("warning was released MULTI");
      }           
      break;
  }

}
