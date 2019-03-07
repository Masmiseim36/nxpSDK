Overview
========
The i2c_read_accel_value example shows how to use I2C driver to communicate with an i2c device:

 1. How to use the i2c driver to read a i2c device who_am_I register.
 2. How to use the i2c driver to write/read the device registers.

In this example, the values of three-axis accelerometer print to the serial terminal on PC through
the virtual serial port on board.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL-EVK  board
- JLink Plus
- 5V power supply
- Personal Computer

Board settings
==============
No special is needed.

Prepare the Demo
================
1.  Connect 5V power supply and JLink Plus to the board, switch SW2001 to power on the board
2.  Connect a USB cable between the host PC and the J1901 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

I2C example -- Read Accelerometer Value
Found a FXLS8471Q on board , the device address is 0x1e .
The accel values:
status_reg = 0xff , x =    43 , y =   -34 , z = -1916
status_reg = 0xff , x =    41 , y =   -35 , z = -1914
status_reg = 0xff , x =    40 , y =   -39 , z = -1914
status_reg = 0xff , x =    40 , y =   -35 , z = -1920
status_reg = 0xff , x =    42 , y =   -40 , z = -1918
status_reg = 0xff , x =    39 , y =   -38 , z = -1915
status_reg = 0xff , x =    44 , y =   -35 , z = -1917
status_reg = 0xff , x =    43 , y =   -34 , z = -1919
status_reg = 0xff , x =    39 , y =   -39 , z = -1918
status_reg = 0xff , x =    39 , y =   -36 , z = -1912

End of I2C example .

Customization options
=====================

