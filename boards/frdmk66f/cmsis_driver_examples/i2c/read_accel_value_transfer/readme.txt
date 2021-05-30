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
- IAR embedded Workbench  8.50.9
- Keil MDK  5.33
- GCC ARM Embedded  9.3.1
- MCUXpresso  11.3.0

Hardware requirements
=====================
- Mini/Micro USB cable
- FRDM-K66F board
- Personal Computer

Board settings
==============
No special is needed.

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
When the example runs successfully, you can see the similar information from the terminal as below.

I2C example -- Read Accelerometer Value
Found an FXOS8700 on board , the device address is 0x1c . 
The accel values:
status_reg = 0xff , x =    63 , y =    82 , z =  2068 
status_reg = 0xff , x =    58 , y =    88 , z =  2079 
status_reg = 0xff , x =    62 , y =    79 , z =  2095 
status_reg = 0xff , x =    64 , y =    91 , z =  2088 
status_reg = 0xff , x =    59 , y =    90 , z =  2089 
status_reg = 0xff , x =    63 , y =    78 , z =  2073 
status_reg = 0xff , x =    57 , y =    89 , z =  2083 
status_reg = 0xff , x =    63 , y =    81 , z =  2091 
status_reg = 0xff , x =    63 , y =    89 , z =  2086 
status_reg = 0xff , x =    59 , y =    87 , z =  2082 

End of I2C example .
