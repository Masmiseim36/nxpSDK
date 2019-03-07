Overview
========
The GPIO Example project is a demonstration program that uses the KSDK software to manipulate the general-purpose
outputs.
The example is supported by the set, clear registers for each GPIO pin output register. 

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
No special settings are required.

Prepare the Demo
================
1.  Connect 5V power supply and JLink Plus to the board, switch SW2001 to power on the board.
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
When the example runs successfully, the following message is displayed in the terminal:
If you turn on the SW601-3, and "SW601-3 is turned on" is shown on the terminal window

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 GPIO Driver example.

 SW601-3 is turned on.
 ......
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

