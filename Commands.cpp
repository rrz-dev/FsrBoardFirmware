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
#include "Configuration.h"

#include <Arduino.h>

void Commands::printFirmwareInfo()
{
  Serial.println("INFO:Welcome to FSR board V0.9 Firmware V0.75");
}

void Commands::printEndstopStatus(Endstop endstop)
{
  Serial.print("INFO:endstop out status: ");
  Serial.println(endstop.is_triggered());
}

void Commands::factorySettings()
{
  Configuration::setDefaults();
  Serial.println("INFO:reverted to factory settings, but not stored in EEPROM");
}

void Commands::storeSettings()
{
  Configuration::storeValues();
  Serial.println("INFO:stored settings to EEPROM");
}

void Commands::printSettings()
{
  Configuration::printSettings();
}

