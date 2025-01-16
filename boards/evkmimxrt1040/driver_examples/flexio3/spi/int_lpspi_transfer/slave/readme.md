# flexio3_spi_int_lpspi_transfer_slave

## Overview
The flexio_spi_slave_interrupt_lpspi_master example shows how to use flexio spi slave driver in interrupt way:

In this example, a flexio simulated slave connect to a lpspi master.


## Running the demo
You can see the similar message shows following in the terminal if the example runs successfully.

LPSPI Master interrupt - FLEXIO SPI Slave interrupt example start.
This example use one lpspi instance as master and one flexio spi slave on one board.
Master and slave are both use interrupt way.
Please make sure you make the correct line connection. Basically, the connection is:
LPSPI_master -- FLEXIO_SPI_slave
   CLK      --    CLK
   PCS      --    PCS
   SOUT     --    SIN
   SIN      --    SOUT
This is FLEXIO SPI slave call back.
LPSPI master <-> FLEXIO SPI slave transfer all data matched!

End of Example.

## Supported Boards
- [MIMXRT1040-EVK](../../../../../_boards/evkmimxrt1040/driver_examples/flexio3/spi/int_lpspi_transfer/slave/example_board_readme.md)
- [EVK-MIMXRT1064](../../../../../_boards/evkmimxrt1064/driver_examples/flexio3/spi/int_lpspi_transfer/slave/example_board_readme.md)
