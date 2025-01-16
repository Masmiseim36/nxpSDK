# freertos_lpspi_b2b_slave

## Overview
The freertos_lpspi_b2b_slave example shows how to use LPSPI driver in FreeRTOS.
In this example are required two boards, one board is used as LPSPI master on which
is runnuing freertos_lpspi_b2b_master and another board is used as LPSPI slave on which
is running freertos_lpspi_b2b_slave example.

Connection of boards is in section Board settings.

It is required to run first the slave demo.


## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below.
If runs on one single board:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FreeRTOS LPSPI slave example starts.
This example uses two boards to connect with one as master and anohter as slave.
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
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/freertos_driver_examples/freertos_lpspi_b2b/slave/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/freertos_driver_examples/freertos_lpspi_b2b/slave/example_board_readme.md)
- [EVK-MIMXRT1020](../../../_boards/evkmimxrt1020/freertos_driver_examples/freertos_lpspi_b2b/slave/example_board_readme.md)
- [MIMXRT1024-EVK](../../../_boards/evkmimxrt1024/freertos_driver_examples/freertos_lpspi_b2b/slave/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/freertos_driver_examples/freertos_lpspi_b2b/slave/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/freertos_driver_examples/freertos_lpspi_b2b/slave/example_board_readme.md)
- [FRDM-K32L2A4S](../../../_boards/frdmk32l2a4s/freertos_driver_examples/freertos_lpspi_b2b/slave/example_board_readme.md)
- [FRDM-K32L3A6](../../../_boards/frdmk32l3a6/freertos_driver_examples/freertos_lpspi_b2b/slave/example_board_readme.md)
- [FRDM-KE15Z](../../../_boards/frdmke15z/freertos_driver_examples/freertos_lpspi_b2b/slave/example_board_readme.md)
- [FRDM-KE17Z](../../../_boards/frdmke17z/freertos_driver_examples/freertos_lpspi_b2b/slave/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/freertos_driver_examples/freertos_lpspi_b2b/slave/example_board_readme.md)
- [MCX-W72-EVK](../../../_boards/mcxw72evk/freertos_driver_examples/freertos_lpspi_b2b/slave/example_board_readme.md)
