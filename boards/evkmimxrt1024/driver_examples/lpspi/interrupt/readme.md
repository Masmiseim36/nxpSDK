# lpspi_interrupt

## Overview
The lpspi_functional_interrupt example shows how to use LPSPI driver in interrupt way:

In this example , one lpspi instance used as LPSPI master and another lpspi instance used as LPSPI slave .

1. LPSPI master send/received data to/from LPSPI slave in interrupt . (LPSPI Slave using interrupt to receive/send the data)


## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below.

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

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/driver_examples/lpspi/interrupt/example_board_readme.md)
- [EVK-MIMXRT1020](../../../_boards/evkmimxrt1020/driver_examples/lpspi/interrupt/example_board_readme.md)
- [MIMXRT1024-EVK](../../../_boards/evkmimxrt1024/driver_examples/lpspi/interrupt/example_board_readme.md)
- [FRDM-K32L2A4S](../../../_boards/frdmk32l2a4s/driver_examples/lpspi/interrupt/example_board_readme.md)
- [FRDM-K32L3A6](../../../_boards/frdmk32l3a6/driver_examples/lpspi/interrupt/example_board_readme.md)
- [FRDM-KE15Z](../../../_boards/frdmke15z/driver_examples/lpspi/interrupt/example_board_readme.md)
- [FRDM-MCXA153](../../../_boards/frdmmcxa153/driver_examples/lpspi/interrupt/example_board_readme.md)
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/driver_examples/lpspi/interrupt/example_board_readme.md)
- [FRDM-MCXA166](../../../_boards/frdmmcxa166/driver_examples/lpspi/interrupt/example_board_readme.md)
- [FRDM-MCXA276](../../../_boards/frdmmcxa276/driver_examples/lpspi/interrupt/example_board_readme.md)
- [FRDM-MCXN236](../../../_boards/frdmmcxn236/driver_examples/lpspi/interrupt/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/lpspi/interrupt/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/lpspi/interrupt/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/lpspi/interrupt/example_board_readme.md)
