Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K66F board
- 3.3V CAN transceiver
- Supported CAN interface (IXXAT, Vector, Kvaser, etc.)
- Personal Computer
- FreeMASTER 2.5 or later installed on PC

Board settings
============
CAN transceiver connected to pins:
- PORTB18 is connected to CAN0_TX
- PORTB19 is connected to CAN0_RX

Prepare the demo
===============
1.  Connect a USB cable between the host PC and the Debug USB port on the target board.
2.  Connect CAN transceiver to CAN interface on PC. Make sure the CAN bus is properly terminated.
3.  Compile and download the program to the target microcontroller.
4.  Run and resume application execution when debugger stops in the main() function.

Connect with FreeMASTER
=======================
5.  Run FreeMASTER, use the Connection Wizard or open Project Options.
6.  Select FreeMASTER-over-CAN communication plug-in at 500kbps.
7.  Start communication, FreeMASTER loads the initial TSA Active Content links in the Welcome page.
8.  Click the "FreeMASTER Demonstration Project (embedded in device)" in the Welcome page.
9.  The demo is now running, you should be able to watch variable values and graphs.

More information
================
Read more information about FreeMASTER tool at http://www.nxp.com/freemaster.
Feel free to ask questions and report issues at FreeMASTER's 
community page at https://community.nxp.com/community/freemaster
