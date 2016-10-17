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
uint16_t Configuration::trigger1Threshold;
uint16_t Configuration::trigger2Threshold;
uint16_t Configuration::trigger3Threshold;
unsigned long Configuration::calibrationLedDelay;
byte Configuration::i2cSlaveAddress;
byte Configuration::coldTemp;
byte Configuration::hotTemp;
byte Configuration::alarmTemp;
byte Configuration::coldR;
byte Configuration::coldG;
byte Configuration::coldB;
byte Configuration::hotR;
byte Configuration::hotG;
byte Configuration::hotB;
byte Configuration::endstopHighActive;
float Configuration::tempNominal;
float Configuration::thermNominal;
unsigned long Configuration::thermBeta;
byte Configuration::thermNumSamples;
byte Configuration::alarmOutEnabled;
byte Configuration::rgbOutEnabled;
byte Configuration::alarmHighActive;
byte Configuration::debugLevel;
  
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
  trigger1Threshold = EEPROMReadInt16(14);
  calibrationLedDelay = EEPROMReadLong(16);
  EEPROM.get(20, i2cSlaveAddress);

  // version 1
  EEPROM.get(21, coldTemp);
  EEPROM.get(22, hotTemp);
  EEPROM.get(23, alarmTemp);
  EEPROM.get(24, coldR);
  EEPROM.get(25, coldG);
  EEPROM.get(26, coldB);
  EEPROM.get(27, hotR);
  EEPROM.get(28, hotG);
  EEPROM.get(29, hotB);  
  EEPROM.get(30, endstopHighActive);

  // version 2
  tempNominal = EEPROMReadFloat(31);
  thermNominal = EEPROMReadFloat(35);
  thermBeta = EEPROMReadLong(39);
  EEPROM.get(43, thermNumSamples);

  // version 4
  EEPROM.get(44, alarmOutEnabled);
  EEPROM.get(45, rgbOutEnabled);
  EEPROM.get(46, alarmHighActive);

  // version 5

  // Byte 47 padding / spare
  trigger2Threshold = EEPROMReadInt16(48);
  trigger3Threshold = EEPROMReadInt16(50);

  printSettings();
}

void Configuration::setDefaults()
{
  longAverageBufferTime = 5000;
  defaultEndstopMinHighMs = 500;
  trigger1Threshold = 14;
  trigger2Threshold = 14;
  trigger3Threshold = 14;
  calibrationLedDelay = 250;
  i2cSlaveAddress = 77;
  coldTemp = 20;
  hotTemp = 120;
  alarmTemp = 160;
  coldR = 0;
  coldG = 0;
  coldB = 255;
  hotR = 255;
  hotG = 0;
  hotB = 0;
  endstopHighActive = 0;
  tempNominal = 25.0f;
  thermNominal = 100000.0f;
  thermBeta = 4267;
  thermNumSamples = 5;
  alarmOutEnabled = 0;
  rgbOutEnabled = 0;
  alarmHighActive = 0;
  debugLevel = 0;
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
  EEPROMUpdateInt16(14, trigger1Threshold);
  EEPROMUpdateLong(16, calibrationLedDelay);
  EEPROM.update(20, i2cSlaveAddress);

  // version 1
  EEPROM.update(21, coldTemp);
  EEPROM.update(22, hotTemp);
  EEPROM.update(23, alarmTemp);
  EEPROM.update(24, coldR);
  EEPROM.update(25, coldG);
  EEPROM.update(26, coldB);
  EEPROM.update(27, hotR);
  EEPROM.update(28, hotG);
  EEPROM.update(29, hotB);
  EEPROM.update(30, endstopHighActive);

  // version 2
  EEPROMUpdateFloat(31, tempNominal);
  EEPROMUpdateFloat(35, thermNominal);
  EEPROMUpdateLong(39, thermBeta);
  EEPROM.update(43, thermNumSamples);

  // version 4
  EEPROM.update(44, alarmOutEnabled);
  EEPROM.update(45, rgbOutEnabled);
  EEPROM.update(46, alarmHighActive);

  // version 5
  // Byte 47 spare/padding
  EEPROMUpdateLong(48, trigger2Threshold);
  EEPROMUpdateLong(50, trigger3Threshold);
}
void Configuration::printSettings()
{
  Serial.print(F("INFO:FSR board configuration version "));Serial.println(EEPROM_VERSION);
  Serial.print(F("INFO:longAverageBufferTime="));          Serial.println(longAverageBufferTime);
  Serial.print(F("INFO:defaultEndstopMinHighMs="));        Serial.println(defaultEndstopMinHighMs);  
  Serial.print(F("INFO:trigger1Threshold="));               Serial.println(trigger1Threshold);
  Serial.print(F("INFO:trigger2Threshold="));               Serial.println(trigger2Threshold);
  Serial.print(F("INFO:trigger3Threshold="));               Serial.println(trigger3Threshold);
  Serial.print(F("INFO:calibrationLedDelay="));            Serial.println(calibrationLedDelay);
  Serial.print(F("INFO:i2cSlaveAddress="));                Serial.println(i2cSlaveAddress);
  Serial.print(F("INFO:coldTemp="));                       Serial.println(coldTemp);
  Serial.print(F("INFO:hotTemp="));                        Serial.println(hotTemp);
  Serial.print(F("INFO:alarmTemp="));                      Serial.println(alarmTemp);
  Serial.print(F("INFO:coldR="));                          Serial.println(coldR);
  Serial.print(F("INFO:coldG="));                          Serial.println(coldG);
  Serial.print(F("INFO:coldB="));                          Serial.println(coldB);
  Serial.print(F("INFO:hotR="));                           Serial.println(hotR);
  Serial.print(F("INFO:hotG="));                           Serial.println(hotG);
  Serial.print(F("INFO:hotB="));                           Serial.println(hotB);
  Serial.print(F("INFO:endstopHighActive="));              Serial.println(endstopHighActive);
  Serial.print(F("INFO:temperatureNominal="));             Serial.println(tempNominal);
  Serial.print(F("INFO:thermistorNominal="));              Serial.println(thermNominal);
  Serial.print(F("INFO:thermistorBeta="));                 Serial.println(thermBeta);
  Serial.print(F("INFO:thermistorNumSamples="));           Serial.println(thermNumSamples);
  Serial.print(F("INFO:alarmOutEnabled="));                Serial.println(alarmOutEnabled);
  Serial.print(F("INFO:rgbOutEnabled="));                  Serial.println(rgbOutEnabled);
  Serial.print(F("INFO:alarmHighActive="));                Serial.println(alarmHighActive);
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
  bool updateVersion = false;
  
  if (version < 1 && EEPROM_VERSION == 1)
  {
    updateVersion = true;
    
    EEPROM.update(21, 20);  // coldTemp
    EEPROM.update(22, 120); // hotTemp
    EEPROM.update(23, 160); // alarmTemp
    EEPROM.update(24, 0);   // coldR
    EEPROM.update(25, 0);   // coldG
    EEPROM.update(26, 255); // coldB
    EEPROM.update(27, 255); // hotR
    EEPROM.update(28, 0);   // hotG
    EEPROM.update(29, 0);   // hotB
    EEPROM.update(30, 0);   // endstopHighActive
  }

  if (version < 2 && EEPROM_VERSION == 2)
  {
    updateVersion = true;

    EEPROMUpdateFloat(31, 25.0f);
    EEPROMUpdateFloat(35, 100000.0f);
    EEPROMUpdateFloat(39, 4267);
    EEPROM.update(40, 5);
  }

  if (version < 3 && EEPROM_VERSION == 3)
  {
    updateVersion = true;
    
    EEPROMUpdateLong(39, 4267);
    EEPROM.update(43, 5);
  }

  if (version < 4 && EEPROM_VERSION == 4)
  {
    updateVersion = true;

    EEPROM.update(44, 0); // alarmOutEnabled
    EEPROM.update(45, 0); // rgbOutEnabled
    EEPROM.update(46, 0); // alarmHighActive
  }

  if (version < 5 && EEPROM_VERSION == 5)
  {
    updateVersion = true;

    EEPROMUpdateLong(48, EEPROMReadInt16(14)); // trigger2threshold
    EEPROMUpdateLong(50, EEPROMReadInt16(14)); // trigger3threshold
  }
  

  if (updateVersion)
  {
    EEPROM.update(4, EEPROM_VERSION); // new version
  }
}

void Configuration::EEPROMUpdateLong(long address, long value)
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

void Configuration::EEPROMUpdateInt16(long address, uint16_t value)
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

void Configuration::EEPROMUpdateFloat(long address, float value)
{
   byte* p = (byte*)(void*)&value;
   for (int i = 0; i < sizeof(value); i++)
   {
       EEPROM.update(address++, *p++);
   }
}

float Configuration::EEPROMReadFloat(long address)
{
   double value = 0.0;
   byte* p = (byte*)(void*)&value;
   for (int i = 0; i < sizeof(value); i++)
   {
       *p++ = EEPROM.read(address++);
   }
   
   return value;
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
    trigger1Threshold = static_cast<uint16_t>(value);
    trigger2Threshold = static_cast<uint16_t>(value);
    trigger3Threshold = static_cast<uint16_t>(value);
  }
  else if (strcasecmp(key,"trigger1Threshold") == 0)
  {
    trigger1Threshold = static_cast<uint16_t>(value);
  }
  else if (strcasecmp(key,"trigger2Threshold") == 0)
  {
    trigger2Threshold = static_cast<uint16_t>(value);
  }
  else if (strcasecmp(key,"trigger3Threshold") == 0)
  {
    trigger3Threshold = static_cast<uint16_t>(value);
  }
  else if (strcasecmp(key,"calibrationLedDelay") == 0)
  {
    calibrationLedDelay = value;
  }
  else if (strcasecmp(key,"i2cSlaveAddress") == 0)
  {
    i2cSlaveAddress = static_cast<byte>(value);
  }
  else if (strcasecmp(key,"coldTemp") == 0)
  {
    coldTemp = static_cast<byte>(value);
  }
  else if (strcasecmp(key,"hotTemp") == 0)
  {
    hotTemp = static_cast<byte>(value);
  }
  else if (strcasecmp(key,"alarmTemp") == 0)
  {
    alarmTemp = static_cast<byte>(value);
  }
  else if (strcasecmp(key,"coldR") == 0)
  {
    coldR = static_cast<byte>(value);
  }
  else if (strcasecmp(key,"coldG") == 0)
  {
    coldG = static_cast<byte>(value);
  }
  else if (strcasecmp(key,"coldB") == 0)
  {
    coldB = static_cast<byte>(value);
  }
  else if (strcasecmp(key,"hotR") == 0)
  {
    hotR = static_cast<byte>(value);
  }
  else if (strcasecmp(key,"hotG") == 0)
  {
    hotG = static_cast<byte>(value);
  }
  else if (strcasecmp(key,"hotB") == 0)
  {
    hotB = static_cast<byte>(value);
  }
  else if (strcasecmp(key,"endstopHighActive") == 0)
  {
    endstopHighActive = static_cast<byte>(value);
  }
  else if (strcasecmp(key,"temperatureNominal") == 0)
  {
    tempNominal = static_cast<float>(value);
  }
  else if (strcasecmp(key,"thermistorNominal") == 0)
  {
    thermNominal = static_cast<float>(value);
  }
  else if (strcasecmp(key,"thermistorBeta") == 0)
  {
    thermBeta = value;
  }
  else if (strcasecmp(key,"thermistorNumSamples") == 0)
  {
    thermNumSamples = static_cast<byte>(value);
  }
  else if (strcasecmp(key,"alarmOutEnabled") == 0)
  {
    alarmOutEnabled = static_cast<byte>(value);
  }
  else if (strcasecmp(key,"rgbOutEnabled") == 0)
  {
    rgbOutEnabled = static_cast<byte>(value);
  }
  else if (strcasecmp(key,"alarmHighActive") == 0)
  {
    alarmHighActive = static_cast<byte>(value);
  }
}

