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
- Micro USB cable
- FRDM-KW41Z board
- Personal Computer

Board settings
==============
No special settings are required.

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
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
I2C example -- Read Accelerometer Value
Found an FXOS8700 on board , the device address is 0x1f . 
The accel values:
status_reg = 0xff , x =    70 , y =    12 , z =  2138 
status_reg = 0xff , x =    49 , y =    73 , z =  2139 
status_reg = 0xff , x =    35 , y =   175 , z =  2140 
status_reg = 0xff , x =     5 , y =   133 , z =  2160 
status_reg = 0xff , x =    83 , y =     4 , z =  2138 
status_reg = 0xff , x =    16 , y =   112 , z =  2126 
status_reg = 0xff , x =    33 , y =    38 , z =  2133 
status_reg = 0xff , x =    25 , y =    61 , z =  2135 
status_reg = 0xff , x =    40 , y =    59 , z =  2138 
status_reg = 0xff , x =    18 , y =    78 , z =  2140 

End of I2C example .
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

