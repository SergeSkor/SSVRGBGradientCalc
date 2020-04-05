#include "SSVRGBGradientCalc.h"
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



//internal variables to avoid sacle recalculation if class is not used, do not change in application...
CRGB gradvar_StartColor;
CRGB gradvar_EndColor;
uint16_t gradvar_StartPos;
uint16_t gradvar_EndPos;
saccum87 gradvar_rdelta87;
saccum87 gradvar_gdelta87;
saccum87 gradvar_bdelta87;
accum88 gradvar_r88;
accum88 gradvar_g88;
accum88 gradvar_b88;

//get RGB gradient color functions
CRGB getGradientColor(CRGB startcolor, CRGB endcolor, uint16_t startpos, uint16_t endpos, uint16_t currentpos)
{
  if ( (startcolor!=gradvar_StartColor)||(endcolor!=gradvar_EndColor)||(startpos!=gradvar_StartPos)||(endpos!=gradvar_EndPos) )
    {
    //Serial.println("Recalc Scale");  //recalculate scale
    //save and calculate scale
    gradvar_StartColor = startcolor;
    gradvar_EndColor = endcolor;
    gradvar_StartPos = startpos;
    gradvar_EndPos = endpos;
    
    saccum87 rdistance87 = (endcolor.r - startcolor.r) << 7;
    saccum87 gdistance87 = (endcolor.g - startcolor.g) << 7;
    saccum87 bdistance87 = (endcolor.b - startcolor.b) << 7;

    uint16_t pixeldistance = endpos - startpos;
    int16_t divisor = pixeldistance ? pixeldistance : 1;

    gradvar_rdelta87 = rdistance87 / divisor;
    gradvar_gdelta87 = gdistance87 / divisor;
    gradvar_bdelta87 = bdistance87 / divisor;

    gradvar_rdelta87 = gradvar_rdelta87 * 2;
    gradvar_gdelta87 = gradvar_gdelta87 * 2;
    gradvar_bdelta87 = gradvar_bdelta87 * 2;

    gradvar_r88 = startcolor.r << 8;
    gradvar_g88 = startcolor.g << 8;
    gradvar_b88 = startcolor.b << 8;
    }
      //else { Serial.println("DO NOT recalc Scale"); }  //do not recalculate scale
    uint16_t posdelta = currentpos-startpos;  
    accum88 r88 = gradvar_r88 + (gradvar_rdelta87 * posdelta);
    accum88 g88 = gradvar_g88 + (gradvar_gdelta87 * posdelta);
    accum88 b88 = gradvar_b88 + (gradvar_bdelta87 * posdelta);
    return CRGB( r88 >> 8, g88 >> 8, b88 >> 8);
}



//RGBGradientCalculator Class
RGBGradientCalculator::RGBGradientCalculator()
{};

RGBGradientCalculator::RGBGradientCalculator(CRGB StartColor, CRGB EndColor, uint16_t StartPos, uint16_t EndPos)
{
  SetScale(StartColor, EndColor, StartPos, EndPos);
}

void RGBGradientCalculator::SetScale(CRGB StartColor, CRGB EndColor, uint16_t StartPos, uint16_t EndPos)
{
  _StartColor = StartColor;
  _EndColor = EndColor;
  _StartPos = StartPos;
  _EndPos = EndPos;
  //
  saccum87 rdistance87 = (_EndColor.r - _StartColor.r) << 7;
  saccum87 gdistance87 = (_EndColor.g - _StartColor.g) << 7;
  saccum87 bdistance87 = (_EndColor.b - _StartColor.b) << 7;
  //
  uint16_t pixeldistance = _EndPos - _StartPos;
  int16_t divisor = pixeldistance ? pixeldistance : 1;
  //
  _rdelta87 = rdistance87 / divisor;
  _gdelta87 = gdistance87 / divisor;
  _bdelta87 = bdistance87 / divisor;
  //
  _rdelta87 = _rdelta87 * 2;
  _gdelta87 = _gdelta87 * 2;
  _bdelta87 = _bdelta87 * 2;
  //
  _r88 = _StartColor.r << 8;
  _g88 = _StartColor.g << 8;
  _b88 = _StartColor.b << 8;
}

CRGB RGBGradientCalculator::GetRGBGradientColor(uint16_t CurrentPos)
{
  uint16_t posdelta = CurrentPos-_StartPos;  
  accum88 r88 = _r88 + (_rdelta87 * posdelta);
  accum88 g88 = _g88 + (_gdelta87 * posdelta);
  accum88 b88 = _b88 + (_bdelta87 * posdelta);
  return CRGB( r88 >> 8, g88 >> 8, b88 >> 8);
}
//End of RGBGradientCalculator Class
