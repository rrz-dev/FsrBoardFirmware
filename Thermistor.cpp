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

#include "Thermistor.h"
#include "Configuration.h"
#include "Pins.h"

#include "Arduino.h"

#include <math.h>

Thermistor::Thermistor()
  : currentTemp(20.0f)
  , resistance(0)
{
}

void Thermistor::update(unsigned long time)
{
  currentTemp = calc(analogRead(THERMISTOR_ANALOG_PIN));
}

float Thermistor::getCurrentTemperature()
{
  return currentTemp;
}

float Thermistor::getRawResistance()
{
  return resistance;
}

float Thermistor::calc(int rawAdc)
{
  float average = static_cast<float>(map(rawAdc,0,1023,1023,0));

  // convert the value to resistance
  average = 1023.0 / average - 1.0;
  average = 100000.0 / average;                                           // 100000 -> R10 resistor on FSR board
  resistance = average;
 
  float steinhart = average / Configuration::getThermistorNominal();      // (R/Ro)
  steinhart = log(steinhart);                                             // ln(R/Ro)
  steinhart /= Configuration::getThermistorBeta();                        // 1/B * ln(R/Ro)
  steinhart += 1.0 / (Configuration::getTemperatureNominal() + 273.15);   // + (1/To)
  steinhart = 1.0 / steinhart;                                            // Invert
  steinhart -= 273.15;                                                    // convert to C

  return steinhart;
}

