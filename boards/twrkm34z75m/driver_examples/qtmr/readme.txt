Overview
========

The QTMR project is a simple demonstration program of the SDK QTMR driver.
The quad-timer module provides four timer channels with a variety of controls affecting both individual
and multi-channel features. Specific features include up/down count,cascading of counters, programmable modulo,
count once/repeated, counter preload, compare registers with preload, shared use of input signals, prescaler controls,
independent capture/compare, fault input control, programmable input filters, and multichannel synchronization.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KM34Z75M board
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
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*********QUADTIMER EXAMPLE START*********
****Timer use-case, 1 second tick.****
 Timer interrupt has occured !
 Timer interrupt has occured !
....................
....................
****Chain Timer use-case, 10 second tick.****
 Timer interrupt has occured !
 Timer interrupt has occured !
.........
.........
****Input capture example.****
****Provide a signal input to the TMR0 pin****
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
And you will find the LED RED is taking turns to shine.
Customization options
=====================

