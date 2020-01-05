Overview
========
The lpi2c_read_accel_value example shows how to use LPI2C driver to communicate with an lpi2c device:

 1. How to use the lpi2c driver to read a lpi2c device who_am_I register.
 2. How to use the lpi2c driver to write/read the device registers.

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
- Personal Computer

Board settings
==============
No special setting is needed

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
LPI2C example -- Read Accelerometer Value
Found an FXOS8700 on board , the device address is 0x1d .
The accel values:
status_reg = 0xff , x =   101 , y =    48 , z =  2123
status_reg = 0xff , x =   101 , y =    45 , z =  2123
status_reg = 0xff , x =   101 , y =    48 , z =  2129
status_reg = 0xff , x =   102 , y =    50 , z =  2120
status_reg = 0xff , x =    99 , y =    50 , z =  2123
status_reg = 0xff , x =    99 , y =    47 , z =  2127
status_reg = 0xff , x =    99 , y =    49 , z =  2133
status_reg = 0xff , x =    99 , y =    48 , z =  2128
status_reg = 0xff , x =   101 , y =    46 , z =  2129
status_reg = 0xff , x =   100 , y =    47 , z =  2111

End of LPI2C example .
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

