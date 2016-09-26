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

Sensor::Sensor(size_t longAverageBufferSize, size_t shortAverageBufferSize, int analogPin)
  : longAverageBuffer(0)
  , shortAverageBuffer(0)
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
  longAverageBuffer = new CircularBuffer<int>(longAverageBufferSize);

  delete shortAverageBuffer;
  shortAverageBuffer = new CircularBuffer<int>(shortAverageBufferSize);
}

void Sensor::update(unsigned long time)
{
  int v = analogRead(analogPin);

  shortAverageBuffer->push(v);

  timeAccu += time - lastTime;

  if (timeAccu > longAverageThreshold /*&& !is_triggered()*/)
  {
    timeAccu -= longAverageThreshold;
    longAverageBuffer->push(v);
  }
  lastTime = time;

  if (Configuration::getDebugLevel() == 6) {
	  debugCurrent(v);
  } else if (Configuration::getDebugLevel() == 7) {
	  debugCurrent(v - shortAverageBuffer->average());
  }
}

bool Sensor::is_triggered()
{
  if (is_calibrating()) return false;
  
  int v = min(abs(shortAverageBuffer->average() - longAverageBuffer->average()), 1024);

  v = shortAverageBuffer->average() - longAverageBuffer->average();
  if (v < 0) v = 0;
  if (v > 1024) v = 1024;

  bool result = v >= static_cast<int>(Configuration::getTriggerThreshold());

  if (result && Configuration::getDebugLevel() == 3) {
	  debugTriggering(v);
  }

  return result;
}

void Sensor::debugCurrent(int v) {
	Serial.print(F("S"));
	Serial.print(analogPin);
	Serial.print(F(":"));
	// minus printing
	Serial.print((v<0)? '-' :  ' ');
	v = abs(v);

	// pad digits
	int digits = 1;
	if ( v > 10 ) digits = 2;
	if ( v > 100 ) digits = 3;
	for (int i = 1; i<4-digits; i++) Serial.print("0");

	Serial.print(v);
	Serial.print(F("\t"));
}



void Sensor::debugTriggering(int diff) {
	Serial.print(F("Triggered S"));
	Serial.print(analogPin);
	Serial.print(F(": Diff="));
	Serial.print(diff);
	Serial.print(F(" ShortABuf="));
	Serial.print(shortAverageBuffer->average());
	Serial.print(F(" LongABuf="));
	Serial.println(longAverageBuffer->average());

}

void Sensor::debugEndline() {
	// Sensor debug needs a newline
	  if (Configuration::getDebugLevel()==6
	     || Configuration::getDebugLevel()==7 ) {
		  Serial.println("");
		  delay(100);
	  }
}

void Sensor::reset()
{
  longAverageThreshold = Configuration::getLongAverageBufferTime() / longAverageBuffer->bufferSize();

  longAverageBuffer->clear();
  shortAverageBuffer->clear();
}

bool Sensor::is_calibrating()
{
  return longAverageBuffer->currentElementCount() < longAverageBuffer->bufferSize();
}

int Sensor::longAverage()
{
  return min(longAverageBuffer->average(), 1024);
}

int Sensor::shortAverage()
{
  return min(shortAverageBuffer->average(), 1024);
}


