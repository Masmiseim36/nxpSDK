Overview
========
The freertos_dspi example shows how to use DSPI driver in FreeRTOS:

In this example , one dspi instance used as DSPI master with blocking and another dspi instance used as DSPI slave .

1. DSPI master sends/receives data using task blocking calls to/from DSPI slave. (DSPI Slave uses interrupt to receive/
send the data)


Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KW24D512 board
- Personal Computer
- Elevator Tower

Code settings (in freertos_dspi.c file) :
For Master:

#define EXAMPLE_DSPI_MASTER_BASE (SPI0_BASE)
#define EXAMPLE_DSPI_SLAVE_BASE (SPI1_BASE)

#define EXAMPLE_DSPI_MASTER_IRQN (SPI0_IRQn)
#define EXAMPLE_DSPI_SLAVE_IRQN (SPI1_IRQn)

#define SPI_MASTER_SLAVE isMASTER
#endif

For Slave :

#define EXAMPLE_DSPI_MASTER_BASE (SPI1_BASE)
#define EXAMPLE_DSPI_SLAVE_BASE  (SPI0_BASE)

#define EXAMPLE_DSPI_MASTER_IRQN (SPI1_IRQn)
#define EXAMPLE_DSPI_SLAVE_IRQN  (SPI0_IRQn)

#define SPI_MASTER_SLAVE isSLAVE


Board settings
==============
SPI one board:
Transfer data from instance0 from one board to instance 0 of other board.
SPI0 pins of one board are connected with SPI0 pins of other board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER BOARD           CONNECTS TO         SLAVE BOARD
Pin Name   Jumper                         Pin Name   Jumper
MOSI       B45                            MISO       B44
MISO       B44                            MOSI       B45
SCK        B48                            SCK        B48
PCS0       B46                            PCS0       B46
GND        B2                             GND        B2
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
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, you can see the similar information on the terminal as below.

DSPI interrupt example start.
This example use one dspi instance as master and another as slave on one board.
Please make sure you make the correct line connection. Basically, the connection is:
DSPI_master -- DSPI_slave
   CLK      --    CLK
   PCS      --    PCS
   SOUT     --    SIN
   SIN      --    SOUT
DSPI master transfer completed successfully.

DSPI slave transfer completed successfully.

DSPI transfer all data matched!
Customization options
=====================

