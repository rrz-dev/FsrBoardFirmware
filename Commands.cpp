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
  Serial.println(F("INFO:diagnostic information"));
  
  Serial.println(F("INFO:sensor 0"));
  Serial.print(F("INFO:is_triggered: "));
  Serial.print(s0.is_triggered());
  Serial.print(F(" is_calibrating: "));
  Serial.println(s0.is_calibrating());
  Serial.print(F("INFO:short_average: "));
  Serial.print(s0.shortAverage());
  Serial.print(F(" long_average: "));
  Serial.println(s0.longAverage());
  
  Serial.println(F("INFO:sensor 1"));
  Serial.print(F("INFO:is_triggered: "));
  Serial.print(s1.is_triggered());
  Serial.print(F(" is_calibrating: "));
  Serial.println(s1.is_calibrating());
  Serial.print(F("INFO:short_average: "));
  Serial.print(s1.shortAverage());
  Serial.print(F(" long_average: "));
  Serial.println(s1.longAverage());

  Serial.println(F("INFO:sensor 2"));
  Serial.print(F("INFO:is_triggered: "));
  Serial.print(s2.is_triggered());
  Serial.print(F(" is_calibrating: "));
  Serial.println(s2.is_calibrating());
  Serial.print(F("INFO:short_average: "));
  Serial.print(s2.shortAverage());
  Serial.print(F(" long_average: "));
  Serial.println(s2.longAverage());
}

void Commands::printFirmwareInfo()
{
  Serial.println(F("INFO:Welcome to FSR board Firmware v1"));
}

void Commands::printEndstopStatus(Endstop endstop)
{
  Serial.print(F("INFO:endstop out status: "));
  Serial.println(endstop.is_triggered());
}

void Commands::factorySettings()
{
  Configuration::setDefaults();
  Serial.println(F("INFO:reverted to factory settings, but not stored in EEPROM"));
}

void Commands::storeSettings()
{
  Configuration::storeValues();
  Serial.println(F("INFO:stored settings to EEPROM"));
}

void Commands::printSettings()
{
  Configuration::printSettings();
}

void Commands::setConfigurationValue(const char* k, long v)
{
  Serial.print(F("INFO:set configuration key '"));
  Serial.print(k);
  Serial.print(F("' to value "));
  Serial.println(v);
  Configuration::setKeyValue(k, v);
}

