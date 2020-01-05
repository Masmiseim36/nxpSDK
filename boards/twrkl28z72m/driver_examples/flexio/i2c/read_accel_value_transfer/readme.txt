Overview
========
The flexio_i2c_read_accel_value example shows how to use FLEXIO I2C  Master driver to communicate with an i2c device:

 1. How to use the flexio i2c master driver to read a i2c device who_am_I register.
 2. How to use the flexio i2c master driver to write/read the device registers.

In this example, the values of three-axis accelerometer print to the serial terminal on PC through
the virtual serial port on board.
Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KL28Z72M board
- Personal Computer

Board settings
==============
The flexio_i2c_read_accel_value example is requires connecting the FLEXIO pins with the I2C pins of accelerometer
SCL      Jumper(J26-7) <--->  Jumper(J15-2)
SDA      Jumper(J26-8) <--->  Jumper(J14-2)

Prepare the Demo
================
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
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

~~~~~~~~~~~~~~~~~~~~~
FlexIO I2C example read accelerometer value

Found an MMA8451 on board, the device address is 0x1D.
The accel values:

status_reg = 0xff , x =   -50 , y =    11 , z =  2045
status_reg = 0xff , x =   -50 , y =     9 , z =  2031
status_reg = 0xff , x =   -49 , y =    12 , z =  2032
status_reg = 0xff , x =   -50 , y =     6 , z =  2046
status_reg = 0xff , x =   -51 , y =     8 , z =  2040
status_reg = 0xff , x =   -45 , y =     4 , z =  2032
status_reg = 0xff , x =   -52 , y =    13 , z =  2042
status_reg = 0xff , x =   -53 , y =    12 , z =  2041
status_reg = 0xff , x =   -51 , y =     8 , z =  2048
status_reg = 0xff , x =   -52 , y =     9 , z =  2036

End of I2C example .
~~~~~~~~~~~~~~~~~~~~~

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

