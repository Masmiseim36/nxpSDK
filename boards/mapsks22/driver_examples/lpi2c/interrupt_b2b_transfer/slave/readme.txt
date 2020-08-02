Overview
========
The lpi2c_interrupt_b2b_transfer_slave example shows how to use lpi2c driver as slave to do board to board transfer 
with interrupt:

In this example, one lpi2c instance as slave and another lpi2c instance on the other board as master. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct.

Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Micro USB cable
- Two MAPS-KS22F256 board & two MAPS-DOCK board
- Personal Computer

Board settings
==============
The lpi2c_interrupt example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial
device work, the jumpers on MAPS-DOCK board should be set as following:
- JP7 on MAPS-Dock: UART0 related jumpers connected

To make lpi2c example work, connections on two MAPS-KS22F256 boards need to be setup as follows:
- PTB0(Pin53) --> PTB0(Pin53);
- PTB1(Pin54) --> PTB1(Pin54);
- JP13 Connected
- JP14 Connected

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
LPI2C board2board interrupt example -- Slave transfer.


End of LPI2C example .
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

