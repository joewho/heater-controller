
#include <TempSensorMonitor.h>
#include <Sensor.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(52, 50, 48, 46, 44, 42);
//lcd corresponding pin #s (4, 6, 11, 12, 13 ,14);

bool firstRun = true;

const int real_temp_pin = 47;

const int next_button_pin = 32;
const int warning_button_pin = 34;

const int floor_led_pin = 22;
const int floor_warning_led_pin = 23;

const int cabinet_led_pin = 24;
const int cabinet_warning_led_pin = 25;

const int room_led_pin = 26;
const int room_warning_led_pin = 27;

const int up_button_pin = 53;
const int down_button_pin = 51;

const int speaker_pin = 28;

TempSensorMonitor floor_monitor("Floor Monitor", "Floor", floor_led_pin, 88.00);
TempSensorMonitor cabinet_monitor("Cabinet Monitor", "Cab", cabinet_led_pin, 65.00);
TempSensorMonitor room_monitor("Room Monitor", "Room", room_led_pin, 45.00);

TempSensorMonitor * monitor_arr[] = {&floor_monitor, &cabinet_monitor, &room_monitor};
int monitor_arr_length = 3;

String monitor_display_names = floor_monitor.GetDisplayName() + " " +
                               cabinet_monitor.GetDisplayName() + " " +
                               room_monitor.GetDisplayName();

int button_reading; //current reading from button
int button_state; //current state of button high or low
int last_button_state; //last state of button
int button_counter; //counter for button pushes

int warning_button_reading;
int warning_button_state;
int last_warning_button_state;
int warning_button_counter;

int up_button_reading;
int up_button_state;
int last_up_button_state;

int down_button_reading;
int down_button_state;
int last_down_button_state;

unsigned long last_debounce_time = 0; //time of button change state
unsigned long last_warning_debounce_time = 0;
unsigned long last_up_debounce_time = 0;
unsigned long last_down_debounce_time = 0;
unsigned long debounce_delay = 50; //milliseconds of delay to read button

unsigned long time_delay = 880; //milliseconds of delay to check time
unsigned long last_time_check = 0; //time at instance
unsigned long current_time = 0;

//unsigned long previous_millis = 0;
//unsigned long current_millis = 0;
//int tone_interval = 15;

unsigned long lastPeriodStart =0;
const int onDuration=1000;
const int periodDuration=1500;

bool floor_alarm_state = false;
bool cabinet_alarm_state = false;
bool room_alarm_state = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Heating Control");
  lcd.setCursor(0, 1);
  lcd.print("Simulation v1.0");
  pinMode(real_temp_pin, INPUT);
  pinMode(next_button_pin, INPUT);
  pinMode(warning_button_pin, INPUT);
  pinMode(floor_led_pin, OUTPUT);
  pinMode(floor_warning_led_pin, OUTPUT);
  pinMode(cabinet_led_pin, OUTPUT);
  pinMode(cabinet_warning_led_pin, OUTPUT);
  pinMode(room_led_pin, OUTPUT);
  pinMode(room_warning_led_pin, OUTPUT);

  button_counter = 0;

}

void turnOffHeat(int index) {
//  Serial.println("Heat OFF");
  digitalWrite(monitor_arr[index]->GetPin(), LOW);
  //   monitor_arr[index].SetCallingForHeat(false);
}

void turnOnHeat(int index) {
//  Serial.println("Heat ON");
  digitalWrite(monitor_arr[index]->GetPin(), HIGH);
  //    monitor_arr[index].SetCallingForHeat(true);
}

void checkTolerance(int index) {
  //TempSensorMonitor recently changed to call for heat
  if (monitor_arr[index]->IsCallingForHeat() && monitor_arr[index]->stateChanged) {
//    Serial.println(monitor_arr[index]->GetName());
//    Serial.println(millis());
    turnOnHeat(index);
  } else {
    //TempSensorMonitor recently changed to stop calling for heat
    if (!monitor_arr[index]->IsCallingForHeat() && monitor_arr[index]->stateChanged) {
//      Serial.println(monitor_arr[index]->GetName());
//      Serial.println(millis());
      turnOffHeat(index);
    }
  }
}

void CheckAllSensors() {
  for (int i = 0; i < monitor_arr_length; i++) {
    monitor_arr[i]->Update();
    //monitor_arr[i].SetReadingTemp(GetNewTemp(i));//get new values from temp sensors
    checkTolerance(i);
  }
}

void DisplayToSerial() {
  //display all monitors in array and temp sensors in array
  //    Serial.println("MONIOR ARRAY");
  for (int i = 0; i < monitor_arr_length; i++) {
    Serial.println(monitor_arr[i]->ToStringShort());
  }

}

String GetMonitorLCDTemps() {
  String txt;
  for (int i = 0; i < monitor_arr_length; i++) {
    txt += (String)monitor_arr[i]->GetReadingTemp() + " ";
  }
  return txt;
}

int GetButtonCounterCase(){
  int mod = button_counter % 4;
  if (mod == 0 && button_counter > 4) button_counter = 0; //reset counter to save memmory
  if(firstRun) return mod;
  return mod+1;
}


void UpdateLCD() {
  if (IWasLastButtonPushed(last_debounce_time)) {
    //output based on current mode of the button pushed
//    int mod = button_counter % 5; //there are 7 diff items to display

    //    Serial.println("button_counter: "+(String)button_counter);
    //    Serial.println("mod: "+(String)mod);

//    if (mod == 0 && button_counter > 5) button_counter = 0; //reset counter to save memmory
    int mod = GetButtonCounterCase();
    String str;
    String tmp;
    switch (mod) {

      case 1:
        str = "In-floor temp";
        tmp = (String)floor_monitor.GetReadingTemp() + " hold->" + floor_monitor.GetTargetTemp();
        break;
      case 2:
        str = "heated cabinet";
        tmp = (String)cabinet_monitor.GetReadingTemp() + " hold->" + cabinet_monitor.GetTargetTemp();
        break;
      case 3:
        str = "room air temp";
        tmp = (String)room_monitor.GetReadingTemp() + " hold->" + room_monitor.GetTargetTemp();
        break;
      case 4:
        str = monitor_display_names;
        tmp = GetMonitorLCDTemps();
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(str);
    lcd.setCursor(0, 1);
    lcd.print(tmp);
  }
}

void UpdateWarningLCD() {
  if (IWasLastButtonPushed(last_warning_debounce_time)) {
    //output based on current mode of the button pushed
    int mod = warning_button_counter % 5; //there are 7 diff items to display

    //    Serial.println("button_counter: "+(String)button_counter);
    //    Serial.println("mod: "+(String)mod);

    if (mod == 0 && warning_button_counter > 5) warning_button_counter = 0; //reset counter to save memmory

    String str;
    String tmp;
    switch (mod) {

      case 1:
        str = "FLOOR TOO HOT!";
        tmp = "";
        stopAlarms();
        
        turnOnFloorAlarm();
        break;
      case 2:
        str = "CABINET TOO HOT!";
        tmp = "";;
        stopAlarms();
        
        turnOnCabinetAlarm();
        break;
      case 3:
        str = "ROOM TOO HOT!";
        tmp = "";
        stopAlarms();
       
         turnOnRoomAlarm();
        break;
      case 4:
        str = "Systems Clear";
        tmp = "";
        stopAlarms();
        break;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(str);
    lcd.setCursor(0, 1);
    lcd.print(tmp);
  }
}

void CheckAllAlarms() {
    if(millis()-lastPeriodStart>=periodDuration){
      lastPeriodStart += periodDuration;
      if(floor_alarm_state) {
        tone(speaker_pin, 550, onDuration);
        //digitalWrite(floor_warning_led_pin, HIGH);
      }
      if(cabinet_alarm_state) {
        tone(speaker_pin, 650, onDuration);
        //digitalWrite(cabinet_warning_led_pin, HIGH);
      }
      if(room_alarm_state) {
        tone(speaker_pin, 750, onDuration);
        //digitalWrite(room_warning_led_pin, HIGH);
      }
      
    }
}
void stopAlarms() {
  digitalWrite(floor_warning_led_pin, LOW);
  digitalWrite(cabinet_warning_led_pin, LOW);
  digitalWrite(room_warning_led_pin, LOW);
  noTone(floor_warning_led_pin);
  floor_alarm_state = false;
  cabinet_alarm_state = false;
  room_alarm_state = false;

}
void turnOnFloorAlarm() {
  floor_alarm_state = true;
  digitalWrite(floor_warning_led_pin, HIGH);
  CheckAllAlarms();
}

void turnOnCabinetAlarm() {
 cabinet_alarm_state = true;
 digitalWrite(cabinet_warning_led_pin, HIGH);
 CheckAllAlarms();


}
void turnOnRoomAlarm() {
  room_alarm_state = true;
  digitalWrite(room_warning_led_pin, HIGH);
  CheckAllAlarms();
}

void RunSimulation() {
  current_time = millis();
  if ((current_time - last_time_check) >= time_delay) {
    last_time_check = current_time;
    CheckAllSensors();
    //      UpdateSerial();
    UpdateLCD();

  }
}

void ButtonReading() {
  if (button_reading != last_button_state) {
    last_debounce_time = millis();
  }

  if ((millis() - last_debounce_time) > debounce_delay) {
    if (button_reading != button_state) {
      button_state = button_reading;

      if (button_state == HIGH) {
        firstRun = false;
        button_counter++;
//        Serial.println("buttonHIGH");
        UpdateLCD();

      }//if button_state ==HIGH
    }//button_reading != button_state
  }// >debounce_delay
  last_button_state = button_reading;
}

void WarningButtonReading() {
  if (warning_button_reading != last_warning_button_state) {
    last_warning_debounce_time = millis();
  }

  if ((millis() - last_warning_debounce_time) > debounce_delay) {
    if (warning_button_reading != warning_button_state) {
      warning_button_state = warning_button_reading;

      if (warning_button_state == HIGH) {
        warning_button_counter++;
        Serial.println("warninButtonHIGH");
        UpdateWarningLCD();

      }//if button_state ==HIGH
    }//button_reading != button_state
  }// >debounce_delay
  last_warning_button_state = warning_button_reading;
}

bool IWasLastButtonPushed(unsigned long myDebounceTime) {
//  Serial.println("myDTime: " + (String)myDebounceTime);
//  Serial.println("last_debounce_time: " + (String)last_debounce_time);
//  Serial.println("last_warning_debounce_time: " + (String)last_warning_debounce_time + "\n");


  if (myDebounceTime == last_debounce_time)
    return (myDebounceTime > last_warning_debounce_time);
  return (myDebounceTime > last_debounce_time);
}

void CheckUpButtonReading(){
  if (up_button_reading != last_up_button_state) {
    last_up_debounce_time = millis();
  }

  if ((millis() - last_up_debounce_time) > debounce_delay) {
    if (up_button_reading != up_button_state) 
     up_button_state = up_button_reading;

      if (up_button_state == HIGH) {
        int lcdScreenState = GetButtonCounterCase();
        int newTargetTemp;
        switch(lcdScreenState){
          //floor temp
          case 1: {
            newTargetTemp  = floor_monitor.GetTargetTemp()+1;
            Serial.println("Floor new temp: "+ (String)newTargetTemp);
            floor_monitor.SetTargetTemp(newTargetTemp);
            Serial.println("Floor GetTargetTemp: "+ (String)floor_monitor.GetTargetTemp());
            //UpdateLCD();
            break;
          }
          //cabinet temp
          case 2: {
            newTargetTemp = cabinet_monitor.GetTargetTemp()+1;
            Serial.println("Cabinet new temp: "+ (String)newTargetTemp);
            cabinet_monitor.SetTargetTemp(newTargetTemp);
            Serial.println("Cabinet GetTargetTemp: "+ (String)cabinet_monitor.GetTargetTemp());            
            //UpdateLCD();            
            break;
          }
          //room temp
          case 3: {
            newTargetTemp = room_monitor.GetTargetTemp()+1;
            Serial.println("Room new temp: "+ (String)newTargetTemp);
            room_monitor.SetTargetTemp(newTargetTemp);
            Serial.println("Room GetTargetTemp: "+ (String)room_monitor.GetTargetTemp());
            
            //UpdateLCD();            
            break;
          }
          default:{
            //do nothing
            break;
        
        }
      }//if button_state ==HIGH
    }//button_reading != button_state
  }// >debounce_delay
  last_up_button_state = up_button_reading;  
}

void CheckDownButtonReading(){
  if (down_button_reading != last_down_button_state) {
    last_down_debounce_time = millis();
  }

  if ((millis() - last_down_debounce_time) > debounce_delay) {
    if (down_button_reading != down_button_state) {
     down_button_state = down_button_reading;

      if (down_button_state == HIGH) {
        int lcdScreenState = GetButtonCounterCase();
        int newTargetTemp;
        switch(lcdScreenState){
          //floor temp
          case 1: {
            newTargetTemp  = floor_monitor.GetTargetTemp()-1;
            Serial.println("Floor new temp: "+ (String)newTargetTemp);
            floor_monitor.SetTargetTemp(newTargetTemp);
            Serial.println("Floor GetTargetTemp: "+ (String)floor_monitor.GetTargetTemp());
            break;
          }
          //cabinet temp
          case 2: {
            newTargetTemp = cabinet_monitor.GetTargetTemp()-1;
            Serial.println("Cabinet new temp: "+ (String)newTargetTemp);
            cabinet_monitor.SetTargetTemp(newTargetTemp);
            Serial.println("Cabinet GetTargetTemp: "+ (String)cabinet_monitor.GetTargetTemp());            
            break;
          }
          //room temp
          case 3: {
            newTargetTemp = room_monitor.GetTargetTemp()-1;
            Serial.println("Room new temp: "+ (String)newTargetTemp);
            room_monitor.SetTargetTemp(newTargetTemp);
            Serial.println("Room GetTargetTemp: "+ (String)room_monitor.GetTargetTemp());
            break;
          }
          default:{
            //do nothing
          }
        }
      }//if button_state ==HIGH
    }//button_reading != button_state
  }// >debounce_delay
  last_down_button_state = down_button_reading;  
}


void loop() {
  // put your main code here, to run repeatedly:
  unsigned long real_temp_reading = digitalRead(real_temp_pin);
  button_reading = digitalRead(next_button_pin);
  warning_button_reading = digitalRead(warning_button_pin);
  up_button_reading = digitalRead(up_button_pin);
  down_button_reading = digitalRead(down_button_pin);

  Serial.println("real_temp_reading: "+(String)real_temp_reading);

  RunSimulation();
  ButtonReading();
  WarningButtonReading();
  CheckUpButtonReading();
  CheckDownButtonReading();
  CheckAllAlarms();
  //lastButtonPushed();


}
