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

#include "GCodeParser.h"

#include "Arduino.h"

GCodeParser::GCodeParser()
  : currentBufferPos(0)
  , command(0)
  , parameter(0)
{
  reset();
}

GCodeParser::~GCodeParser()
{
  delete command;
  delete parameter;
}

void GCodeParser::parse(const char& c, AddCommandCallback addCommand)
{
  while (true)
  {
    switch (parserState)
    {
    case None:
      switch (c)
      {
        case ' ':   // ignore whitespace and tabulators
        case '\t':
          return;
        case 'n':   // line number
        case 'N':
          changeState(ScanLineNumber);
          return;
        case 'g':   // g code command - standard GCode
        case 'G':
        case 'm':   // m code command - RepRap command
        case 'M':
        case 't':   // t code command - select tool/extruder
        case 'T':
          command->setCommandType((CommandType)(c < 97 ? c : c - 32)); // make uppercase
          changeState(ScanCommandNumber);
          return;
        case '*':   // checksum parameter
          changeState(ScanChecksum);
          return;
        case ';':   // start of line comment -> ignore until eol
          changeState(LineComment);
          // no break or return... start of comment is command end
        case '\r':
        case '\n':
        case '\0':
          if (command->getCommandType() != UNKNOWN)
          {
            addCommand(*command);
            command->setCommandType(UNKNOWN);
          }
          break;
      }
      break;
    case ParameterScan:
      switch (c)
      {
        case ' ':   // ignore whitespace and tabulators
        case '\t':
          return;
        case '*':   // checksum parameter
          changeState(ScanChecksum);
          return;
        case ';':   // start of line comment -> ignore until eol
          changeState(LineComment);
          // no break or return... start of comment is command end
        case '\r':
        case '\n':
        case '\0':
          if (command->getCommandType() != UNKNOWN)
          {
            addCommand(*command);
            command->reset();
          }
          break;
        case 'x':   // A X coordinate, usually to move to. This can be an Integer or Fractional number. 
        case 'X':
        case 'y':   // A Y coordinate, usually to move to. This can be an Integer or Fractional number. 
        case 'Y':
        case 'z':   // A Z coordinate, usually to move to. This can be an Integer or Fractional number. 
        case 'Z':
        case 's':   // Command parameter, such as time in seconds; temperatures; voltage to send to a motor 
        case 'S':
        case 'p':   // Command parameter, such as time in milliseconds; proportional (Kp) in PID Tuning 
        case 'P':
        case 'i':   // Parameter - X-offset in arc move; integral (Ki) in PID Tuning 
        case 'I':
        case 'j':   // Parameter - Y-offset in arc move 
        case 'J':
        case 'd':   // Parameter - used for diameter; derivative (Kd) in PID Tuning 
        case 'D':
        case 'h':   // Parameter - used for heater number in PID Tuning 
        case 'H':
        case 'f':   // Feedrate in mm per minute. (Speed of print head movement) 
        case 'F':
        case 'r':   // Parameter - used for temperatures 
        case 'R':
        case 'q':   // Parameter - not currently used 
        case 'Q':
        case 'e':   // Length of extrudate. This is exactly like X, Y and Z, but for the length of filament to extrude. It is common for newer stepper based systems to interpret ... Better: Skeinforge 40 and up interprets this as the absolute length of input filament to consume, rather than the length of the extruded output. 
        case 'E':
        case 't':
        case 'T':
        case 'b':
        case 'B':
          parameter->setType((ParameterType)(c < 97 ? c : c - 32)); // make uppercase
          changeState(ScanParameterValue);
          return;
      }

      break;
    case ScanParameterValue:
      if (c >= 32 && c <= 128 && c != ' ' && c != '\t' && c != ';')
      {
        buffer[currentBufferPos++] = c;
      }
      else
      {
        if (currentBufferPos > 0)
        {
          bool isString = false;
          bool isArray = false;
          for (unsigned int i = 0; i < currentBufferPos; i++)
          {
            if (!isdigit(buffer[i]) && buffer[i] != '.' && buffer[i] != '-')
            {
              if (buffer[i] == ':')
              {
                isArray = true;
                break;
              }
              else
              {
                isString = true;
                break;
              }
            }
          }

          if (!isString && !isArray)
          {
            double value = atof(buffer);
            parameter->setNumeric(value);
          }
          else if (isString && !isArray)
          {
            //std::cout << "String parameter: " << buffer << std::endl;
            parameter->setString(buffer);
          }
          else if (!isString && isArray)
          {
            //TODO: implement
            //std::cout << "Numeric Array parameter: " << buffer << std::endl;
          }
          else
          {
            //TODO: implement
            //std::cout << "String Array parameter: " << buffer << std::endl;
          }
        }
        changeState(ParameterScan);
        resetBuffer();

        command->addParameter(*parameter);

        continue;
      }
      break;
    case ScanLineNumber:
      if (c >= 48 && c <= 57)   // 0 to 9 / ascii codes
      {
        // number
        buffer[currentBufferPos++] = c;

        return;
      }
      else
      {
        // not a number -> line number ended
        long lineNumber = atol(buffer);
        command->setLineNumber(lineNumber);
        //Serial << "DEBUG: Line number: " << lineNumber << endl;
        changeState(None);
        resetBuffer();

        continue;
      }

      break;
    case ScanCommandNumber:
      if (c >= 48 && c <= 57)   // 0 to 9 / ascii codes
      {
        // number
        buffer[currentBufferPos++] = c;

        return;
      }
      else
      {
        // not a number -> command number ended
        long commandNumber = atol(buffer);
        command->setCommandNumber(commandNumber);
        //Serial << "DEBUG: command: " << (char)command->getCommandType() << " code: " << commandNumber << endl;
        changeState(ParameterScan);
        resetBuffer();

        continue;
      }

      break;
    case ScanChecksum:
      if (c >= 48 && c <= 57)   // 0 to 9 / ascii codes
      {
        // number
        buffer[currentBufferPos++] = c;

        return;
      }
      else
      {
        // not a number -> checksum ended
        long checksum = atol(buffer);
        //TODO: compute checksum of current line and return
        //Serial << "DEBUG: checksum: " << checksum << endl;
        changeState(None);
        resetBuffer();

        continue;
      }

      break;
    case LineComment:
      if (c == '\r' || c == '\n' || c == '\0')
      {
        changeState(None);
        return;
      }
    }

    return;
  }
}

void GCodeParser::reset()
{
  if (!command)
  {
    command = new Command();
  }

  if (!parameter)
  {
    parameter = new Parameter();
  }

  for (unsigned int i = 0; i < BUFFER_LENGTH; i++)
  {
    buffer[i] = '\0';
  }

  parserState = None;
  previousParserState = None;

  currentBufferPos = 0;
}

void GCodeParser::resetBuffer()
{
  for (unsigned int i = 0; i <= currentBufferPos; i++)
  {
    buffer[i] = '\0';
  }

  currentBufferPos = 0;
}

void GCodeParser::changeState(ParserState newState)
{
  previousParserState = parserState;
  parserState = newState;

  //std::cout << "change parser state from " << parserStateStrings[previousParserState] << " to " << parserStateStrings[parserState] << std::endl;
}


