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

#include "CircularBuffer.h"
#include "Configuration.h"

#include <stdint.h>

class Sensor
{
public:
  Sensor(uint16_t* threshold, int analogPin);

  void update(unsigned long time);
  bool is_triggered();

  void reset();

  bool is_calibrating();

  int longAverage();
  int shortAverage();
  void debugTriggering(int diff);
  void debugPeek(int v, int v2);
  void debugCurrent(int v);
  static void debugEndline();

private:
  uint16_t* triggerThreshold;
  CircularBuffer<int, DEFAULT_SHORT_AVERAGE_BUFFER_SIZE> longAverageBuffer;
  CircularBuffer<int, DEFAULT_LONG_AVERAGE_BUFFER_SIZE> shortAverageBuffer;
  unsigned long lastTime;
  unsigned long timeAccu;
  unsigned long longAverageThreshold;
  int analogPin;
  int debug;
};

