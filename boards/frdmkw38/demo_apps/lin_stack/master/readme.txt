Overview
========
The LIN (Local Interconnect Network) demo application is a use-case of LIN stack driver built on Low Power UART. 
This application demonstrates a signal transfer between the two nodes. Master node and slave node signals. Both 
master and slave node must have implemented the timer that is used by timeout service routine every 500 us and 
master node use the same timer to check for switch of the frame table scheduler every 5 ms, but the real period between
frames may larger than 5ms according to the configurations.

User can use the SW3 button to set master node to sleep and SW2 button to wakeup the master node.


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
If communication is succesfull, users will see the LED blinky about every 2.5ms, the master node will print:
~~~~~~~~~~~~
LIN master demo start.
LIN master will receive data from slave node, and send control command to slave.
 -> LED 2 is ON!
 -> LED 1 is ON!
 -> LED 2 is ON!
 -> LED 1 is ON!
 -> LED 2 is ON!

~~~~~~~~~~~~
 
Customization options
=====================

