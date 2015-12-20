/*
   FSR Board Firmware - Firmware for PCB doing FSR Sensor tests for
    reprap printers auto bed leveling.
    
    Copyright (C) 2015  Roland "Glatzemann" Rosenkranz

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Color.h"

#include "Arduino.h"

Color::Color(float r, float g, float b)
{
  rgb.r = r;
  rgb.g = g;
  rgb.b = b;
  
  calcHsv();    
}

Color::Color(RGB rgb)
  : rgb(rgb)
{
  calcHsv();
}

Color::Color(HSV hsv)
  : hsv(hsv)
{
  calcRgb();    
}

void Color::calcHsv()
{
    double      min, max, delta;

    min = rgb.r < rgb.g ? rgb.r : rgb.g;
    min = min   < rgb.b ? min   : rgb.b;

    max = rgb.r > rgb.g ? rgb.r : rgb.g;
    max = max   > rgb.b ? max   : rgb.b;

    hsv.v = max;
    delta = max - min;
    if (delta < 0.00001f)
    {
      hsv.s = 0;
      hsv.h = 0; // undefined
      return;
    }
    
    if( max > 0.0f ) 
    {
      hsv.s = (delta / max);
    } 
    else 
    {
      // if max is 0, then r = g = b = 0
      // s = 0, v is undefined
      hsv.s = 0.0;
      hsv.h = NAN;
      return;
    }
    
    if( rgb.r >= max )
    {
      hsv.h = ( rgb.g - rgb.b ) / delta;        // yellow <-> magenta
    }
    else if( rgb.g >= max )
    {
      hsv.h = 2.0f + ( rgb.b - rgb.r ) / delta;  // cyan <-> yellow
    }
    else
    {
      hsv.h = 4.0f + ( rgb.r - rgb.g ) / delta;  // magenta <-> cyan
    }

    hsv.h *= 60.0f;

    if ( hsv.h < 0.0f )
    {
      hsv.h += 360.0f;
    }
}

void Color::calcRgb()
{
  float hh, p, q, t, ff;
  long  i;

  if (hsv.s <= 0.0) 
  {
    rgb.r = hsv.v;
    rgb.g = hsv.v;
    rgb.b = hsv.v;
    return;
  }
    
  hh = hsv.h;
  if (hh >= 360.0f) hh = 0.0;
  hh /= 60.0f;
  i = static_cast<long>(hh);
  ff = hh - i;
  p = hsv.v * (1.0f - hsv.s);
  q = hsv.v * (1.0f - (hsv.s * ff));
  t = hsv.v * (1.0f - (hsv.s * (1.0f - ff)));

  switch(i) 
  {
    case 0:
      rgb.r = hsv.v;
      rgb.g = t;
      rgb.b = p;
      break;
    case 1:
      rgb.r = q;
      rgb.g = hsv.v;
      rgb.b = p;
      break;
    case 2:
      rgb.r = p;
      rgb.g = hsv.v;
      rgb.b = t;
      break;
    case 3:
      rgb.r = p;
      rgb.g = q;
      rgb.b = hsv.v;
      break;
    case 4:
      rgb.r = t;
      rgb.g = p;
      rgb.b = hsv.v;
      break;
    case 5:
    default:
      rgb.r = hsv.v;
      rgb.g = p;
      rgb.b = q;
      break;
  }
}

