Overview
========
The EDMA ping pong transfer example is a simple demonstration program that uses the SDK software.
It excuates ping pong transfer from source buffer to destination buffer using the SDK EDMA drivers.
The purpose of this example is to show how to use ping pong buffer by the EDMA and to provide a simple example for
debugging and further development.
The example demostrate the ping pong transfer by the EDMA scatter gather feature.
Please reference user manual for the detail of the feature.
                                                                                                                                                                     minor loop finished
                                                                                                                                 +--------------------------------------------------------------+
                                                                                                                                 v                                                              |
+-------+     +---------------------------------------------------------------+     +---------------------------+  load tcd1   +-----------------------------------+  major loop not finish   +------------------+
| start | --> |                       tcd 0 major loop                        | --> | tcd 0 major loop finished | -----------> |         tcd 1 major loop          | -----------------------> | tcd 1 minor loop |
+-------+     +---------------------------------------------------------------+     +---------------------------+              +-----------------------------------+                          +------------------+
                |                         ^                       ^                                                              |
                | major loop not finish   | minor loop finished   | load tcd 0                                                   |
                v                         |                       |                                                              v
              +------------------------+  |                       |                                                            +-----------------------------------+
              |    tcd 0 minor loop    | -+                       +----------------------------------------------------------- |     tcd 1 major loop finished     |
              +------------------------+                                                                                       +-----------------------------------+
                                                                                                                                 |
                                                                                                                                 |
                                                                                                                                 v
                                                                                                                               +-----------------------------------+
                                                                                                                               |        transfer counter ++        |
                                                                                                                               +-----------------------------------+
                                                                                                                                 |
                                                                                                                                 | transfer counter not less than 5
                                                                                                                                 v
                                                                                                                               +-----------------------------------+
                                                                                                                               |         example complete          |
                                                                                                                               +-----------------------------------+

Toolchain supported
===================
- IAR embedded Workbench  8.50.9
- Keil MDK  5.33
- GCC ARM Embedded  9.3.1
- MCUXpresso  11.3.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K66F board
- Personal Computer

Board settings
==============
No special is needed.

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
EDMA scatter gather transfer example begin.

Destination Buffer:
0       0       0       0       0       0       0       0

EDMA scatter gather transfer example finish.

Destination Buffer:
1       2       3       4       5       6       7       8
~~~~~~~~~~~~~~~~~~~~~
