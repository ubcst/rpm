# rpm
Repository containing Arduino code for getting the RPMs of the different Hall Effect sensors.

Code primarily based off of: http://playground.arduino.cc/Main/ReadingRPM

This code was written for and tested on an Arduino Mega 2560

## Pinout
Pin 2 - Input pin for the Hall Effect sensor over the engine gear. An interrupt will be triggered on a falling edge.

Pin 3 - Input pin for the Hall Effect sensor over the wheel gear. An interrupt will be triggered on a falling edge.

Pin 18 - Input pin for the clutch switch. An interrupt will be triggered on either a rising or a falling edge.

Pin 22 - Output pin that gets set HIGH when the engine RPM and the wheel RPM is within 5% of each other.

## Testing Notes
When testing the circuit, make sure that whatever is passed into the input pins is either a logical HIGH (5V) or logical LOW (0V). If it isn't then you'll probably see fluctuating values as the input would be some voltage between 0V and 5V.
