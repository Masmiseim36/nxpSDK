Overview
========
The LPTMR project is a simple demonstration program of the SDK LPTMR driver. It sets up the LPTMR
hardware block to trigger a periodic interrupt after every 1 second. When the LPTMR interrupt is triggered
a message a printed on the UART terminal and an LED is toggled on the board.
Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KL28Z72M board
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
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~
Low Power Timer Example
LPTMR interrupt No.1
LPTMR interrupt No.2
LPTMR interrupt No.3
....................
~~~~~~~~~~~~~~~~~~~~~~~
And you will find the LED RED is taking turns to shine.

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

