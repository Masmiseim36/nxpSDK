Overview
========
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage method please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_enet_transfer example shows how to use CMSIS ENET driver:

In this example, the ENET transmits 20 number broadcast frames and will print the received frame.


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- Network cable RJ45 standard
- TWR-K64F120M board
- TWR-SER board
- Personal Computer

Board settings
==============
MII mode setting in TWR-SERIAL board is :
RMII mode - J2 3-4 shunt on, J3 2-3 shunt on, J12 shunt on 9,10.
On TWR-K64F120M board:
J32 1-2 shunt on to keep the same clock source for 
Ethernet and PHY when using RMII mode.

Prepare the Demo
================
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert Cable to Ethernet RJ45 port and connect it to your PC.
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs, the log would be seen on the OpenSDA terminal like:

ENET example start.

The 1 frames transmitted success!

The 2 frames transmitted success!

The 3 frames transmitted success!

The 4 frames transmitted success!

The 5 frames transmitted success!

The 6 frames transmitted success!

The 7 frames transmitted success!

The 8 frames transmitted success!

The 9 frames transmitted success!

The 10 frames transmitted success!

The 11 frames transmitted success!

The 12 frames transmitted success!

The 13 frames transmitted success!

The 14 frames transmitted success!

The 15 frames transmitted success!

The 16 frames transmitted success!

The 17 frames transmitted success!

The 18 frames transmitted success!

The 19 frames transmitted success!

The 20 frames transmitted success!

The 1 frames has been successfully received!

The 2 frames has been successfully received!

The 3 frames has been successfully received!

The 4 frames has been successfully received!

The 7 frames has been successfully received!

The 10 frames has been successfully received!

The 13 frames has been successfully received!

The 16 frames has been successfully received!

The 17 frames has been successfully received!

The 18 frames has been successfully received!

The 19 frames has been successfully received!

The 20 frames has been successfully received!


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
the transmitted frames is a 1000 length broadcast frame. the frame can be seen
when the PC is installed with wireshark.
For receive side, the total number of all received frames will be print out like:
The # frames has been successfully received.
Customization options
=====================

