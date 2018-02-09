# Joystick-to-RGB-LED project for Elegoo UNO R3
Uses components from the Elegoo UNO R3 Super Starter Kit
Allows user to control colour of a single RGB LED by using the joystick as a controller.
Colour can be fixed by clicking the joystick button, and released by clicking it again.

This code is rough as all hell but it does the trick.

*All pin-allocations and electronic setup is included in code comments

## Components You Will Need

NB All components are included in the Elegoo Uno R3 Super Starter Set

- 1x Elegoo Uno R3
- 1x USB A to B cable (for power and code upload)
- 1x Joystick Module
- 1x RGB LED
- 3x 220ohm resistor
- a bunch of male-male and male-female connectors
- a protoyping board or breadboard. Or a soldering kit, if you intend to throw this all out later

## Build Instructions

1. Connect RGB LED common cathode (long) pin to GND
2. Connect remaining three pins **in series with a 220ohm resistor each**, and from there to the following Digital (PWM) pins: 
  * R to 7
  * G to 6
  * B to 3
3. Connect joystick pins:
  * GND to GND
  * +5V to +5V
  * VRx to Analog pin A0
  * VRy to Analog pin A1
  * SW to Ditial (PWM) pin 2
4. Connect USB cable, upload .ino file and GO NUTS, BABY
