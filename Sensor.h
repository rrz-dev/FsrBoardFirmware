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

#include <stdint.h>

class Sensor
{
public:
  Sensor(size_t idx, size_t longAverageBufferSize, size_t shortAverageBufferSize, int analogPin);
  ~Sensor();

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
  void createBuffer(size_t longAverageBufferSize, size_t shortAverageBufferSize);

private:
  size_t idx;
  CircularBuffer<int>* longAverageBuffer;
  CircularBuffer<int>* shortAverageBuffer;
  unsigned long lastTime;
  unsigned long timeAccu;
  unsigned long longAverageThreshold;
  int analogPin;
  int debug;
};

