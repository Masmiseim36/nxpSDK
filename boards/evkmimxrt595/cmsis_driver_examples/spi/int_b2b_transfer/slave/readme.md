# cmsis_spi_int_b2b_transfer_slave

## Overview
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage method please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_spi_interrupt_b2b_transfer_slave example shows how to use CMSIS spi driver as master to do board to board transfer 
with interrupt:

In this example, one spi instance as master and another spi instance on the other board as slave. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct.

## Supported Boards
- [EVK-MIMXRT595](../../../../../_boards/evkmimxrt595/cmsis_driver_examples/spi/int_b2b_transfer/slave/example_board_readme.md)
- [EVK-MIMXRT685](../../../../../_boards/evkmimxrt685/cmsis_driver_examples/spi/int_b2b_transfer/slave/example_board_readme.md)
- [FRDM-RW612](../../../../../_boards/frdmrw612/cmsis_driver_examples/spi/int_b2b_transfer/slave/example_board_readme.md)
- [LPCXpresso51U68](../../../../../_boards/lpcxpresso51u68/cmsis_driver_examples/spi/int_b2b_transfer/slave/example_board_readme.md)
- [LPCXpresso54628](../../../../../_boards/lpcxpresso54628/cmsis_driver_examples/spi/int_b2b_transfer/slave/example_board_readme.md)
- [LPCXpresso54S018](../../../../../_boards/lpcxpresso54s018/cmsis_driver_examples/spi/int_b2b_transfer/slave/example_board_readme.md)
- [LPCXpresso54S018M](../../../../../_boards/lpcxpresso54s018m/cmsis_driver_examples/spi/int_b2b_transfer/slave/example_board_readme.md)
- [LPCXpresso55S06](../../../../../_boards/lpcxpresso55s06/cmsis_driver_examples/spi/int_b2b_transfer/slave/example_board_readme.md)
- [LPCXpresso55S16](../../../../../_boards/lpcxpresso55s16/cmsis_driver_examples/spi/int_b2b_transfer/slave/example_board_readme.md)
- [LPCXpresso55S28](../../../../../_boards/lpcxpresso55s28/cmsis_driver_examples/spi/int_b2b_transfer/slave/example_board_readme.md)
- [LPCXpresso55S36](../../../../../_boards/lpcxpresso55s36/cmsis_driver_examples/spi/int_b2b_transfer/slave/example_board_readme.md)
- [LPCXpresso55S69](../../../../../_boards/lpcxpresso55s69/cmsis_driver_examples/spi/int_b2b_transfer/slave/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../../../_boards/mimxrt685audevk/cmsis_driver_examples/spi/int_b2b_transfer/slave/example_board_readme.md)
- [RD-RW612-BGA](../../../../../_boards/rdrw612bga/cmsis_driver_examples/spi/int_b2b_transfer/slave/example_board_readme.md)
