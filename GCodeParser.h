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

#include "Command.h"
#include "Parameter.h"

#define BUFFER_LENGTH 32

typedef void(*AddCommandCallback) (Command c);

enum ParserState
{
  None,
  ParameterScan,
  ScanLineNumber,
  ScanCommandNumber,
  ScanChecksum,
  LineComment,
  ScanParameterValue,
};

class GCodeParser
{
public:
  GCodeParser();
  ~GCodeParser();

  void parse(const char& c, AddCommandCallback addCommand);

private:
  void reset();
  void resetBuffer();
  inline void changeState(ParserState newState);

private:
  ParserState     previousParserState;
  ParserState     parserState;
  char            buffer[BUFFER_LENGTH];
  unsigned int    currentBufferPos;
  Command*        command;
  Parameter*      parameter;
};

