#include "Sensor.h"

const byte floorTempSensorPin = 1;
const byte floorPumpPin = 3;
const byte floorLEDPin = 4;

float floorTargetTemp = 88;

Sensor s1("Floor Temp", "TEMP", floorTempSensorPin);

//specific to floor
struct SensorMap{
  Sensor* sensor;
  float target;
  byte actuatorPin;
  byte ledPin;
  byte zoneID;
  byte alarmID;
};

SensorMap concrete{sensor: &s1, 
                  target: floorTargetTemp,
                  actuatorPin: floorPumpPin,
                  ledPin: floorLEDPin,
                  zoneID: -1,
                  alarmID:-1 };
void setup() {
  Serial.begin(9600);
  Serial.println("s1-value: "+(String)s1.getValue());
  s1.read(-1);
  Serial.println("s1-value: "+(String)s1.getValue());
  concrete.sensor->read(-1);
}
void loop() {
  // put your main code here, to run repeatedly:

}
