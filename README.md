# Arduino-Code_23-24
## Arduino Side of Koalby/Ava

### Included Programs
- Dynamixel_Readdress - Readdress a Dynamixel motor. Currently in need of fixing/commenting
- HerkuleX_Check - Checks all attached HerkuleX’s (prints any ID’s that return an anlg)
- HerkuleX_Readdress - set the address of a HerkuleX motor. NEVER run this on the real robot. It checks and resets all attached HerkuleX’s, so *only* the motor which you want to change should be connected
- Koalby_Firmware - Main firmware for Koalby, contains serial listen and motor control functionality. This should be left on the robot when running Python code.
  - ArduinoPoppy.c - actual robot control and functionality
  - ArduinoPoppy.h - definitions for motor parameters, contains motor array
- Read_Dyn_Angle - Read and print angle of one Dynamixel, used to find limits. Motor torque is disabled to allow for
- Read_Herk_Angle - Read and print angle of one HerkuleX, used to find limits
- TestProtocolSwap - Test program used to check speed switching between Dynamixel protocols, not useful in most cases but left here anyway
- scan_dynamixel_mega - Prints all attached Dynamixels at any baud rate or protocol. Directly from a library example

### Firmware

Include this library
https://github.com/Arduinolibrary/DFRobot_Herkulex_library/raw/master/Herkulex.zip
in ArduinoIDE

## Branches (Update as new branches are made)
### main
Ready to deploy for good production codebase on Ava's Arduino
### dev
Ready to test development codebase on Ava's Arduino
