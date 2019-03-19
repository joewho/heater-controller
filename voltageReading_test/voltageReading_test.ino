double voltageReading;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
pinMode(7,OUTPUT);
pinMode(6,OUTPUT);
pinMode(A0,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  voltageReading = (double)analogRead(A0)*5/1024;
  Serial.println(voltageReading);
  if((int)voltageReading > 0){
    digitalWrite(7,LOW);
    digitalWrite(6,HIGH);
  }else{
    digitalWrite(7,HIGH);  
    digitalWrite(6,LOW);
  }
}
