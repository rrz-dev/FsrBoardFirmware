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

#include <Arduino.h>
#include "Sensor.h"
#include "SensorLed.h"
#include "Configuration.h"

SensorLed::SensorLed()
	: state(SLS_NONE)
	, currentLed(0)
	, timeAccu(0)
	, lastTime(millis())
{
}

void SensorLed::add(Sensor* sensor, int sensorLedPin)
{
	sensors[sensorCount] = sensor;
	sensorLed[sensorCount] = sensorLedPin;

	sensorCount++;
}

void SensorLed::update(unsigned long time)
{
	switch (state)
	{
	case SLS_NONE:
		for (size_t i = 0; i < sensorCount; i++)
		{
			pinMode(sensorLed[i], OUTPUT);
			analogWrite(sensorLed[i], 0);
		}
    state = SLS_CALIBRATE;
		break;
	case SLS_CALIBRATE:
		if (sensors[0]->is_calibrating() || sensors[1]->is_calibrating() || sensors[2]->is_calibrating())
		{
			unsigned long deltaTime = labs(time - lastTime);
			lastTime = time;
			timeAccu += deltaTime;

			if (timeAccu >= Configuration::getCalibrationLedDelay())
			{
				analogWrite(sensorLed[currentLed], 0);

				timeAccu -= Configuration::getCalibrationLedDelay();
				currentLed++;
				if (currentLed >= sensorCount)
				{
					currentLed = 0;
				}

				analogWrite(sensorLed[currentLed], 255);
			}
		}
		else
		{
			state = SLS_MONITORING;
		}
		break;
	case SLS_MONITORING:
		for (size_t i = 0; i < sensorCount; i++)
		{
			analogWrite(sensorLed[i], sensors[i]->shortAverage());
		}
		break;
	}
}
