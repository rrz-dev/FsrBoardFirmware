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

#include <Arduino.h>

template <typename T> class CircularBuffer {
public:
  CircularBuffer(const size_t size)
    : size(size)
    , head(0)
    , tail(0)
    , count(0)
    , avg(0)
  {
    buffer = new T[size];
  }

  virtual ~CircularBuffer()
  {
    delete [] buffer;
  }

  void clear()
  {
    head = 0;
    tail = 0;
    count = 0;
    avg = 0;

    for (size_t i = 0; i < size; i++)
    {
      buffer[size] = 0;
    }
  }

  void push(const T& object)
  {
    size_t nextHead = next(head);
    
    buffer[head] = object;
    head = nextHead;

    if (count < size) count++;

    T sum = 0;
    for (size_t i = 0; i < count; i++)
    {
      sum += buffer[i];
    }
    avg = sum / count;
  }

  bool pop(T& object)
  {
    if (tail == head) return false;

    object = buffer[tail];
    tail = next(tail);

    return true;
  }

  T average()
  {
    return avg;
  }

  size_t bufferSize()
  {
    return size;
  }

  size_t currentElementCount()
  {
    return count;
  }
  
private:
  size_t next(size_t current)
  {
    return (current + 1) % size;
  }

private:
  T* buffer;
  volatile size_t head;
  volatile size_t tail;
  const size_t size;
  size_t count;
  volatile T avg;
};

