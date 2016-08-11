/*
 * RGBLed.h
 *
 *  Created on: 01.08.2016
 *      Author: haverland
 */

#ifndef FSRBOARDFIRMWARE_RGBLED_H_
#define FSRBOARDFIRMWARE_RGBLED_H_

enum LedState
{
	Manual,
	Heating,
};

class RGBLed {
public:
	RGBLed();
	~RGBLed();
	void set(byte red, byte green, byte blue);
	void off();
	void on();
	void setUseThermistor();
	void thermistor(float temperatur);
private:
	LedState state;
};

#endif /* FSRBOARDFIRMWARE_RGBLED_H_ */
