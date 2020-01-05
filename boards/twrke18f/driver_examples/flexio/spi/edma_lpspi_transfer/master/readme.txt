Overview
========
The flexio_spi_master_edma_lpspi_slave example shows how to use flexio spi master driver in edma way:

In this example, a flexio simulated master connect to a lpspi slave .



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
The example requires connecting the FLEXIO pins with the LPSPI pins
Insert TWR-KE18F board into Primary Elevator. The connection should be set as follows:
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
FLEXIO Master edma - LPSPI Slave interrupt example start.

This example use one flexio spi as master and one lpspi instance as slave on one board.

Master uses edma and slave uses interrupt way.

Please make sure you make the correct line connection. Basically, the connection is:

FLEXI_SPI_master -- LPSPI_slave

      CLK        --    CLK

      PCS        --    PCS

      SOUT       --    SIN

      SIN        --    SOUT

This is LPSPI slave call back.

FLEXIO SPI master <-> LPSPI slave transfer all data matched!

End of example.
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

