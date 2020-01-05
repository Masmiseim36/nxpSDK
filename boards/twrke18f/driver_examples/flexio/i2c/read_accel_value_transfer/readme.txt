Overview
========
The flexio_i2c_read_accel_value example shows how to use FLEXIO I2C  Master driver to communicate with an i2c device:

 1. How to use the flexio i2c master driver to read a i2c device who_am_I register.
 2. How to use the flexio i2c master driver to write/read the device registers.

In this example, the values of three-axis accelerometer print to the serial terminal on PC through
the virtual serial port on board.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Micro USB cable
- TWR-KE18F board
- Primary Elevator
- Personal Computer

Board settings
==============
The example requires connecting the FLEXIO pins with the I2C pins of accelerometer
Insert TWR-KE18F board into Primary Elevator. The connection should be set as following:
- B7-Elevator, A7-Elevator connected
- B11-Elevator, A8-Elevator connected

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
FlexIO I2C example read accelerometer value
Found a FXOS8700 on board, the device address is 0x1D. 
The accel values:
status_reg = 0xff , x =   180 , y =   -47 , z =  2024 
status_reg = 0xff , x =    25 , y =     4 , z =  2029 
status_reg = 0xff , x =   130 , y =   -98 , z =  2115 
status_reg = 0xff , x =   103 , y =   -84 , z =  2137 
status_reg = 0xff , x =    57 , y =   -55 , z =  2093 
status_reg = 0xff , x =    80 , y =   -53 , z =  2066 
status_reg = 0xff , x =    93 , y =   -70 , z =  2064 
status_reg = 0xff , x =    96 , y =   -70 , z =  2072 
status_reg = 0xff , x =    89 , y =   -64 , z =  2067 
status_reg = 0xff , x =    67 , y =   -60 , z =  2111 

End of I2C example .
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

