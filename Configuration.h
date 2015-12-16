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
#define DEFAULT_LONG_AVERAGE_BUFFER_SIZE	16
#define DEFAULT_SHORT_AVERAGE_BUFFER_SIZE	4
#define EEPROM_VERSION                    0

class Configuration
{
public:
  Configuration();

  static void load();

  static void setDefaults();
  static void storeValues();
  static void printSettings();

  static void setKeyValue(String key, String value);

  static unsigned long getLongAverageBufferTime() { return longAverageBufferTime; }
  static unsigned long getDefaultEndstopMinHighMs() { return defaultEndstopMinHighMs; }
  static uint16_t getTriggerThreshold() { return triggerThreshold; }
  static unsigned long getCalibrationLedDelay() { return calibrationLedDelay; }
  static byte getI2cSlaveAddress() { return i2cSlaveAddress; }

private:
  static void updateEepromFormat(byte version);

private:
  static unsigned long longAverageBufferTime;
  static unsigned long defaultEndstopMinHighMs;
  static uint16_t triggerThreshold;
  static unsigned long calibrationLedDelay;
  static byte i2cSlaveAddress;
};

