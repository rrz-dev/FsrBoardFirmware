/*
 * RGBLed.cpp
 *
 *  Created on: 01.08.2016
 *      Author: haverland
 */

#include <Arduino.h>
#include "Pins.h"
#include "RGBLed.h"
#include "Configuration.h"
#include "Color.h"

RGBLed::RGBLed()  {
  state = Configuration::getRgbOutEnabled() ? Manual : Heating;

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  off();
}

RGBLed::~RGBLed() {
	off();
}

void RGBLed::set(byte red, byte green, byte blue) {
	state = Manual;
	analogWrite(LED_R, red);
	analogWrite(LED_G, green);
	analogWrite(LED_B, blue);
}

void RGBLed::off() {
	set(0,0,0);
}

void RGBLed::setUseThermistor() {
	state  = Heating;
}

void RGBLed::thermistor(float temperatur) {
  
   if (state == Heating) {
      float lowTemp = Configuration::getColdTemp();
    	float highTemp = Configuration::getHotTemp();
    	float divider = highTemp - lowTemp;
    	float t = (temperatur - lowTemp) / divider;

    	Color cold(Configuration::getColdR()/255.0f, Configuration::getColdG()/255.0f, Configuration::getColdB()/255.0f);
    	Color hot(Configuration::getHotR()/255.0f, Configuration::getHotG()/255.0f, Configuration::getHotB()/255.0f);

    	Color rgbLedColor = cold.interpolate(hot, t, &linearF);
    	RGB rgb = rgbLedColor.getRGB();

    	analogWrite(LED_R, static_cast<byte>(rgb.r * 255));
    	analogWrite(LED_G, static_cast<byte>(rgb.g * 255));
    	analogWrite(LED_B, static_cast<byte>(rgb.b * 255));
  }

}

