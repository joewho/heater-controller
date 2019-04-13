#include <FastLED.h>

FASTLED_USING_NAMESPACE
#define DATA_PIN    3
//#define CLK_PIN   4
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    64
CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

}

void loop() {
  // put your main code here, to run repeatedly:
for(int i=0;i<NUM_LEDS;i++){
  leds[i] = CRGB::White;
  FastLED.show();
  delay(40);
  leds[i] = CRGB::Black;
}
/*
for(int i=NUM_LEDS-1;i<=0;i--){
  
}
*/
}
