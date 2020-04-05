#ifndef FastLED_RGBGrad_h
#define FastLED_RGBGrad_h

#include <FastLED.h>

/*
 
     There are two approaches to get RGB gradient color value:
       1) Use function getGradientColor(), or
       2) Use class RGBGradientCalculator.
   
     First approach (function) is simpler, does not required instantiate of object, but internally is using set of global variables to avoid re-calculation of scale. 
     This may be recommended only for non-overlapping (single at-a-time) gradients.
     For more complicated cases use second approach (class), which provide isolation of multiple gradient calculations, but requires instantiation of object.
     
    (c) Serge S.
    11/24/2019
 */


//get RGB gradient color functions
CRGB getGradientColor(CRGB startcolor, CRGB endcolor, uint16_t startpos, uint16_t endpos, uint16_t currentpos);



//RGBGradientCalculator Class
class RGBGradientCalculator
{
  public:
    //constructors
	RGBGradientCalculator();
    RGBGradientCalculator(CRGB StartColor, CRGB EndColor, uint16_t StartPos, uint16_t EndPos);
	
    void SetScale(CRGB StartColor, CRGB EndColor, uint16_t StartPos, uint16_t EndPos);
    CRGB GetRGBGradientColor(uint16_t CurrentPos);

  protected:
  CRGB _StartColor;
  CRGB _EndColor;
  uint16_t _StartPos;
  uint16_t _EndPos;
  //
  saccum87 _rdelta87;
  saccum87 _gdelta87;
  saccum87 _bdelta87;
  accum88 _r88;
  accum88 _g88;
  accum88 _b88;
}; //End of RGBGradientCalculator Class

#endif
//END OF FILE
