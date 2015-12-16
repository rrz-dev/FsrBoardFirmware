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

#include <Wire.h>

#include "Configuration.h"
#include "Endstop.h"
#include "Pins.h"
#include "Sensor.h"
#include "SensorLed.h"
#include "GCodeParser.h"

Sensor sensor[SENSOR_COUNT] = { Sensor(DEFAULT_LONG_AVERAGE_BUFFER_SIZE, DEFAULT_SHORT_AVERAGE_BUFFER_SIZE, TRIGGER_THRESHOLD, SENSOR1_ANALOG_PIN) 
                              , Sensor(DEFAULT_LONG_AVERAGE_BUFFER_SIZE, DEFAULT_SHORT_AVERAGE_BUFFER_SIZE, TRIGGER_THRESHOLD, SENSOR2_ANALOG_PIN)
                              , Sensor(DEFAULT_LONG_AVERAGE_BUFFER_SIZE, DEFAULT_SHORT_AVERAGE_BUFFER_SIZE, TRIGGER_THRESHOLD, SENSOR3_ANALOG_PIN) 
                              } ;
Endstop endstop;
SensorLed sensorLed;
GCodeParser parser;

const int fsrDebugPin[] = { SENSOR1_LED_PIN, SENSOR2_LED_PIN, SENSOR3_LED_PIN };

void setup() 
{
  Wire.begin(I2C_SLAVE_ADDRESS);
  Wire.onReceive(receiveEvent);
  
  Serial.begin(9600);
  while (!Serial) { }

  Serial.println("INFO:Welcome to FSR board V0.9 Firmware V0.75");
    
  pinMode(CALIBRATION_SWITCH_PIN, INPUT_PULLUP);
  
  for (size_t i = 0; i < SENSOR_COUNT; i++) 
  {
	  sensorLed.add(&sensor[i], fsrDebugPin[i]);
  }
}

void loop() 
{
  //
  // update sensors, trigger endstop and handle calibration switch
  //
  bool calibrationSwitch = !digitalRead(CALIBRATION_SWITCH_PIN);
  bool sensorTriggered = false;
  for (size_t i = 0; i < SENSOR_COUNT; i++) 
  {
    if (calibrationSwitch)
    {
      sensor[i].reset();
    }
    sensor[i].update(millis());
    sensorTriggered |= sensor[i].is_triggered();
  }
  endstop.update(millis(), sensorTriggered);

  //
  // update sensor debug led display
  //
  sensorLed.update(millis());
}

void addCommand(Command c)
{
  //TODO: handle GCode commands
}

void receiveEvent(int howMany) 
{
  while (Wire.available() > 0)  //TODO: limit byte reads per loop iteration
  {
    parser.parse( Wire.read(), addCommand );
  }
}

void serialEvent()
{
  //TODO: add I2C
  
  while (Serial.available() > 0)  //TODO: limit byte reads per loop iteration
  {
    parser.parse( Serial.read(), addCommand );
  }
}

