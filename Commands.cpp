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

#include "Commands.h"
#include "Endstop.h"
#include "Sensor.h"
#include "Configuration.h"

#include <Arduino.h>

void Commands::printDiagnose(Sensor& s0, Sensor& s1, Sensor& s2)
{
  Serial.println(PSTR("INFO:diagnostic information"));
  
  Serial.println(PSTR("INFO:sensor 0"));
  Serial.print(PSTR("INFO:is_triggered: "));
  Serial.print(s0.is_triggered());
  Serial.print(PSTR(" is_calibrating: "));
  Serial.println(s0.is_calibrating());
  Serial.print(PSTR("INFO:short_average: "));
  Serial.print(s0.shortAverage());
  Serial.print(PSTR(" long_average: "));
  Serial.println(s0.longAverage());
  
  Serial.println(PSTR("INFO:sensor 1"));
  Serial.print(PSTR("INFO:is_triggered: "));
  Serial.print(s1.is_triggered());
  Serial.print(PSTR(" is_calibrating: "));
  Serial.println(s1.is_calibrating());
  Serial.print(PSTR("INFO:short_average: "));
  Serial.print(s1.shortAverage());
  Serial.print(PSTR(" long_average: "));
  Serial.println(s1.longAverage());

  Serial.println(PSTR("INFO:sensor 2"));
  Serial.print(PSTR("INFO:is_triggered: "));
  Serial.print(s2.is_triggered());
  Serial.print(PSTR(" is_calibrating: "));
  Serial.println(s2.is_calibrating());
  Serial.print(PSTR("INFO:short_average: "));
  Serial.print(s2.shortAverage());
  Serial.print(PSTR(" long_average: "));
  Serial.println(s2.longAverage());
}

void Commands::printFirmwareInfo()
{
  Serial.println(PSTR("INFO:Welcome to FSR board Firmware v1-RC1"));
}

void Commands::printEndstopStatus(Endstop endstop)
{
  Serial.print(PSTR("INFO:endstop out status: "));
  Serial.println(endstop.is_triggered());
}

void Commands::factorySettings()
{
  Configuration::setDefaults();
  Serial.println(PSTR("INFO:reverted to factory settings, but not stored in EEPROM"));
}

void Commands::storeSettings()
{
  Configuration::storeValues();
  Serial.println(PSTR("INFO:stored settings to EEPROM"));
}

void Commands::printSettings()
{
  Configuration::printSettings();
}

void Commands::setConfigurationValue(const char* k, long v)
{
  Serial.print(PSTR("INFO:set configuration key '"));
  Serial.print(k);
  Serial.print(PSTR("' to value "));
  Serial.println(v);
  Configuration::setKeyValue(k, v);
}

