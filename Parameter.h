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

enum ParameterType
{
  NONE = '\0',
  S = 'S',
  P = 'P',
  I = 'I',
  J = 'J',
  D = 'D',
  H = 'H',
  F = 'F',
  R = 'R',
  Q = 'Q',
  E = 'E',
  X = 'X',
  Y = 'Y',
  Z = 'Z',
  PT = 'T',
  B = 'B',
};

enum ParameterValueType
{
  PVT_UNKNOWN,
  PVT_STRING,
  PVT_NUMERIC,
  PVT_ARRAY,
};

class Parameter
{
public:
  Parameter();

  void setType(ParameterType type);
  void setNumeric(double value);
  void setString(const char* value);

  ParameterType getType() { return type; }
  ParameterValueType getValueType() { return valueType; }

public:
  ParameterType type;
  ParameterValueType valueType;
  double value;
  char stringValue[32];
};


