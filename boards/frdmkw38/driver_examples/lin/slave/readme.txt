Overview
========
The LIN (Local Interconnect Network) demo application is a use-case of LIN low level driver built on Low Power UART. This application demonstrates a signal transfer between the two nodes. Master node and slave node signals are represented by the character array. Both master and slave node must have implemented the timer that is used by timeout service routine every 500 us and master node use the same timer to check for switch of the frame table scheduler every 5 ms.
The application and driver use the state machines to handle various of states of the bus on different levels.
User can use the buttons to sleep and wakeup the master node which is indicated by LED state.
This application supports only unconditional frame types.


Toolchain supported
===================
- IAR embedded Workbench  8.32.4
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Two Mini/micro USB cables
- Two FRDM-KW38 boards
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

