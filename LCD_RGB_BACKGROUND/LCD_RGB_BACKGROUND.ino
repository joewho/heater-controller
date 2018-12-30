// include the library code:
#include <LiquidCrystal.h>
#define REDLITE 9
#define GREENLITE 8
#define BLUELITE 7

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int brightness = 255;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
lcd.begin(16,2);
lcd.print("RGB 16x2 Display ");
lcd.setCursor(0,1);
lcd.print("Multicolor LCD");

pinMode(REDLITE, OUTPUT);
pinMode(GREENLITE, OUTPUT);
pinMode(BLUELITE, OUTPUT);
 /*   analogWrite(GREENLITE,255);
    analogWrite(REDLITE, 0);
    analogWrite(BLUELITE,255);
    delay(1000);
*/
}

int red,blue,green;
void loop() {
  
  // put your main code here, to run repeatedly:
  red = 255;
  green = 0;
  blue = 0;
  analogWrite(BLUELITE,255- blue);
  for(int i=0;i<255;i++){
    green += 1;
    red -= 1;
    analogWrite(GREENLITE,255 - green);
    analogWrite(REDLITE, 255 - red);
    
    //setBacklight(i,0,255-i);
    delay(15);
  }
  Serial.println("END FIRST FOR LOOP");

  red = 0;
  green = 255;
  blue = 0;
  analogWrite(REDLITE,255);
  for(int i=0;i<255;i++){
    blue += 1;
    green -= 1;
    analogWrite(GREENLITE,255 - green);
    analogWrite(BLUELITE, 255 - blue);
    delay(15);
  }
  
  Serial.println("END SECOND FOR LOOP");


  red = 0;
  green = 0;
  blue = 255;
  analogWrite(GREENLITE,255);
  for(int i=0;i<255;i++){
    red += 1;
    blue -= 1;
    analogWrite(REDLITE,255 - red);
    analogWrite(BLUELITE, 255 - blue);
    delay(15);
  }
    Serial.println("END THIRD FOR LOOP");

}
