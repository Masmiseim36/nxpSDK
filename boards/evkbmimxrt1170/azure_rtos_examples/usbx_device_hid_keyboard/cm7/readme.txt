Overview
========
This example works as a USB HID device. It will appear as a USB keyboard device on PC.


Toolchain supported
===================
- IAR embedded Workbench  9.40.1
- Keil MDK  5.38.1
- GCC ARM Embedded  12.2
- MCUXpresso  11.8.0

Hardware requirements
=====================
- Two Micro USB cables
- Target Board
- Personal Computer(PC)

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
3.  Compile the demo.
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
6.  Connect a USB cable between the PC and the USB1 device port of the board.
7.  PC can get the input char.
Running the demo
================
After writing the program to the flash of the target board,
press the reset button on your board to start the demo.
The serial port will output:

USBX device HID Keyboard example

Then, connect a USB cable between PC and the USB device port
of the board. A HID-compliant keyboard will appear in the
Device Manager of Windows and send a key every 2 secondes.

The serial port will output:

HID device activate
