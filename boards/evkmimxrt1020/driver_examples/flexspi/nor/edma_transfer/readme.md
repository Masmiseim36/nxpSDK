# flexspi_nor_edma_transfer

## Overview
The flexspi_nor_edma_transfer example shows how to use flexspi driver with dma:

In this example, flexspi will send data and operate the external nor flash connected with FLEXSPI. Some simple flash command will
be executed, such as Write Enable, Erase sector, Program page.
Example will first erase the sector and program a page into the flash, at last check if the data in flash is correct.

## Supported Boards
- [MIMXRT1060-EVKB](../../../../_boards/evkbmimxrt1060/driver_examples/flexspi/nor/edma_transfer/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../../_boards/evkbmimxrt1170/driver_examples/flexspi/nor/edma_transfer/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../../_boards/evkcmimxrt1060/driver_examples/flexspi/nor/edma_transfer/example_board_readme.md)
- [EVK-MIMXRT1010](../../../../_boards/evkmimxrt1010/driver_examples/flexspi/nor/edma_transfer/example_board_readme.md)
- [EVK-MIMXRT1015](../../../../_boards/evkmimxrt1015/driver_examples/flexspi/nor/edma_transfer/example_board_readme.md)
- [EVK-MIMXRT1020](../../../../_boards/evkmimxrt1020/driver_examples/flexspi/nor/edma_transfer/example_board_readme.md)
- [MIMXRT1024-EVK](../../../../_boards/evkmimxrt1024/driver_examples/flexspi/nor/edma_transfer/example_board_readme.md)
- [MIMXRT1040-EVK](../../../../_boards/evkmimxrt1040/driver_examples/flexspi/nor/edma_transfer/example_board_readme.md)
- [EVK-MIMXRT1064](../../../../_boards/evkmimxrt1064/driver_examples/flexspi/nor/edma_transfer/example_board_readme.md)
- [MIMXRT1160-EVK](../../../../_boards/evkmimxrt1160/driver_examples/flexspi/nor/edma_transfer/example_board_readme.md)
- [MIMXRT1180-EVK](../../../../_boards/evkmimxrt1180/driver_examples/flexspi/nor/edma_transfer/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../../../_boards/mcimx93autoevk/driver_examples/flexspi/nor/edma_transfer/example_board_readme.md)
- [MCIMX93-EVK](../../../../_boards/mcimx93evk/driver_examples/flexspi/nor/edma_transfer/example_board_readme.md)
- [MCIMX93-QSB](../../../../_boards/mcimx93qsb/driver_examples/flexspi/nor/edma_transfer/example_board_readme.md)
