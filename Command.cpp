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

#include "Command.h"
#include "Arduino.h"

Command::Command()
  : // lineNumber(-1)
   parameterCount(0)
  , commandType(UNKNOWN)
  , commandNumber(0)
{
}

//void Command::setLineNumber(long lineNumber)
//{
//  Command::lineNumber = lineNumber;
//}

void Command::setCommandType(CommandType type)
{
  commandType = type;
}

void Command::setCommandNumber(long number)
{
  commandNumber = number;
}

void Command::addParameter(Parameter p)
{
  parameters[parameterCount++] = p;
}

void Command::print()
{
/*
  using namespace std;

  cout << "DEBUG: Command ";
  if (lineNumber > -1)
  {
    cout << "(line: " << lineNumber << ") ";
  }
  cout << (char)commandType << commandNumber << " ";

  for (int i = 0; i < parameterCount; i++)
  {
    cout << (char)parameters[i].type << " = ";

    switch (parameters[i].getValueType())
    {
    case PVT_NUMERIC:
      cout << parameters[i].value << " ";
      break;
    case PVT_STRING:
      cout << parameters[i].stringValue << " ";
      break;
    }
  }

  cout << endl;
*/

  Serial.print("DEBUG: Command (line: ");
//  Serial.print(lineNumber);
  Serial.print(") ");
  Serial.print((char)commandType);
  Serial.println(commandNumber);
}

double Command::getParameterValue(ParameterType parameterType, double defaultValue)
{
  for (unsigned int i = 0; i < parameterCount; i++)
  {
    if (parameters[i].type == parameterType && parameters[i].valueType == PVT_NUMERIC)
    {
      return parameters[i].value;
    }
  }

  return defaultValue;
}

double Command::getParameterValue(uint32_t index, double defaultValue)
{
  if (parameterCount < index || parameters[index].valueType != PVT_NUMERIC)
  {
    return defaultValue;
  }

  return parameters[index].value;
}

const char* Command::getParameterStringValue(ParameterType parameterType, const char* defaultValue)
{
  for (unsigned int i = 0; i < parameterCount; i++)
  {
    if (parameters[i].type == parameterType && parameters[i].valueType == PVT_STRING)
    {
      return parameters[i].stringValue;
    }
  }

  return defaultValue;
}

const char* Command::getParameterStringValue(uint32_t index, const char* defaultValue)
{
  if (parameterCount < index || parameters[index].valueType != PVT_STRING)
  {
    return defaultValue;
  }

  return parameters[index].stringValue;  
}


void Command::reset()
{
//  lineNumber = -1;
  parameterCount = 0;
  commandNumber = -1;
  commandType = UNKNOWN;

  for (int i = 0; i < MAX_PARAMETER_COUNT; i++)
  {
    parameters[i].reset();
  }
}

