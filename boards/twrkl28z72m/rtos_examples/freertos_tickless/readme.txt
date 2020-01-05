Overview
========
This document explains the freertos_tickless example. It shows the CPU enter at sleep mode and then
it is waked up by expired time delay that using GPT module.

Running the demo
================
The following message shows in the terminal if the example runs successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Tickless Demo example

Press SW2 to wake up the CPU

Tick count :

1

CPU waked up by EXT interrupt

5001

10001
......
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KL28Z72M board
- Personal Computer

Board settings
==============
No special is needed.

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for JLink serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

