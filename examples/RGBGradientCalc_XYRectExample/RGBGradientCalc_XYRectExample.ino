#include <FastLED.h>
#include "SSVRGBGradientCalc.h"

// Params for width and height
const uint8_t kMatrixWidth = 16;
const uint8_t kMatrixHeight = 16;

#define LED_PIN  D8 //FastLED control pin
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS  (kMatrixWidth * kMatrixHeight)
CRGB leds[NUM_LEDS];

// Param for different pixel layouts, see XYMatrix example for FastLED library
const bool    kMatrixSerpentineLayout = true;
const bool    kMatrixVertical = false;

//see XYMatrix example for FastLED library
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  
  if( kMatrixSerpentineLayout == false) {
    if (kMatrixVertical == false) {
      i = (y * kMatrixWidth) + x;
    } else {
      i = kMatrixHeight * (kMatrixWidth - (x+1))+y;
    }
  }

  if( kMatrixSerpentineLayout == true) {
    if (kMatrixVertical == false) {
      if( y & 0x01) {
        // Odd rows run backwards
        uint8_t reverseX = (kMatrixWidth - 1) - x;
        i = (y * kMatrixWidth) + reverseX;
      } else {
        // Even rows run forwards
        i = (y * kMatrixWidth) + x;
      }
    } else { // vertical positioning
      if ( x & 0x01) {
        i = kMatrixHeight * (kMatrixWidth - (x+1))+y;
      } else {
        i = kMatrixHeight * (kMatrixWidth - x) - (y+1);
      }
    }
  }
  return i;
}


void FillGradientRect(int LeftBottomX, int LeftBottomY, int RightTopX, int RightTopY, CRGB LeftBottomColor, CRGB LeftTopColor, CRGB RightBottomColor, CRGB RightTopColor)
{
  RGBGradientCalculator LeftGradient  (LeftBottomColor,  LeftTopColor,  LeftBottomY, RightTopY);
  RGBGradientCalculator RightGradient (RightBottomColor, RightTopColor, LeftBottomY, RightTopY);
  for( byte y = LeftBottomY; y < RightTopY; y++) //
    {
    CRGB cl = LeftGradient.GetRGBGradientColor(y); 
    CRGB cr = RightGradient.GetRGBGradientColor(y);
    fill_gradient_RGB(leds,  XY(LeftBottomX, y), cl, XY(RightTopX, y), cr ); //FastLED gradient function
    }
}


void setup() 
{
  Serial.begin(115200);
  
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(128);
  FastLED.clear ();
  FastLED.show();
  delay(500);
  Serial.println();

  //LeftBottom Point
  int x1 = 5; 
  int y1 = 3;
  
  //RightTop Point
  int x2 = 10; 
  int y2 = 10;
  
  //Colors
  CRGB c1 = CRGB::Blue; 
  CRGB c2 = CRGB::Yellow;
  CRGB c3 = CRGB::Red; 
  CRGB c4 = CRGB::Green;
  //
  FillGradientRect( x1, y1, x2, y2, c1, c2, c3, c4);
  FastLED.show();
}

void loop() 
{
  //FastLED.show();
  //delay(10);
}
