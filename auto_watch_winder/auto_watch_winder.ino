#include <Servo.h>
Servo myServo;

//const int switchPin = 2;
//int switchVal;
const int delayMicro = 600000;
const int delayMillie = 166;
const int beginPos = 0;
const int endPos = 179;
int pos = beginPos;

void setup() {
  myServo.attach(9);
//  pinMode(switchPin,INPUT);
  Serial.begin(9600);
  myServo.write(pos);
  Serial.println("Servo Started at beginPos");
}

void loop() {
  for(pos=beginPos;pos<=endPos;pos++){
    myServo.write(pos);
    delay(delayMillie);
  }
  for(pos=endPos;pos>=beginPos;pos--){
    myServo.write(pos);
    delay(delayMillie);
  }
  
}
