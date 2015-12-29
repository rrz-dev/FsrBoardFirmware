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
  Serial.println(F("INFO:loading eeprom configuration..."));
  
  char eepromFormat[4];
  EEPROM.get(0, eepromFormat);
  bool eepromInitialized = eepromFormat[0] == 'F' && eepromFormat[1] == 'S' && eepromFormat[2] == 'R' && eepromFormat[3] == 'B';
  if (!eepromInitialized)
  {
    Serial.println(F("INFO:eeprom is not initialized. Setting default values..."));
    setDefaults();
    storeValues();
  }
  
  byte version = EEPROM.read(4);

  if (version < EEPROM_VERSION)
  {
    Serial.println(F("INFO:updating old eeprom version..."));
    updateEepromFormat(version);
  }

  // 1 byte padding

  longAverageBufferTime = EEPROMReadLong(6);
  defaultEndstopMinHighMs = EEPROMReadLong(10);
  triggerThreshold = EEPROMReadInt16(14);
  calibrationLedDelay = EEPROMReadLong(16);
  EEPROM.get(20, i2cSlaveAddress);

  printSettings();
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
  EEPROM.update(5, 0);  // padding

  int eepromAddress = 6;

  EEPROMUpdateLong(6, longAverageBufferTime);
  EEPROMUpdateLong(10, defaultEndstopMinHighMs);
  EEPROMUpdateInt16(14, triggerThreshold);
  EEPROMUpdateLong(16, calibrationLedDelay);
  EEPROM.update(20, i2cSlaveAddress);
}

void Configuration::printSettings()
{
  Serial.print(F("INFO:FSR board configuration version "));Serial.println(EEPROM_VERSION);
  Serial.print(F("INFO:longAverageBufferTime="));          Serial.println(longAverageBufferTime);
  Serial.print(F("INFO:defaultEndstopMinHighMs="));        Serial.println(defaultEndstopMinHighMs);  
  Serial.print(F("INFO:triggerThreshold="));               Serial.println(triggerThreshold);
  Serial.print(F("INFO:calibrationLedDelay="));            Serial.println(calibrationLedDelay);
  Serial.print(F("INFO:i2cSlaveAddress="));                Serial.println(i2cSlaveAddress);
}

void Configuration::killEEPROM()
{
  for (int i = 0 ; i < EEPROM.length() ; i++) 
  {
    EEPROM.write(i, 0);
  }
}

void Configuration::updateEepromFormat(byte version)
{
  //TODO: implement
}

void Configuration::EEPROMUpdateLong(int address, long value)
{
  //Write the 4 bytes into the eeprom memory.
  EEPROM.update(address, (value & 0xFF));
  EEPROM.update(address + 1, ((value >> 8) & 0xFF));
  EEPROM.update(address + 2, ((value >> 16) & 0xFF));
  EEPROM.update(address + 3, ((value >> 24) & 0xFF));
}

long Configuration::EEPROMReadLong(long address)
{
  long a = EEPROM.read(address);
  long b = EEPROM.read(address + 1);
  long c = EEPROM.read(address + 2);
  long d = EEPROM.read(address + 3);

  return ((a << 0) & 0xFF) + ((b << 8) & 0xFFFF) + ((c << 16) & 0xFFFFFF) + ((d << 24) & 0xFFFFFFFF);
}      

void Configuration::EEPROMUpdateInt16(int address, uint16_t value)
{
  //Write the 2 bytes into the eeprom memory.
  EEPROM.update(address, (value & 0xFF));
  EEPROM.update(address + 1, ((value >> 8) & 0xFF));
}
  
long Configuration::EEPROMReadInt16(long address)
{
  long a = EEPROM.read(address);
  long b = EEPROM.read(address + 1);

  return ((a << 0) & 0xFF) + ((b << 8) & 0xFFFF);
}

void Configuration::setKeyValue(const char* key, long value)
{
  if (strcasecmp(key,"longAverageBufferTime") == 0)
  {
    longAverageBufferTime = value;
  }
  else if (strcasecmp(key,"defaultEndstopMinHighMs") == 0)
  {
    defaultEndstopMinHighMs = value;
  }
  else if (strcasecmp(key,"triggerThreshold") == 0)
  {
    triggerThreshold = static_cast<uint16_t>(value);
  }
  else if (strcasecmp(key,"calibrationLedDelay") == 0)
  {
    calibrationLedDelay = value;
  }
  else if (strcasecmp(key,"i2cSlaveAddress") == 0)
  {
    i2cSlaveAddress = static_cast<int>(value);
  }
}


