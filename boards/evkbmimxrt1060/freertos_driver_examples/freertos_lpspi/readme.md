# freertos_lpspi

## Overview
The lpspi_interrupt example shows how to use LPSPI driver in interrupt way:

In this example , one lpspi instance used as LPSPI master and another lpspi instance used as LPSPI slave.

1. LPSPI master send/received data to/from LPSPI slave in interrupt . (LPSPI Slave using interrupt to receive/send the data)

The example supports board to board connection.

With board to board connection, one LPSPI instance on one board is used as LPSPI master and the LPSPI instance on other board is used as LPSPI slave. Tasks are created to run on each board to handle LPSPI communication.
    File freertos_lpspi.c should have following definitions:
    #define EXAMPLE_CONNECT_SPI BOARD_TO_BOARD
    For board used as LPSPI master:
        #define SPI_MASTER_SLAVE isMASTER
    For board used as LPSPI slave:
        #define SPI_MASTER_SLAVE isSLAVE

## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below.
If runs on one single board:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FreeRTOS LPSPI example start.
This example use one lpspi instance as master and another as slave on a single board.
Master and slave are both use interrupt way.
Please make sure you make the correct line connection. Basically, the connection is:
LPSPI_master -- LPSPI_slave
    CLK      --    CLK
    PCS      --    PCS
    SOUT     --    SIN
    SIN      --    SOUT

LPSPI master transfer completed successfully.
LPSPI slave transfer completed successfully.
LPSPI transfer all data matched !
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
If runs on two boards, slave should start first.
Master side:
~~~~~~~~~~~~
FreeRTOS LPSPI example start.
This example use two boards to connect with one as master and anohter as slave.
Master and slave are both use interrupt way.
Please make sure you make the correct line connection. Basically, the connection is:
LPSPI_master -- LPSPI_slave
    CLK      --    CLK
    PCS      --    PCS
    SOUT     --    SIN
    SIN      --    SOUT

LPSPI master transfer completed successfully.
LPSPI transfer all data matched !
~~~~~~~~~~~~~
SLAVE side:
~~~~~~~~~~~~~
FreeRTOS LPSPI example start.
This example use two boards to connect with one as master and anohter as slave.
Master and slave are both use interrupt way.
Please make sure you make the correct line connection. Basically, the connection is:
LPSPI_master -- LPSPI_slave
    CLK      --    CLK
    PCS      --    PCS
    SOUT     --    SIN
    SIN      --    SOUT

LPSPI slave transfer completed successfully.
LPSPI transfer all data matched !
~~~~~~~~~~~~~~~

## Supported Boards
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/freertos_driver_examples/freertos_lpspi/example_board_readme.md)
- [MIMXRT1060-EVKB](../../_boards/evkbmimxrt1060/freertos_driver_examples/freertos_lpspi/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/freertos_driver_examples/freertos_lpspi/example_board_readme.md)
- [EVK-MIMXRT1020](../../_boards/evkmimxrt1020/freertos_driver_examples/freertos_lpspi/example_board_readme.md)
- [MIMXRT1024-EVK](../../_boards/evkmimxrt1024/freertos_driver_examples/freertos_lpspi/example_board_readme.md)
- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/freertos_driver_examples/freertos_lpspi/example_board_readme.md)
- [EVK-MIMXRT1064](../../_boards/evkmimxrt1064/freertos_driver_examples/freertos_lpspi/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/freertos_driver_examples/freertos_lpspi/example_board_readme.md)
- [FRDM-K32L2A4S](../../_boards/frdmk32l2a4s/freertos_driver_examples/freertos_lpspi/example_board_readme.md)
- [FRDM-K32L3A6](../../_boards/frdmk32l3a6/freertos_driver_examples/freertos_lpspi/example_board_readme.md)
- [FRDM-KE15Z](../../_boards/frdmke15z/freertos_driver_examples/freertos_lpspi/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/freertos_driver_examples/freertos_lpspi/example_board_readme.md)
