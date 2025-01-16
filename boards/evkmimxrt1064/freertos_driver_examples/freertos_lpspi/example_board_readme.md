Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1064 board
- Personal Computer

Board settings
============
Remove the resistor R334 and weld 0Ω resistor to R278,R279,R280,R281.

LPSPI:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       MASTER           connect to           SLAVE
Pin Name   Board Location     Pin Name    Board Location
SOUT       J24-4                SIN       J24-2
SIN        J24-5                SOUT      J24-9
SCK        J24-6                SCK       J24-10
PCS0       J24-3                PCS0      J24-1
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
The following message shows in the terminal if the example runs successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FreeRTOS LPSPI example start.

This example use one lpspi instance as master and another as slave on a single board.

Master and slave are both use interrupt way.

Please make sure you make the correct line connection. Basically, the connection is:

LPSPI_master -- LPSPI_slave

    CLK      --    CLK

    PCS0     --    PCS0

    SOUT     --    SIN

    SIN      --    SOUT

LPSPI master transfer completed successfully.

Slave-to-master data verified ok.

LPSPI slave transfer completed successfully.

Master-to-slave data verified ok.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
