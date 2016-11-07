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

#define SENSOR1_ANALOG_PIN      0   // FSR Sensor 1
#define SENSOR2_ANALOG_PIN      1   // FSR Sensor 2
#define SENSOR3_ANALOG_PIN      2   // FSR Sensor 3
#define SENSOR1_LED_PIN         9   // FSR Debug LED Sensor 1 (optional)
#define SENSOR2_LED_PIN         10  // FSR Debug LED Sensor 2 (optional)
#define SENSOR3_LED_PIN         11  // FSR Debug LED Sensor 3 (optional)
#define THERMISTOR_ANALOG_PIN   3   // Thermistor (optional)

#define CALIBRATION_SWITCH_PIN  2   // Switch (optional)

#define ENDSTOP_OUT_PIN         13  // Endstop signal to master arduino
#define ENDSTOP_ALT1_OUT_PIN    7   // first alternate Endstop signal to master arduino (defaults to AUX2_PIN)
#define ENDSTOP_ALT2_OUT_PIN    8   // second alternate Endstop signal to master arduino (defaults to AUX3_PIN)

#define LED_R                   3   // RGB stripe red (mosfet)
#define LED_G                   5   // RGB stripe green (mosfet)
#define LED_B                   6   // RGB stripe blue (mosfet)

#define ALARM_OUT_PIN           4   // Alarm signal to master arduino (optional)

#define AUX1_PIN                -1  // Arduino RST - Not usable from code   - currently unused
#define AUX2_PIN                7   // Arduino D7                           - currently unused
#define AUX3_PIN                8   // Arduino D8                           - currently unused
#define AUX4_PIN                0   // Arduino RX                           - currently unused
#define AUX5_PIN                1   // Arduino TX                           - currently unused
#define AUX6_PIN                12  // Arduino D12                          - currently unused

#define I2C_SDA_PIN             18  // I2C communication SDA to master arduino (optional)
#define I2C_SDL_PIN             19  // I2C communication SDL to master arduino (optional)

