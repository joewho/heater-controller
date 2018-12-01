int LEDPin = 2;
unsigned long currentTime;
unsigned long lastTimeCheck;
int timeDelay = 250;
bool LEDState = LOW;

void setup() {
  // put your setup code here, to run once:
  
  pinMode(LEDPin,OUTPUT);
}

void loop() {
  currentTime = millis();
  if((currentTime - lastTimeCheck) >= timeDelay){
    lastTimeCheck = currentTime;
    if(LEDState == LOW)
      LEDState = HIGH;
    else
      LEDState = LOW; 
    digitalWrite(LEDPin,LEDState);
    
  }
  // put your main code here, to run repeatedly:
 // digitalWrite(LEDPin,HIGH);
}
