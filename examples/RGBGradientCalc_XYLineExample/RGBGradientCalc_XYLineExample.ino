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


void DrawGradientLine(int x1, int y1, int x2, int y2, CRGB color1, CRGB color2)
{
    //https://ru.wikibooks.org/wiki/%D0%A0%D0%B5%D0%B0%D0%BB%D0%B8%D0%B7%D0%B0%D1%86%D0%B8%D0%B8_%D0%B0%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC%D0%BE%D0%B2/%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%91%D1%80%D0%B5%D0%B7%D0%B5%D0%BD%D1%85%D1%8D%D0%BC%D0%B0
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    if ((deltaX == 0) && (deltaY == 0))  //added to draw point if start and end points are the same
      {
        leds[XY(x1, y1)] = color1; 
        return;
      }
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    //
        leds[XY(x2, y2)] = color2;

    if (deltaX ==0)
      {
        //vert.line, faster
        RGBGradientCalculator GC (color1,  color2,  y1, y2);
        while (y1 != y2)
          {
            leds[XY(x1, y1)] = GC.GetRGBGradientColor(y1); //setPixelColor(x1,y1, GC.GetRGBGradientColor(y1) );
            y1 += signY;
          }
        return;  
      }

    if (deltaY ==0)
      {
        //hor.line, faster
        RGBGradientCalculator GC (color1,  color2,  x1, x2);
        while (x1 != x2)
          {
            leds[XY(x1, y1)] = GC.GetRGBGradientColor(x1); //setPixelColor(x1,y1, GC.GetRGBGradientColor(x1) );
            x1 += signX;
          }
        return;  
      }
    
    //ANY line, but vert and hor are done above, faster
    //gradient by bigger delta, X or Y
    RGBGradientCalculator GC; //create gradient calculator
    if (deltaX > deltaY) GC.SetScale (color1,  color2,  x1, x2); //set scale according to bigger delta
                    else GC.SetScale (color1,  color2,  y1, y2);
    int error = deltaX - deltaY;
    while(x1 != x2 || y1 != y2) 
   {
        //gradient by bigger delta
        if (deltaX > deltaY) leds[XY(x1, y1)] = GC.GetRGBGradientColor(x1);  //setPixelColor(x1,y1, GC.GetRGBGradientColor(x1) ); 
                        else leds[XY(x1, y1)] = GC.GetRGBGradientColor(y1);  //setPixelColor(x1,y1, GC.GetRGBGradientColor(y1) ); 
        
        const int error2 = error * 2;
        //
        if(error2 > -deltaY) 
        {
            error -= deltaY;
            x1 += signX;
        }
        if(error2 < deltaX) 
        {
            error += deltaX;
            y1 += signY;
        }
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

  //Point #1
  int x1 = 5; 
  int y1 = 3;
  CRGB color1 = CRGB::Blue; 

  //Point #2
  int x2 = 10; 
  int y2 = 10;
  CRGB color2 = CRGB::Yellow;

  DrawGradientLine( x1, y1, x2, y2, color1, color2);
  FastLED.show();
}

void loop() 
{
  //FastLED.show();
  //delay(10);
}
