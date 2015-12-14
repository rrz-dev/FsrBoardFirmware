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

#include "Sensor.h"
#include "Configuration.h"

Sensor::Sensor(size_t longAverageBufferSize, size_t shortAverageBufferSize, uint16_t triggerThreshold, int analogPin)
  : longAverageBuffer(0)
  , shortAverageBuffer(0)
  , triggerThreshold(triggerThreshold)
  , analogPin(analogPin)
  , timeAccu(0)
{
  createBuffer(longAverageBufferSize, shortAverageBufferSize);
}

Sensor::~Sensor()
{
  delete longAverageBuffer;
  delete shortAverageBuffer;
}

void Sensor::createBuffer(size_t longAverageBufferSize, size_t shortAverageBufferSize)
{
  delete longAverageBuffer;
  longAverageBuffer = new CircularBuffer<uint16_t>(longAverageBufferSize);

  delete shortAverageBuffer;
  shortAverageBuffer = new CircularBuffer<uint16_t>(shortAverageBufferSize);

  longAverageThreshold = LONG_AVERAGE_BUFFER_TIME / longAverageBuffer->bufferSize();
}

void Sensor::update(unsigned long time)
{
  int v = analogRead(analogPin);

  shortAverageBuffer->push(v);

  timeAccu += time - lastTime;

  if (timeAccu > longAverageThreshold )
  {
    timeAccu -= longAverageThreshold;
    longAverageBuffer->push(v);
  }
  lastTime = time;
}

bool Sensor::is_triggered()
{
  uint16_t v = static_cast<uint16_t>(labs(longAverageBuffer->average() - shortAverageBuffer->average()));
  
  return v >= triggerThreshold;
}

void Sensor::reset()
{
  longAverageBuffer->clear();
  shortAverageBuffer->clear();
}

bool Sensor::is_calibrating()
{
  return longAverageBuffer->currentElementCount() < longAverageBuffer->bufferSize();
}

uint16_t Sensor::longAverage()
{
  return longAverageBuffer->average();
}

uint16_t Sensor::shortAverage()
{
  return shortAverageBuffer->average();
}

