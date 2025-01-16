Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1040-EVK board
- Personal Computer

Board settings
==============
Remove the M.2 external device if inserted on board
Weld 0Ω resistor to R346,R350,R356,R362.

To make the example work, connections needed to be as follows:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        SLAVE           connect to      MASTER
Pin Name   Board Location     Pin Name    Board Location
PCS0       J17-3                SIN       J33-6
SOUT       J17-4                SOUT      J33-4
SIN        J17-5                SCK       J33-3
SCK        J17-6                PCS0      J33-5
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
You can see the similar message shows following in the terminal if the example runs successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FLEXIO Master - LPSPI Slave interrupt example start.

This example use one flexio spi as master and one lpspi instance as slave on one board.

Master and slave are both use interrupt way.

Please make sure you make the correct line connection. Basically, the connection is:

FLEXIO_SPI_master -- LPSPI_slave

      CLK        --    CLK

      PCS        --    PCS

      SOUT       --    SIN

      SIN        --    SOUT

This is LPSPI slave call back.

FLEXIO SPI master <-> LPSPI slave transfer all data matched!

End of example.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
