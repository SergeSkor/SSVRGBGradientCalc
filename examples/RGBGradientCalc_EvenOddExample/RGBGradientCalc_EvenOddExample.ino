#include <FastLED.h>
#include "SSVRGBGradientCalc.h"

#define LED_PIN  D8 //FastLED control pin

#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS 16

CRGB leds[NUM_LEDS];


CRGB EvenGradientStart = CRGB::Red;
CRGB EvenGradientEnd   = CRGB::Blue;

CRGB OddGradientStart = CRGB::Green;
CRGB OddGradientEnd   = CRGB::Yellow;

RGBGradientCalculator EvenGradient, OddGradient;

//Calculated in setup()
uint16_t EvenMinInd;
uint16_t EvenMaxInd;
uint16_t OddMinInd;
uint16_t OddMaxInd;


void setup() 
{
  Serial.begin(115200);
  
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(128);
  FastLED.clear ();
  FastLED.show();
  delay(500);
  Serial.println();

  //calculate gradient scales
  EvenMinInd = 0;  //always
  OddMinInd = 1;  //always
  if ( (NUM_LEDS % 2) == 0)
    {
    //total number of leds is even
    EvenMaxInd = NUM_LEDS - 2;
    OddMaxInd = NUM_LEDS - 1;
    }
     else
      {
      //total number of leds is odd
      EvenMaxInd =  NUM_LEDS - 1;
      OddMaxInd = NUM_LEDS - 2;
      }

  Serial.print("Min Even Index (always): "); Serial.println(EvenMinInd);
  Serial.print("Max Even Index): "); Serial.println(EvenMaxInd);
  Serial.print("Min Odd Index (always): "); Serial.println(OddMinInd);
  Serial.print("Max Odd Index): "); Serial.println(OddMaxInd);

  EvenGradient.SetScale (EvenGradientStart, EvenGradientEnd,  EvenMinInd, EvenMaxInd); //set Even gradient scale
  OddGradient.SetScale  (OddGradientStart,  OddGradientEnd,   OddMinInd,  OddMaxInd);  //set Odd gradient scale
}

void loop() 
{
  CRGB color;
  for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
    if ( (i % 2) == 0)
      color = EvenGradient.GetRGBGradientColor(i); //current index is even calculator to get color
       else color = OddGradient.GetRGBGradientColor(i); //current index is odd so use odd calculator to get color
    leds[i]=color;
    }
  FastLED.show();

  delay(10000);
}
