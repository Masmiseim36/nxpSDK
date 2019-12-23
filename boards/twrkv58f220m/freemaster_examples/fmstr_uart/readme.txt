Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KV58F220M board
- TWR-SER board + TWR-ELEV pair (optionally)
- Personal Computer
- FreeMASTER 2.5 or later installed on PC

Board settings
============
Refer to TWR-KV58F220M Tower module User Guide for more information
about configuring power supply input pins. 

The following jumper settings will route UART0 to OpenSDA virtual serial port
- J24: 2-3
- J25: 2-3
The following jumper settings will route UART0 to elevator connector and to TWR-SER RS232 port
- J24: 1-2
- J25: 1-2

Optional TWR-SER Tower System module configuration (when TWR-SER RS232 serial is to be used)
- J17: 1-2
- J19: 1-2


Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Optionally: connect RS232 cable between the host PC and TWR-SER RS232 port
3.  Compile and download the program to the target board.

Running the demo
================
4.  Run FreeMASTER and use connection wizard to connect at 115200 bps.
5.  Click on "FreeMASTER Demonstration Project (embedded in device)" on the Welcome page
6.  Read more information about FreeMASTER tool at http://www.nxp.com/freemaster
