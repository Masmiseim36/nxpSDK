# flexio3_spi_int_lpspi_transfer_master

## Overview
The flexio_spi_master_interrupt_lpspi_slave example shows how to use flexio spi master driver in interrupt way:

In this example, a flexio simulated master connect to a lpspi slave .


## Running the demo
You can see the similar message shows following in the terminal if the example runs successfully.

FLEXIO Master - LPSPI Slave interrupt example start.
This example use one flexio spi as master and one lpspi instance as slave on one board.
Master and slave are both use interrupt way.
Please make sure you make the correct line connection. Basically, the connection is:
FLEXI_SPI_master -- LPSPI_slave
      CLK        --    CLK
      PCS        --    PCS
      SOUT       --    SIN
      SIN        --    SOUT
This is LPSPI slave call back.
FLEXIO SPI master <-> LPSPI slave transfer all data matched!

End of example.

## Supported Boards
- [MIMXRT1040-EVK](../../../../../_boards/evkmimxrt1040/driver_examples/flexio3/spi/int_lpspi_transfer/master/example_board_readme.md)
- [EVK-MIMXRT1064](../../../../../_boards/evkmimxrt1064/driver_examples/flexio3/spi/int_lpspi_transfer/master/example_board_readme.md)
