
const byte battery1 = 5;
const byte battery2 = 6;
const byte battery3 = 7;
const byte battery4 = 8;
const byte batteryCount = 4;
const int  chargingTimer = 1500; //ms of time to induce charge on battery

byte* batteryBank[] = {battery1,battery2,battery3,battery4};

void setup() {
  // put your setup code here, to run once:
  pinMode(battery1,OUTPUT);
  pinMode(battery2,OUTPUT);
  pinMode(battery3,OUTPUT);
  pinMode(battery4,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  

  for(int i=0;i<batteryCount;i++){
    digitalWrite(batteryBank[i],HIGH);
    delay(chargingTimer);
    digitalWrite(batteryBank[i],LOW);
  }
  
}
