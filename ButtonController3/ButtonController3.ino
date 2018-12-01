#include <ButtonController.hpp>

int myArr[] = {4, 5, 6};
int arrLength = 3;
const int but1pin = 5;
ButtonController bC = ButtonController();

//bC.addButton("but1",but1pin);

void setup() {
  // put your setup code here, to run once:
   
  Serial.begin(9600);
  Serial.print(bC.getLength());
}

void loop() {
  // put your main code here, to run repeatedly:
 // for(auto i = 0;i <arrLength;i++){
    //Serial.print(myArr[i]);
  //}
}
