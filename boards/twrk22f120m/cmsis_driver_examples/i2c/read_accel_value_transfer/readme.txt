Overview
========
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage method please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_i2c_read_accel_value example shows how to use CMSIS I2C driver to communicate with an i2c device:

 1. How to use the i2c driver to read a i2c device who_am_I register.
 2. How to use the i2c driver to write/read the device registers.

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
- TWR-K22F120M board
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
I2C example -- Read Accelerometer Value
Found an FXOS8700 on board , the device address is 0x1f . 
The accel values:
status_reg = 0xff , x =  1515 , y =    63 , z =  1453 
status_reg = 0xff , x =  1512 , y =    33 , z =  1451 
status_reg = 0xff , x =  1508 , y =     1 , z =  1492 
status_reg = 0xff , x =  1451 , y =     1 , z =  1618 
status_reg = 0xff , x =  1459 , y =    96 , z =  1558 
status_reg = 0xff , x =  1530 , y =    10 , z =  1369 
status_reg = 0xff , x =  1473 , y =    70 , z =  1603 
status_reg = 0xff , x =  1467 , y =   127 , z =  1577 
status_reg = 0xff , x =  1489 , y =   135 , z =  1449 
status_reg = 0xff , x =  1631 , y =   274 , z =  1780 

End of I2C example .
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

