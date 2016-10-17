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

#pragma once

#include "Arduino.h"

#define SENSOR_COUNT						          3
#define DEFAULT_LONG_AVERAGE_BUFFER_SIZE	32
#define DEFAULT_SHORT_AVERAGE_BUFFER_SIZE	4
#define EEPROM_VERSION                    5



class Configuration
{
public:
  Configuration();

  static void load();

  static void setDefaults();
  static void storeValues();
  static void printSettings();
  static void killEEPROM();

  static void setKeyValue(const char* key, long value);

  static unsigned long getLongAverageBufferTime() { return longAverageBufferTime; }
  static unsigned long getDefaultEndstopMinHighMs() { return defaultEndstopMinHighMs; }
  static uint16_t getTrigger1Threshold() { return trigger1Threshold; }
  static uint16_t getTrigger2Threshold() { return trigger2Threshold; }
  static uint16_t getTrigger3Threshold() { return trigger3Threshold; }
  static unsigned long getCalibrationLedDelay() { return calibrationLedDelay; }
  static byte getI2cSlaveAddress() { return i2cSlaveAddress; }
  static byte getColdTemp() { return coldTemp; }
  static byte getHotTemp() { return hotTemp; }
  static byte getAlarmTemp() { return alarmTemp; }
  static byte getColdR() { return coldR; }
  static byte getColdG() { return coldG; }
  static byte getColdB() { return coldB; }
  static byte getHotR() { return hotR; }
  static byte getHotG() { return hotG; }
  static byte getHotB() { return hotB; }
  static boolean getEndstopHighActive() { return endstopHighActive > 0; }
  static unsigned long getThermistorBeta() { return thermBeta; }
  static float getThermistorNominal() { return thermNominal; }
  static float getTemperatureNominal() { return tempNominal; }
  static byte getThermistorNumSamples() { return thermNumSamples; }
  static boolean getAlarmOutEnabled() { return alarmOutEnabled > 0; }
  static boolean getRgbOutEnabled() { return rgbOutEnabled > 0; }
  static boolean getAlarmHighActive() { return alarmHighActive > 0; }
  static byte getDebugLevel() { return debugLevel; }
  static void setDebugLevel( byte level) { debugLevel = level; }
private:
  static void updateEepromFormat(byte version);
  static void EEPROMUpdateLong(long address, long value);
  static long EEPROMReadLong(long address);
  static void EEPROMUpdateInt16(long address, uint16_t value);
  static long EEPROMReadInt16(long address);
  static void EEPROMUpdateFloat(long address, float value);
  static float EEPROMReadFloat(long address);
  
private:
  static byte debugLevel;
  static unsigned long longAverageBufferTime;
  static unsigned long defaultEndstopMinHighMs;
  static uint16_t trigger1Threshold;
  static uint16_t trigger2Threshold;
  static uint16_t trigger3Threshold;
  static unsigned long calibrationLedDelay;
  static byte i2cSlaveAddress;
  static byte coldTemp;
  static byte hotTemp;
  static byte alarmTemp;
  static byte coldR;
  static byte coldG;
  static byte coldB;
  static byte hotR;
  static byte hotG;
  static byte hotB;
  static byte endstopHighActive;
  static float tempNominal;
  static float thermNominal;
  static unsigned long thermBeta;
  static byte thermNumSamples;
  static byte alarmOutEnabled;
  static byte rgbOutEnabled;
  static byte alarmHighActive;
};

