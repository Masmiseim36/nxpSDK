Overview
========
The pflash example shows how to use flash driver to operate program flash:


Running the demo
================
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 PFlash Example Start

 PFlash Information:

 Total Program Flash Size:      512 KB, Hex: (0x80000)

 Program Flash Sector Size:     2 KB, Hex: (0x800)

 Flash is UNSECURE!

 Erase a sector of flash

 Successfully Erased Sector 0x7f800 -> 0x80000

 Program a buffer to a sector of flash

 Successfully Programmed and Verified Location 0x7f800 -> 0x7f810

 End of PFlash Example
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KL28Z72M board
- Personal Computer

Board settings
==============
This Example project does not call for any special hardware configurations.
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


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

