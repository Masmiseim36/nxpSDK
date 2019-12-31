Overview
========
The lin_master demo application demonstrates how to use LIN bus signal transfer from master node.

This demo application is expected to work with another LIN instance implemented as slave node on the other board. Master node repeatedly initializes transfer by generating frame header, master and slave then exchange response as signal represented by the character array. This demo uses auto-baudrate feature.

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
- Three Dupont female-to-female wires

Board settings
==============
- Connect J13-1 of the two boards
- Connect J13-2 of the two boards
- Connect J13-4 of the two boards
- Connect 12 V adapter to J32

Prepare the Demo
================
1.  Connect a mini/micro USB cable between the PC host and the OpenSDA USB port on the boards.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
6.  Download the program for master node to the target board.

Running the demo
================
Master starts communication on button SW2 press. First frame is sent from master to slave. Slave will update its frame buffer and continue to send its own frame. After that slave will repeate the response from master and master will send the new frame in the next turn. The next two frames are of diagnostic type and could be used for customer purposes (for example LIN cluster go to sleep command). This sequence is than repeated.

If communication is succesfull the master and slave node will both repeatedly print:

LIN DEMO
SLAVE
LIN DEMO
MASTER
Customization options
=====================

