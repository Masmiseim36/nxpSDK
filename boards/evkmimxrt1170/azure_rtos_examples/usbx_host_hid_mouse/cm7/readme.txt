Overview
========
This example can work with a USB HID mouse. When connecting
a USB HID mouse and pressing keys, the serial port will output
which key has been pressed.


Toolchain supported
===================
- IAR embedded Workbench  9.32.1
- Keil MDK  5.37
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.7.0

Hardware requirements
=====================
- A Micro USB cables
- Target Board
- Personal Computer(PC)
- A USB mouce device

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
6.  Insert a USB mouce device in the USB1 device port of the board.
Running the demo
================
After writing the program to the flash of the target board,
press the reset button on your board to start the demo.
The serial port will output:

USBX host HID mouse example

Then, connect a USB mouse to the USB high speed device port of the board.
The example will print the pressed mouse movement.
