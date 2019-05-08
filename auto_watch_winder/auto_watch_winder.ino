#include <Servo.h>
Servo myServo;

//const int switchPin = 2;
//int switchVal;
 int mspeed = 1500;
const int delayMicro = 600000;
const int delayMillie = 1000;
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
// myServo.write(45);//rotate ccw full speed
// delay(2000);
//myServo.write(90);//stop
// delay(100);
// myServo.write(135);//rotate cw full speed
// delay(2000);
//myServo.write(90);//stop
// delay(100);
myServo.write(89);//rotate ccw slowly
 delay(3000);
myServo.write(90);//stop
 delay(100);
myServo.write(91);//rotate ccw full speed
 delay(3000);
myServo.write(90);//stop
 delay(100);


 
 /*
 //run motor
 mspeed = 1300;
 myServo.writeMicroseconds(mspeed);
 delay(5000);

 //stop motor
 mspeed = 1500;
 myServo.writeMicroseconds(mspeed);
 delay(2500);

 //run motor
 mspeed=3000;
 myServo.writeMicroseconds(mspeed);
 delay(5000);

 //stop motor
 mspeed = 1500;
 myServo.writeMicroseconds(mspeed);
 delay(2500);
 */
}
