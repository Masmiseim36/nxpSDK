# flexspi_nor_dma_transfer

## Overview
The flexspi_nor_dma_transfer example shows how to use flexspi driver with dma:

In this example, flexspi will send data and operate the external nor flash connected with FLEXSPI. Some simple flash command will
be executed, such as Write Enable, Erase sector, Program page.
Example will first erase the sector and program a page into the flash, at last check if the data in flash is correct.

## Supported Boards
- [FRDM-RW612](../../../../_boards/frdmrw612/driver_examples/flexspi/nor/dma_transfer/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../../_boards/mimxrt685audevk/driver_examples/flexspi/nor/dma_transfer/example_board_readme.md)
- [RD-RW612-BGA](../../../../_boards/rdrw612bga/driver_examples/flexspi/nor/dma_transfer/example_board_readme.md)
