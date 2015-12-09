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
#include "Endstop.h"
#include "Pins.h"
#include "Sensor.h"
#include "SensorLed.h"

Sensor* sensor[SENSOR_COUNT];
Endstop* endstop;
SensorLed* sensorLed;

const int fsrAnalogPin[] = { SENSOR1_ANALOG_PIN, SENSOR2_ANALOG_PIN, SENSOR3_ANALOG_PIN };
const int fsrDebugPin[] = { SENSOR1_LED_PIN, SENSOR2_LED_PIN, SENSOR3_LED_PIN };

void setup() 
{
  pinMode(CALIBRATION_SWITCH_PIN, INPUT_PULLUP);
  
  endstop = new Endstop(DEFAULT_ENDSTOP_MIN_HIGH_MS);

  sensorLed = new SensorLed();
  for (size_t i = 0; i < SENSOR_COUNT; i++) 
  {
    sensor[i] = new Sensor(DEFAULT_LONG_AVERAGE_BUFFER_SIZE, DEFAULT_SHORT_AVERAGE_BUFFER_SIZE, TRIGGER_THRESHOLD, fsrAnalogPin[i]);
	sensorLed->add(sensor[i]);
  }
}

void loop() 
{
  //
  // update sensors and trigger endstop
  //
  for (size_t i = 0; i < SENSOR_COUNT; i++) 
  {
    sensor[i]->update(millis());
    endstop->update(millis(), sensor[i]->is_triggered());
  }

  //
  // update sensor debug led display
  //
  sensorLed->update(millis());

  //
  // handle calibration switch
  //
  if (!digitalRead(CALIBRATION_SWITCH_PIN))
  {
    for (size_t i = 0; i < SENSOR_COUNT; i++)
    {
      sensor[i]->reset();
    }
  }
}

