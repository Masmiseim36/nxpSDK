Overview
========
The lin_master demo application demonstrates how to use LIN bus signal transfer from slave node.

This demo application is expected to work with another LIN instance implemented as master node on the other board. Slave is waiting for master to initialize the communication then exchange a response as signal represented by the character array. This demo uses auto-baudrate feature.

Toolchain supported
===================
- IAR embedded Workbench 8.11.1
- Keil MDK 5.23
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso10.3.1

Hardware requirements
=====================
- Two Mini/micro USB cables
- Two FRDM-KW36 boards
- Personal Computer
- Power adapter 12 V
- Three Dupont female-to-female wire

Board settings
==============
- Unmount R34 and R27 resistors
- Connect J13-1 of the two boards
- Connect J13-2 of the two boards
- Connect J13-4 of the two boards
Note: When using autobaudrate feature connect J1-5 and J2-9

Prepare the Demo
================
1.  Connect a mini/micro USB cable between the PC host and the OpenSDA USB port on the boards.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
6.  Download the program for slave node to the target board.

Running the demo
================
Slave node is awaiting master to start communication. If slave receive a frame header from master it will update its frame buffer or continue to send its own frame then stay awaiting a new frame header. Master exchanges the signals four times. The next two frames are of diagnostic type and could be used for customer purposes (for example LIN cluster go to sleep command). This sequence is than repeated.

If communication is succesfull the master and slave node will both repeatedly print:

LIN DEMO
SLAVE
LIN DEMO
MASTER
Customization options
=====================

