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

#include "Parameter.h"

#include <stdint.h>

enum CommandType
{
  UNKNOWN = '\0',
  G = 'G',
  M = 'M',
  T = 'T',
};


class Command
{
public:
  Command();

  //void setLineNumber(long lineNumber);
  void setCommandType(CommandType type);
  void setCommandNumber(long number);

  void addParameter(Parameter p);

  void print();
  void reset();

  CommandType getCommandType() { return commandType; }
  long getCommandCode() { return commandNumber; }
  //long getLineNumber() { return lineNumber; }

  Parameter parameters[5];
  unsigned int parameterCount;

  //TODO: use template functions for parameter getters
  double getParameterValue(ParameterType parameterType, double defaultValue = 0);
  double getParameterValue(uint32_t index, double defaultValue = 0);
  const char* getParameterStringValue(ParameterType parameterType, const char* defaultValue = "");
  const char* getParameterStringValue(uint32_t index, const char* defaultValue = "");

private:
  //long lineNumber;
  CommandType commandType;
  long commandNumber;
};


