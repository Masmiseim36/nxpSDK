Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1040-EVK board
- Personal Computer

Board settings
============
Weld 0Ω resistor to R346,R350,R356,R362.

LPSPI:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       MASTER           connect to           SLAVE
Pin Name   Board Location     Pin Name    Board Location
SOUT       J17-4                SIN       J17-2
SIN        J17-5                SOUT      J17-9
SCK        J17-6                SCK       J17-10
PCS0       J17-3                PCS0      J17-1
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
