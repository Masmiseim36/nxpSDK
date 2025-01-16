# flexspi_hyper_ram_polling_transfer

## Overview
The flexspi_hyper_ram_polling_transfer example shows how to use flexspi driver with polling:

In this example, flexspi will send data and operate the external Hyper RAM connected with FLEXSPI. Some simple flash command will
be executed, such as Read ID, Read Data and Write Data.
Example will first Write 256 bytes of data from the start of Hyper ram, then read the data back. at last check if the data in flash is correct.

## Supported Boards
- [MIMXRT1180-EVK](../../../../_boards/evkmimxrt1180/driver_examples/flexspi/hyper_ram/polling_transfer/example_board_readme.md)
