Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K64F board
- 3.3V CAN transceiver
- Supported CAN interface (IXXAT, Vector, Kvaser, etc.)
- Personal Computer
- FreeMASTER 2.5 or later installed on PC

Board settings
============
CAN transceiver connected to pins:
- PORTB18 (pin 64) is connected to CAN0_TX
- PORTB19 (pin 65) is connected to CAN0_RX

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Connect CAN transceiver to CAN interface on PC.
3.  Compile and download the program to the target board.

Running the demo
================
4.  Run FreeMASTER and use connection wizard to connect via FreeMASTER-over-CAN communication plugin at 500kbps.
5.  Click on "FreeMASTER Demonstration Project (embedded in device)" on the Welcome page
6.  Read more information about FreeMASTER tool at http://www.nxp.com/freemaster
