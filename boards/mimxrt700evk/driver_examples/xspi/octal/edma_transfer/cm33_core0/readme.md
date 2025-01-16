# xspi_octal_edma_transfer

## Overview
The xspi_octal_edma_transfer example shows how to use xspi driver with dma:

In this example, xspi will send data and operate the external octal flash connected with XSPI. Some simple flash command will be executed, such as Write Enable, Erase sector, Program page.
Example will first erase the sector and program a page into the flash, at last check if the data in flash is correct.

## Supported Boards
- [MIMXRT700-EVK](../../../../_boards/mimxrt700evk/driver_examples/xspi/octal/edma_transfer/example_board_readme.md)
