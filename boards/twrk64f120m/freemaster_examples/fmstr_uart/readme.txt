Hardware requirements
=====================
- Mini/micro USB cable
- TWR-K64F120M board
- TWR-SER board + TWR-ELEV pair (optionally)
- Personal Computer
- FreeMASTER 2.5 or later installed on PC

Board settings
============
Refer to TWR-K64F120M Tower module User Guide (TWRK64F120MUG.pdf) for more information
about configuring power supply input pins. 

The following jumper settings will route UART1 to OpenSDA virtual serial port
- J10: 1-2
- J15: 1-2
The following jumper settings will route UART1 to elevator connector and to TWR-SER RS232 port
- J10: 2-3
- J15: 2-3

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
