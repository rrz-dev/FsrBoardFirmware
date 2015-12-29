# FSR Board Firmware
Firmware for the RepRap FSR Board (http://www.repraptools.de) licensed under GPL v3 license (see LICENSE for more information).

# Supported M-Codes
* M112 - print diagnostic sensor information
* M115 - print firmware information
* M119 - display endstop status
* M500 - store settings in EEPROM
* M502 - revert to factory settings
* M503 - print settings
* M800 - set configuration key/value (see notes for further information on keys). Two arguments are used: K and V

# Setting keys and default values
For setting configuration values use the M800 M-Code. It has two argument types: K and V.

**Example usage:** M800 K*[key]* V*[value]*

* longAverageBufferTime (default: 5000) - time in milliseconds on which the sensor values without touch are averaged
* defaultEndstopMinHighMs (default: 500) - this is the time in milliseconds the endstop out is triggered at least
* triggerThreshold (default: 14) - this is the minimum differenc between the current sensor reading and the average sensor reading for triggering the endstop out
* calibrationLedDelay (default: 250) - this is the delay in milliseconds for the blinking leds when calibrating the sensor board after a reset
* i2cSlaveAddress (default: 77) - slave address which is used to listen for messages on the i2c bus
