Overview
========
The fourWireModeInterrupt example shows how to use TSC driver in 4-wire interrupt modes:
The touch screen controller needs to co-work with ADC and TSC analogue.

In this example , we make use of the available electrodes on board to show driver usage.
1. Firstly, before TSC starts work, ADC driver configure ADC_HCx;
2. Then, we configure TSC module to work in non-auto-measure mode.
3. Then, we start screen detection and wait for software order after detects a touch (and generates an interrupt);
4. Then, we start measure during detect interruption.
5. Then, we judge whether the measure data is valid during measure interruption.
6. Lastly, if measure data is valid, serial terminal prints x/y-coordinate information.


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
- NXP LCD8000-43T
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect 5V power supply and JLink Plus to the board, switch SW2001 to power on the board.
2.  Connect a USB cable between the host PC and the J1901 USB port on the target board.
3.  Connect LCD8000-43T to the J901 LCD expansion port connector.
4.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When running successfully, the log output in terminal shall be similar as below:

TSC fourWireModeInterrupt Example Start!
ADC_5HC_DoAntoCalibration() Done.
Please touch screen.
x = 3741, y = 1068
x = 3741, y = 572
x = 3746, y = 517
x = 3745, y = 518
x = 3751, y = 508
x = 3752, y = 512
x = 3750, y = 507
...

Customization options
=====================

