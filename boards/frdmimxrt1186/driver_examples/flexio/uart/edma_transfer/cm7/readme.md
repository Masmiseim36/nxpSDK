# flexio_uart_edma_transfer

## Overview
The flexio_uart_edma example shows how to use flexio uart driver in edma way:

In this example, a flexio simulated uart connect to PC through USB-Serial, the board will send back all characters
that PC send to the board.

## Supported Boards
- [EVK9-MIMX8ULP](../../../../_boards/evk9mimx8ulp/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [EVKB-IMXRT1050](../../../../_boards/evkbimxrt1050/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../../_boards/evkbmimxrt1060/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../../_boards/evkbmimxrt1170/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../../_boards/evkcmimxrt1060/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [EVK-MCIMX7ULP](../../../../_boards/evkmcimx7ulp/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [EVK-MIMX8ULP](../../../../_boards/evkmimx8ulp/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [EVK-MIMXRT1010](../../../../_boards/evkmimxrt1010/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [EVK-MIMXRT1015](../../../../_boards/evkmimxrt1015/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [EVK-MIMXRT1020](../../../../_boards/evkmimxrt1020/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [MIMXRT1024-EVK](../../../../_boards/evkmimxrt1024/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [MIMXRT1040-EVK](../../../../_boards/evkmimxrt1040/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [EVK-MIMXRT1064](../../../../_boards/evkmimxrt1064/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [MIMXRT1160-EVK](../../../../_boards/evkmimxrt1160/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [MIMXRT1180-EVK](../../../../_boards/evkmimxrt1180/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [FRDM-K32L2A4S](../../../../_boards/frdmk32l2a4s/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [FRDM-K32L3A6](../../../../_boards/frdmk32l3a6/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [FRDM-KE15Z](../../../../_boards/frdmke15z/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [FRDM-KE17Z](../../../../_boards/frdmke17z/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [FRDM-KE17Z512](../../../../_boards/frdmke17z512/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [FRDM-MCXA156](../../../../_boards/frdmmcxa156/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [FRDM-MCXE247](../../../../_boards/frdmmcxe247/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [FRDM-MCXN236](../../../../_boards/frdmmcxn236/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [FRDM-MCXN947](../../../../_boards/frdmmcxn947/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [FRDM-MCXW71](../../../../_boards/frdmmcxw71/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [MCX-W71-EVK](../../../../_boards/mcxw71evk/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [KW45B41Z-EVK](../../../../_boards/kw45b41zevk/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [KW47-EVK](../../../../_boards/kw47evk/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [MCX-N5XX-EVK](../../../../_boards/mcxn5xxevk/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [MCX-N9XX-EVK](../../../../_boards/mcxn9xxevk/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [MCX-W72-EVK](../../../../_boards/mcxw72evk/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [MIMXRT700-EVK](../../../../_boards/mimxrt700evk/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [KW47-LOC](../../../../_boards/kw47loc/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [MCXW72-LOC](../../../../_boards/mcxw72loc/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [FRDM-IMXRT1186](../../../../_boards/frdmimxrt1186/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [FRDM-MCXA266](../../../../_boards/frdmmcxa266/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [FRDM-MCXA366](../../../../_boards/frdmmcxa366/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [FRDM-MCXW72](../../../../_boards/frdmmcxw72/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
- [FRDM-MCXA577](../../../../_boards/frdmmcxa577/driver_examples/flexio/uart/edma_transfer/example_board_readme.md)
