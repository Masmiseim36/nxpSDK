Overview
========
The bubble application demonstrates basic usage of the on-board accelerometer to implement a
bubble level. A bubble level utilizes two axes to visually show deviation from a level plane
(0 degrees) on a given access. This demo uses the FTM/TPM to modulate the duty cycle of two onboard LEDs
to gradually increase LED intensity as the board deviates from a level state.


Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini/Micro USB cable
- FRDM-KL25Z board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a mini/micro USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Unplug the usb cable about 10 second then plug again to begin running the demo.

Running the demo
================
  When the board is programmed, simply tilt the board to see the LED illuminate. One LED color
indicates X-axis angle while another indicates Y-axis angle.
  When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Welcome to BUBBLE example

You will see the change of LED brightness when change angles of board

x= -72 y = -328

x= -16 y = -268

x= -40 y = -296

x= -40 y = -296

x= -96 y = -288

x= -96 y = -288
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

