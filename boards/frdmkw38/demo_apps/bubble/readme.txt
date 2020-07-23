Overview
========
The bubble application demonstrates basic usage of the on-board accelerometer to implement a
bubble level. A bubble level utilizes two axes to visually show deviation from a level plane
(0 degrees) on a given access. .


Toolchain supported
===================
- IAR embedded Workbench  8.32.4
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/Micro USB cable
- FRDM-KW38 board
- Personal Computer

Board settings
==============
This example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.

Prepare the Demo
================
1.  Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal with these settings:
     - 115200 baud rate
     - 8 data bits
     - No parity
     - One stop bit
     - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the board is programmed, simply tilt the board to see the RGB LED illuminate. One color
indicates X-axis angle while another indicates Y-axis angle.
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~
Welcome to the BUBBLE example

You will see angle data change in the console when change the angles of board

x=  6 y = 22
x=  8 y = 26
x= 10 y = 28
x= 10 y = 28
x= 11 y = 29
x= 11 y = 29
~~~~~~~~~~~~
Customization options
=====================

