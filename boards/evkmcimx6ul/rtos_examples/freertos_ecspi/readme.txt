Overview
========
The freertos_ecspi example shows how to use ECSPI driver in FreeRTOS:

In this example , one ecspi instance is used as ECSPI master with blocking and another ecspi instance is used as ECSPI slave.

1. ECSPI master sends/receives data using task blocking calls to/from ECSPI slave. (ECSPI Slave uses interrupt to receive/send
the data)

The example supports board to board connection.

With board to board connection, one ECSPI instance on one board is used as ECSPI master and the ECSPI instance on other board is used as ECSPI slave. Tasks are created to run on each board to handle ECSPI communication.
    File freertos_ecspi.c should have following definitions:
    #define EXAMPLE_CONNECT_ECSPI BOARD_TO_BOARD
    For board used as ECSPI master:
        #define ECSPI_MASTER_SLAVE isMASTER
    For board used as ECSPI slave:
        #define ECSPI_MASTER_SLAVE isSLAVE


Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL board
- JLink Plus
- 5V power supply
- Personal Computer

Board settings
==============
ECSPI board to board:
Before start this example, please make sure the R1724/R1725/R1726/R1727 are jointed and the 
R1633 is unjointed on board MCIMX6UL-EVK -BB due to the the pin multiplexing. 
Transfers data through the instance 4 of the ECSPI interface. ECSPI4 pins of the master board are
connected to the ECSPI4 pins of the slave board.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER BOARD           CONNECTS TO         SLAVE BOARD
Pin Name    Board Location               Pin Name  Board Location
MISO        J1704 - 5                       MISO      J1704 - 5
MOSI        J1704 - 4                       MOSI      J1704 - 4
SCK         J1704 - 6                       SCK       J1704 - 6
SS0         J1704 - 3                       SS0       J1704 - 3
GND         J1704 - 7                       GND       J1704 - 7
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect 5V power supply and JLink Plus to the board, switch SW2001 to power on the board.
2.  Connect a USB cable between the host PC and the J1901 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, you can see the similar information on the terminal as below.

For master:

FreeRTOS ECSPI example start.
This example use two boards to connect with one as master and anohter as slave.
Master and slave uses interrupt way. Slave should start first. 
Please make sure you make the correct line connection. Basically, the connection is: 
ECSPI_master -- ECSPI_slave   
   CLK      --    CLK  
   PCS      --    PCS 
   MOSI     --    MOSI 
   MISO     --    MISO 
   GND      --    GND 
ECSPI master transfer completed successfully. 

ECSPI transfer all data matched!

For slave:

FreeRTOS ECSPI example start.
This example use two boards to connect with one as master and anohter as slave.
Master and slave uses interrupt way. Slave should start first. 
Please make sure you make the correct line connection. Basically, the connection is: 
ECSPI_master -- ECSPI_slave   
   CLK      --    CLK  
   PCS      --    PCS 
   MOSI     --    MOSI 
   MISO     --    MISO 
   GND      --    GND 
ECSPI slave transfer completed successfully.

ECSPI transfer all data matched!

Customization options
=====================

