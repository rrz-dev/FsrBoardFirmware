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

#include "Parameter.h"

#include "Arduino.h"

Parameter::Parameter()
  : type(NONE)
  , valueType(PVT_UNKNOWN)
  , value(0.0)
{
  memset(&stringValue[0], 0, sizeof(stringValue));
}

void Parameter::setType(ParameterType type)
{
  Parameter::type = type;
}

void Parameter::setNumeric(double value)
{
  Parameter::value = value;
  Parameter::valueType = PVT_NUMERIC;
}

void Parameter::setString(const char* value)
{
  Parameter::valueType = PVT_STRING;

  const size_t len = strlen(value);
  strncpy(stringValue, value, len);
}

void Parameter::reset()
{
  type = ParameterType::NONE;
  valueType = PVT_UNKNOWN;
  value = 0.0;
  for (int i = 0; i < 32; i++)
  {
    stringValue[i] = '\0';
  }
}

