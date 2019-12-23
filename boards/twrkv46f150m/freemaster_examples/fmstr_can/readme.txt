Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KV46F150M board
- TWR-SER board
- TWR-ELEV pair of boards
- Supported CAN interface (IXXAT, Vector, Kvaser, etc.)
- Personal Computer
- FreeMASTER 2.5 or later installed on PC

Board settings
============
Refer to Tower module User Guide for more information
about configuring power supply input pins. 

CAN pins are routed to primary elevator connector:
- PORTA12 is connected to CAN0_TX (elevator pin B42)
- PORTA13 is connected to CAN0_RX (elevator pin B41)

TWR-SER Tower System module configuration:
- Short J5(1-2), J5(3-4), J5(5-6), and J5(7-8) to enable CAN connection.
- Short J5(9-10) to connect CAN bus termination resistor

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
