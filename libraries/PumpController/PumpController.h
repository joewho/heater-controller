//
//  PumpController.h
//  
//
//  Created by Joseph Cutro on 12/2/18.
//

#ifndef PumpController_h
#define PumpController_h
#include <TempSensorMonitor.h>

const byte floor_pump_led_pin =22;
const byte floor_flow_led_pin =23;

const byte cabinet_pump_led_pin =24;
const byte cabinet_flow_led_pin =25;

const byte room_pump_led_pin =26;
const byte room_flow_led_pin =27;

TempSensorMonitor floor_monitor("Floor Temp", "Floor", floor_pump_led_pin, 88.00);
TempSensorMonitor cabinet_monitor("Cabinet Temp", "Cab", cabinet_pump_led_pin, 65.00);
TempSensorMonitor room_monitor("Room Temp", "Room", room_pump_led_pin, 45.00);

TempSensorMonitor * monitor_arr[] = {&floor_monitor, &cabinet_monitor, &room_monitor};
int monitor_arr_length = 3;

String monitor_display_names = floor_monitor.GetDisplayName() + " " +
cabinet_monitor.GetDisplayName() + " " +
room_monitor.GetDisplayName();

void setupPumps(){
    pinMode(floor_pump_led_pin, OUTPUT);
    pinMode(floor_flow_led_pin, OUTPUT);
    
    pinMode(cabinet_pump_led_pin, OUTPUT);
    pinMode(cabinet_flow_led_pin, OUTPUT);
    
    pinMode(room_pump_led_pin, OUTPUT);
    pinMode(room_flow_led_pin, OUTPUT);
}

#endif /* PumpController_h */
