# flexspi_octal_dma_transfer

## Overview
The flexspi_nor_dma_transfer example shows how to use flexspi driver with dma:

In this example, flexspi will send data and operate the external octal flash connected with FLEXSPI. Some simple flash command will
be executed, such as Write Enable, Erase sector, Program page.
Example will first erase the sector and program a page into the flash, at last check if the data in flash is correct.

## Supported Boards
- [EVK-MIMXRT595](../../../../_boards/evkmimxrt595/driver_examples/flexspi/octal/dma_transfer/example_board_readme.md)
- [EVK-MIMXRT685](../../../../_boards/evkmimxrt685/driver_examples/flexspi/octal/dma_transfer/example_board_readme.md)
- [LPCXpresso55S36](../../../../_boards/lpcxpresso55s36/driver_examples/flexspi/octal/dma_transfer/example_board_readme.md)
