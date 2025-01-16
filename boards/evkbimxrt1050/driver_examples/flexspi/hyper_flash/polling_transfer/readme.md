# flexspi_hyper_flash_polling_transfer

## Overview
The flexspi_hyper_flash_polling_transfer example shows how to use flexspi driver with polling:

In this example, flexspi will send data and operate the external Hyper flash connected with FLEXSPI. Some simple flash command will
be executed, such as Read ID, Erase Sector and Program Buffer.
Example will first configures hyper flash to enter ASO mode, read ID-CFI parameters, then exit ASO mode.
Second, the example erase a sector in flash, check if the erase is successful and program the same sector, then read back
the sector data. At last check if the data read back is correct.

## Supported Boards
- [EVKB-IMXRT1050](../../../../_boards/evkbimxrt1050/driver_examples/flexspi/hyper_flash/polling_transfer/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../../_boards/evkbmimxrt1060/driver_examples/flexspi/hyper_flash/polling_transfer/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../../_boards/evkcmimxrt1060/driver_examples/flexspi/hyper_flash/polling_transfer/example_board_readme.md)
- [EVK-MIMXRT1064](../../../../_boards/evkmimxrt1064/driver_examples/flexspi/hyper_flash/polling_transfer/example_board_readme.md)
