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
#include "Configuration.h"

#include <Arduino.h>

Endstop::Endstop()
  : isTriggered(false)
  , triggeredSince(0)
  , triggeredMessage(false)
{
  pinMode(ENDSTOP_OUT_PIN, OUTPUT);
  digitalWrite(ENDSTOP_OUT_PIN, Configuration::getEndstopHighActive() ? HIGH : LOW);
}

void Endstop::update(unsigned long time, bool triggered)
{
  unsigned long deltaTime = labs(time - triggeredSince);

  if (!isTriggered && triggered)
  {
    // endstop is just triggered
    isTriggered = true;
    endstopHigh();
    timeAccu = Configuration::getDefaultEndstopMinHighMs();
  }
  else if (isTriggered && triggered)
  {
    timeAccu = Configuration::getDefaultEndstopMinHighMs();
  }
  
  if (isTriggered)
  {
    timeAccu -= deltaTime;
    // endstop is no longer triggered
    if (timeAccu <= 0)
    {
      isTriggered = false;
      endstopLow();
    }
  }

  triggeredSince = time;
}

void Endstop::endstopHigh()
{
  if (!triggeredMessage)
  {
    Serial.println("INFO:triggering endstop out");
    triggeredMessage = true;
  }
  digitalWrite(ENDSTOP_OUT_PIN, Configuration::getEndstopHighActive() ? LOW : HIGH);
}

void Endstop::endstopLow()
{
  triggeredMessage = false;
  digitalWrite(ENDSTOP_OUT_PIN, Configuration::getEndstopHighActive() ? HIGH : LOW);
}

bool Endstop::is_triggered()
{
  return isTriggered;
}

