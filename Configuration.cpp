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

#include "Configuration.h"

#include <EEPROM.h>

unsigned long Configuration::longAverageBufferTime;
unsigned long Configuration::defaultEndstopMinHighMs;
uint16_t Configuration::triggerThreshold;
unsigned long Configuration::calibrationLedDelay;
byte Configuration::i2cSlaveAddress;

Configuration::Configuration()
{
  setDefaults();
}

void Configuration::load()
{
  bool eepromFormat = EEPROM.read(0) == 'F' && EEPROM.read(1) == 'S' && EEPROM.read(2) == 'R' && EEPROM.read(3) == 'B';
  if (!eepromFormat) 
  {
    storeValues();
    return;
  }
  
  byte version = EEPROM.read(4);

  if (version < EEPROM_VERSION)
  {
    updateEepromFormat(version);
  }

  // 1 byte padding

  int eepromAddress = 6;

  EEPROM.get(eepromAddress, longAverageBufferTime); eepromAddress += sizeof(longAverageBufferTime);
  EEPROM.get(eepromAddress, defaultEndstopMinHighMs); eepromAddress += sizeof(defaultEndstopMinHighMs);
  EEPROM.get(eepromAddress, triggerThreshold); eepromAddress += sizeof(triggerThreshold);
  EEPROM.get(eepromAddress, calibrationLedDelay); eepromAddress += sizeof(calibrationLedDelay);
  EEPROM.get(eepromAddress, i2cSlaveAddress); eepromAddress += sizeof(i2cSlaveAddress);
}

void Configuration::setDefaults()
{
  longAverageBufferTime = 5000;
  defaultEndstopMinHighMs = 500;
  triggerThreshold = 14;
  calibrationLedDelay = 250;
  i2cSlaveAddress = 77;
}

void Configuration::storeValues()
{
  EEPROM.update(0, 'F');
  EEPROM.update(1, 'S');
  EEPROM.update(2, 'R');
  EEPROM.update(3, 'B');
  EEPROM.update(4, EEPROM_VERSION);
  
}

void Configuration::printSettings()
{
  Serial.print("INFO:FSR board configuration version"); Serial.println(EEPROM_VERSION);
  Serial.print("INFO:longAverageBufferTime=");          Serial.println(longAverageBufferTime);
  Serial.print("INFO:defaultEndstopMinHighMs=");        Serial.println(defaultEndstopMinHighMs);  
  Serial.print("INFO:triggerThreshold=");               Serial.println(triggerThreshold);
  Serial.print("INFO:calibrationLedDelay=");            Serial.println(calibrationLedDelay);
  Serial.print("INFO:i2cSlaveAddress=");                Serial.println(i2cSlaveAddress);
}

void Configuration::updateEepromFormat(byte version)
{
  //TODO: implement
}

void Configuration::setKeyValue(String key, String value)
{
  if (key.equalsIgnoreCase("longAverageBufferTime"))
  {
    longAverageBufferTime = atol(value.c_str());
  }
  else if (key.equalsIgnoreCase("defaultEndstopMinHighMs"))
  {
    defaultEndstopMinHighMs = atol(value.c_str());
  }
  else if (key.equalsIgnoreCase("triggerThreshold"))
  {
    triggerThreshold = value.toInt();
  }
  else if (key.equalsIgnoreCase("calibrationLedDelay"))
  {
    calibrationLedDelay = atol(value.c_str());
  }
  else if (key.equalsIgnoreCase("i2cSlaveAddress"))
  {
    i2cSlaveAddress = value.toInt();
  }
}

