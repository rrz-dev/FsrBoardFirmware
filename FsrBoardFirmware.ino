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

#include <EEPROM.h>
#include <Wire.h>

#include "Configuration.h"
#include "Color.h"
#include "Command.h"
#include "Commands.h"
#include "Endstop.h"
#include "Parameter.h"
#include "Pins.h"
#include "Sensor.h"
#include "SensorLed.h"
#include "Thermistor.h"
#include "GCodeParser.h"

Sensor sensor[SENSOR_COUNT] = { Sensor(DEFAULT_LONG_AVERAGE_BUFFER_SIZE, DEFAULT_SHORT_AVERAGE_BUFFER_SIZE, SENSOR1_ANALOG_PIN) 
                              , Sensor(DEFAULT_LONG_AVERAGE_BUFFER_SIZE, DEFAULT_SHORT_AVERAGE_BUFFER_SIZE, SENSOR2_ANALOG_PIN)
                              , Sensor(DEFAULT_LONG_AVERAGE_BUFFER_SIZE, DEFAULT_SHORT_AVERAGE_BUFFER_SIZE, SENSOR3_ANALOG_PIN) 
                              } ;
Endstop endstop;
SensorLed sensorLed;
GCodeParser parser;
Thermistor thermistor;

const int fsrDebugPin[] = { SENSOR1_LED_PIN, SENSOR2_LED_PIN, SENSOR3_LED_PIN };

void setup() 
{
  Serial.begin(9600);
  while (!Serial) { }

  Commands::printFirmwareInfo();

  //Configuration::killEEPROM();
  Configuration::load();

  Wire.begin(Configuration::getI2cSlaveAddress());
  Wire.onReceive(receiveEvent);
    
  pinMode(CALIBRATION_SWITCH_PIN, INPUT_PULLUP);
  
  for (size_t i = 0; i < SENSOR_COUNT; i++) 
  {
    sensor[i].reset();
	  sensorLed.add(&sensor[i], fsrDebugPin[i]);
  }
}

void loop() 
{
  unsigned long time = millis();
  
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
    sensor[i].update(time);
    sensorTriggered |= sensor[i].is_triggered();
  }
  endstop.update(time, sensorTriggered);

  //
  // update sensor debug led display
  //
  sensorLed.update(time);

  //
  // Thermistor temperature reading and RGB LED output
  //
  thermistor.update(time);

  float lowTemp = Configuration::getColdTemp();
  float highTemp = Configuration::getHotTemp();
  float divider = highTemp - lowTemp;
  float t = (thermistor.getCurrentTemperature() - lowTemp) / divider;

  Color cold(Configuration::getColdR()/255.0f, Configuration::getColdG()/255.0f, Configuration::getColdB()/255.0f);
  Color hot(Configuration::getHotR()/255.0f, Configuration::getHotG()/255.0f, Configuration::getHotB()/255.0f);

  Color rgbLedColor = cold.interpolate(hot, t, &linearF); //TODO: output color as PWM values for RGB leds
  
}

void handleMCode(Command c)
{
  switch (c.getCommandCode())
  {
    case 112:   // diagnose
      Commands::printDiagnose(sensor[0], sensor[1], sensor[2]);
      break;
    case 115:   // get firmware version and capabilities
      Commands::printFirmwareInfo();
      break;
    case 119:   // get endstop status
      Commands::printEndstopStatus(endstop);
      break;
    case 500:   // store parameters in EEPROM
      Commands::storeSettings();
      break;
    case 502:   // revert to the default "factory settings"
      Commands::factorySettings();
      break;
    case 503:   // print settings
      Commands::printSettings();
      break;
    case 800:   // set key value
      Commands::setConfigurationValue(c.getParameterStringValue(K), c.getParameterValue(V));
      break;
  }
}

void handleGCode(Command c)
{
  //TODO: implement
}

void addCommand(Command c)
{
  switch (c.getCommandType())
  {
    case M:
      handleMCode(c);
      break;
    case G:
      handleGCode(c);
      break;
  }
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
  while (Serial.available() > 0)  //TODO: limit byte reads per loop iteration
  {
    parser.parse( Serial.read(), addCommand );
  }
}

