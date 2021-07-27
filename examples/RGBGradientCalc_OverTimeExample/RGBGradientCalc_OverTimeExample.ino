#include <FastLED.h>
#include "SSVRGBGradientCalc.h"

#define LED_PIN  D8 //FastLED control pin

#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS 16

CRGB leds[NUM_LEDS];

CRGB MinColor = CRGB::Blue;  //color matching to minimum value
CRGB MaxColor = CRGB::Red; //color matching to maximum value

RGBGradientCalculator Gradient1;

uint32_t minV = 0; //minimum value
uint32_t maxV = 1023; //maximum value

accum88 beatrate = 30; //beats per minute!
uint16_t cycles=2; //how many cycles at a time

void setup() 
{
  Serial.begin(115200);
  
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(128);
  FastLED.clear ();
  FastLED.show();
  delay(500);
  Serial.println();

  Gradient1.SetScale (MinColor, MaxColor, minV, maxV); //set gradient1 scale
}

void loop() 
{

  for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
      uint16_t phaseshift = (0x0FFFF / NUM_LEDS * i * cycles); //0-zero, max 0x0FFFF - 360 degree
      uint32_t value = beatsin16(beatrate, minV, maxV, 0, phaseshift); //current sin value
      CRGB color = Gradient1.GetRGBGradientColor(value); //get color matching to current value
      leds[i]=color; //set color
    }
  
  FastLED.show();
  delay(10);
}