Overview
========
The lpspi_functional_interrupt example shows how to use LPSPI driver in interrupt way:

In this example , one lpspi instance used as LPSPI master and another lpspi instance used as LPSPI slave .

1. LPSPI master send/received data to/from LPSPI slave in interrupt . (LPSPI Slave using interrupt to receive/send the data)



Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Micro USB cable
- TWR-KE18F board
- Primary Elevator
- Personal Computer

Board settings
==============
The example requires connecting between LPSPI1 pins and LPSPI0 pins
The connection should be set as following:
- B7-Elevator, B48-Elevator connected
- B9-Elevator, B46-Elevator connected
- B10-Elevator, B44-Elevator connected
- B11-Elevator, B45-Elevator connected

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
LPSPI functional interrupt example start.
This example use one lpspi instance as master and another as slave on one board.
Master uses interrupt way and slave uses interrupt way.
Note that some LPSPI instances interrupt is in INTMUX ,you should set the intmux when you porting this example accordingly
Please make sure you make the correct line connection. Basically, the connection is:
LPSPI_master -- LPSPI_slave
   CLK      --    CLK
   PCS      --    PCS
   SOUT     --    SIN
   SIN      --    SOUT

LPSPI transfer all data matched!
End of example.
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

