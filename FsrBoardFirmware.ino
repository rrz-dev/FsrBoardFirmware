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
#include "RGBLed.h"

Sensor sensor[SENSOR_COUNT] = { Sensor(DEFAULT_LONG_AVERAGE_BUFFER_SIZE, DEFAULT_SHORT_AVERAGE_BUFFER_SIZE, SENSOR1_ANALOG_PIN) 
                              , Sensor(DEFAULT_LONG_AVERAGE_BUFFER_SIZE, DEFAULT_SHORT_AVERAGE_BUFFER_SIZE, SENSOR2_ANALOG_PIN)
                              , Sensor(DEFAULT_LONG_AVERAGE_BUFFER_SIZE, DEFAULT_SHORT_AVERAGE_BUFFER_SIZE, SENSOR3_ANALOG_PIN) 
                              } ;
Endstop endstop;
SensorLed sensorLed;
GCodeParser parser;
Thermistor thermistor;
RGBLed leds;

const int fsrDebugPin[] = { SENSOR1_LED_PIN, SENSOR2_LED_PIN, SENSOR3_LED_PIN };

static boolean alarmOutTriggerMessage = false;

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

  pinMode(ALARM_OUT_PIN, OUTPUT);
  digitalWrite(ALARM_OUT_PIN, Configuration::getAlarmHighActive() ? HIGH : LOW);

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

  // Sensor debug needs a newline
  if (Configuration::getDebugLevel()==6
     || Configuration::getDebugLevel()==7 ) {
	  Serial.println("");
	  delay(100);
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

  leds.thermistor(thermistor.getCurrentTemperature());

  //
  // check alarm temperature
  //
  if (Configuration::getAlarmOutEnabled())
  {
    if (thermistor.getCurrentTemperature() >= Configuration::getAlarmTemp())
    {
      digitalWrite(ALARM_OUT_PIN, Configuration::getAlarmHighActive() ? LOW : HIGH);
      if (Configuration::getDebugLevel() > 0 && !alarmOutTriggerMessage)
      {
        Serial.println("DEBUG:triggering alarm out");
        alarmOutTriggerMessage = true;
      }
    }
    else
    {
      digitalWrite(ALARM_OUT_PIN, Configuration::getAlarmHighActive() ? HIGH : LOW);
      alarmOutTriggerMessage = false;
    }
  }

/*
  Serial.print("INFO: sensor raw debug: ");
  for (int i = 0; i < SENSOR_COUNT; i++)
  {
    Serial.print(sensor[i].shortAverage());
    Serial.print("/");
    Serial.print(sensor[i].longAverage());
    int v = sensor[i].shortAverage() - sensor[i].longAverage();
    Serial.print(" (");
    Serial.print(v);
    Serial.print(") ; ");
  }
  Serial.println();
*/  
}

void handleMCode(Command c)
{
  switch (c.getCommandCode())
  {
    case 111:   // debug
      Configuration::setDebugLevel(c.getParameterValue(P));
      Serial.print("Debuglevel set to ");
      Serial.println(Configuration::getDebugLevel());

      break;
    case 112:   // diagnose
      Commands::printDiagnose(sensor[0], sensor[1], sensor[2], thermistor);
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
    case 921:   // set RGB-LEDs
      switch((int)c.getParameterValue(S)) {
		  case 0:
          leds.off();
				  break;
		  case 1: 
          leds.set(c.getParameterValue(K, 255), c.getParameterValue(V, 255), c.getParameterValue(B, 255));
				  break;
		  case 2: 
          leds.setUseThermistor();
          break;
      }
      break;
    default:
      Commands::unknownCommand();
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

