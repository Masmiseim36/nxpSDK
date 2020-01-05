Overview
========
The FTM project is a simple demonstration program of the SDK FTM driver to use FTM as a timer.
It sets up the FTM hardware block to trigger an interrupt every 1 millisecond.
When the FTM interrupt is triggered a message a printed on the UART terminal.
Hardware requirements
=====================
- Micro USB cable
- FRDM-KE16Z board
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
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~
FTM example to simulate a timer

You will see a "-" or "|" in terminal every 1 second:
-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
~~~~~~~~~~~~~~~~~~~~~~~

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.2.1

