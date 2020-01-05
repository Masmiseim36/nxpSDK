Overview
========
The LPIT project is a simple example of the SDK LPIT driver. It sets up the LPIT
hardware block to trigger a periodic interrupt after every 1 second. When the LPIT interrupt is triggered
a message a printed on the serial terminal and an LED is toggled on the board.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Starting channel No.0 ...

 Channel No.0 interrupt is occured !

 Channel No.0 interrupt is occured !
 ......
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Hardware requirements
=====================
- Mini/Micro USB cable
- TWR-KL28Z72M board
- Personal Computer

Board settings
==============
This LPIT Example project does not call for any special hardware configurations. Although not required, the recommendation is to leave the development
board's jumper settings and configurations in default state when running this example.

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

