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
  for( byte y = LeftBottomY; y <= RightTopY; y++) //
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
}

void loop() 
{
  uint32_t frame_delay = 5000;

  //FillGradientRect(LeftBottomX, LeftBottomY, RightTopX, RightTopY, LeftBottomColor, LeftTopColor, RightBottomColor, RightTopColor);
  FastLED.clear();
  Serial.println("Frame 1");
  FillGradientRect( 0,  0,  15,  15,  CRGB::Red,    CRGB::Blue,   CRGB::Green,  CRGB::Yellow);
  FastLED.show();
  delay(frame_delay);
  
  FastLED.clear();
  Serial.println("Frame 2");
  FillGradientRect( 1,  1,  6,  6,   CRGB::Red,    CRGB::Blue,   CRGB::Green,  CRGB::Yellow);
  FillGradientRect( 9,  9,  14, 14,  CRGB::Red,    CRGB::Blue,   CRGB::Green,  CRGB::Yellow);
  FillGradientRect( 1,  9,  6,  14,  CRGB::Red,    CRGB::Blue,   CRGB::Green,  CRGB::Yellow);
  FillGradientRect( 9,  1,  14, 6,   CRGB::Red,    CRGB::Blue,   CRGB::Green,  CRGB::Yellow);
  FastLED.show();
  delay(frame_delay);

  FastLED.clear();
  Serial.println("Frame 3");
  FillGradientRect( 0,  0,  1,  1,   CRGB::Red,    CRGB::Blue,   CRGB::Blue,  CRGB::Red);
  FillGradientRect( 2,  2,  4,  4,   CRGB::Red,    CRGB::Blue,   CRGB::Blue,  CRGB::Red);
  FillGradientRect( 5,  5,  8,  8,   CRGB::Red,    CRGB::Blue,   CRGB::Blue,  CRGB::Red);
  FillGradientRect( 9,  9,  15, 15,  CRGB::Red,    CRGB::Blue,   CRGB::Blue,  CRGB::Red);
  FillGradientRect( 0,  9,  4,  15,  CRGB::Red,    CRGB::Blue,   CRGB::Blue,  CRGB::Red);
  FillGradientRect( 9,  0,  15, 4,   CRGB::Red,    CRGB::Blue,   CRGB::Blue,  CRGB::Red);
  FastLED.show();
  delay(frame_delay);
  
  FastLED.clear();
  Serial.println("Frame 4");
  CRGB CenterColor  = CRGB::Red;
  CRGB CornersColor = CRGB::Yellow;
  CRGB MidsidesColor = CRGB::Blue;
  FillGradientRect( 0,  0,  7,  7,   CornersColor,  MidsidesColor, MidsidesColor, CenterColor);
  FillGradientRect( 8,  8,  15, 15,  CenterColor,   MidsidesColor, MidsidesColor, CornersColor);
  FillGradientRect( 0,  8,  7,  15,  MidsidesColor, CornersColor,  CenterColor,   MidsidesColor);
  FillGradientRect( 8,  0,  15, 7,   MidsidesColor, CenterColor,   CornersColor,  MidsidesColor);
  FastLED.show();
  delay(frame_delay);
}

  