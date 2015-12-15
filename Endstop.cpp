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

#include "Endstop.h"
#include "Pins.h"

#include <Arduino.h>

Endstop::Endstop(long minimumTriggerTime)
  : isTriggered(false)
  , triggeredSince(0)
  , minimumTriggerTime(minimumTriggerTime)
{
  pinMode(ENDSTOP_OUT_PIN, OUTPUT);
}

void Endstop::update(unsigned long time, bool triggered)
{
  if (!isTriggered && triggered)
  {
    // endstop is just triggered
    isTriggered = true;
    triggeredSince = time;
    endstopHigh();
  }
  else if (isTriggered)
  {
    // endstop is no longer triggered
    if (labs(time - triggeredSince) > minimumTriggerTime)
    {
      isTriggered = false;
      endstopLow();
    }
  }
}

void Endstop::endstopHigh()
{
  digitalWrite(ENDSTOP_OUT_PIN, HIGH);
  //TODO: INVERT!!!
}

void Endstop::endstopLow()
{
  digitalWrite(ENDSTOP_OUT_PIN, LOW);
  //TODO: INVERT!!!
}

