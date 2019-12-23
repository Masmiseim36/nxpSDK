Overview
========
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage methord please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_i2c_read_accel_value example shows how to use CMSIS I2C driver to communicate with an i2c device:

 1. How to use the i2c driver to read a i2c device who_am_I register.
 2. How to use the i2c driver to write/read the device registers.

In this example, the values of three-axis accelerometer print to the serial terminal on PC through
the virtual serial port on board.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini USB cable
- TWR-KV10Z32 board
- Personal Computer

Board settings
==============
No special is needed.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
I2C example -- Read Accelerometer Value
Found an MMA8451 on board , the device address is 0x1d . 
The accel values:
status_reg = 0xff , x =    38 , y =    69 , z =  1932 
status_reg = 0xff , x =   123 , y =    99 , z =  2021 
status_reg = 0xff , x =   505 , y =   391 , z =  2054 
status_reg = 0xff , x =    48 , y =    52 , z =  2028 
status_reg = 0xff , x =    65 , y =    22 , z =  2008 
status_reg = 0xff , x =    38 , y =    30 , z =  2055 
status_reg = 0xff , x =    44 , y =     6 , z =  2057 
status_reg = 0xff , x =    47 , y =    14 , z =  2049 
status_reg = 0xff , x =    42 , y =     4 , z =  2051 
status_reg = 0xff , x =    42 , y =     0 , z =  2046 

End of I2C example .
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

