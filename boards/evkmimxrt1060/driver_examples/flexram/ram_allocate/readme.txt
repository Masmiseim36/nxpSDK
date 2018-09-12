Overview
========
The FLEXRAM project is a simple demonstration program of the SDK FLEXRAM driver. It allocate the on-chip ram and then access the OCRAM to demo magic address and access error interrupt.

Toolchain supported
===================
- Keil MDK 5.25
- IAR embedded Workbench 8.30.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.1

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1060 board
- Personal Computer

Board settings
==============
This example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FLEXRAM ram allocate example.

Allocate on-chip ram:

   OCRAM bank numbers 4

   DTCM  bank numbers 8

   ITCM  bank numbers 4

Allocate on-chip ram success

OCRAM Magic address 0x202800a0 match.

OCRAM Magic address 0x202800a4 match.

OCRAM access to 0x202a0000 boundary.

DTCM Magic address 0x200000a0 match.

ITCM Magic address 0xa0 match.

FLEXRAM ram allocate example finish
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

