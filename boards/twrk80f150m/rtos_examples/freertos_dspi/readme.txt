Overview
========
The freertos_dspi example shows how to use DSPI driver in FreeRTOS:

In this example , one dspi instance used as DSPI master with blocking and another dspi instance used as DSPI slave .

1. DSPI master sends/receives data using task blocking calls to/from DSPI slave. (DSPI Slave uses interrupt to receive/
send the data)


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini USB cable
- TWR-K80F150M board
- Personal Computer
- Elevator Tower

Code settings (in freertos_dspi.c file) : 
For Master:

#define EXAMPLE_DSPI_MASTER_BASE (SPI0_BASE)
#define EXAMPLE_DSPI_SLAVE_BASE (SPI1_BASE)

#define EXAMPLE_DSPI_MASTER_IRQN (SPI0_IRQn)
#define EXAMPLE_DSPI_SLAVE_IRQN  (SPI1_IRQn)

#define SINGLE_BOARD 0
#define BOARD_TO_BOARD 1

#define EXAMPLE_CONNECT_DSPI BOARD_TO_BOARD
#if (EXAMPLE_CONNECT_DSPI == BOARD_TO_BOARD)
#define isMASTER 0
#define isSLAVE 1
#define SPI_MASTER_SLAVE isMASTER
#endif

For Slave :

#define EXAMPLE_DSPI_MASTER_BASE (SPI1_BASE)
#define EXAMPLE_DSPI_SLAVE_BASE (SPI0_BASE)

#define EXAMPLE_DSPI_MASTER_IRQN (SPI1_IRQn)
#define EXAMPLE_DSPI_SLAVE_IRQN  (SPI0_IRQn)

#define SINGLE_BOARD 0
#define BOARD_TO_BOARD 1

#define EXAMPLE_CONNECT_DSPI BOARD_TO_BOARD
#if (EXAMPLE_CONNECT_DSPI == BOARD_TO_BOARD)
#define isMASTER 0
#define isSLAVE 1
#define SPI_MASTER_SLAVE isSLAVE
#endif


Board settings
==============
SPI one board:
Transfer data from instance0 from one board to instance 0 of other board.
SPI0 pins of one board are connected with SPI0 pins of other board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER BOARD           CONNECTS TO         SLAVE BOARD
Pin Name   Primary Elevator               Pin Name  Primary Elevator
MOSI       B44                            MISO      B45
MISO       B45                            MOSI      B44
SCK        B48                            SCK       B48
PCS0       B46                            PCS0      B46
GND        B2                             GND       B2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

MASTER side : 

FreeRTOS DSPI example start.
This example use one dspi instance as master and another as slave on one board.
Master and slave are both use interrupt way.
Please make sure you make the correct line connection. Basically, the connection is:
DSPI_master -- DSPI_slave
   CLK      --    CLK
   PCS0     --    PCS0
   SOUT     --    SIN
   SIN      --    SOUT
DSPI master transfer completed successfully.

DSPI transfer all data matched!


SLAVE side : 
FreeRTOS DSPI example start.
This example use one dspi instance as master and another as slave on one board.
Master and slave are both use interrupt way.
Please make sure you make the correct line connection. Basically, the connection is:
DSPI_master -- DSPI_slave
   CLK      --    CLK
   PCS0     --    PCS0
   SOUT     --    SIN
   SIN      --    SOUT

DSPI slave transfer completed successfully.

DSPI transfer all data matched!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

