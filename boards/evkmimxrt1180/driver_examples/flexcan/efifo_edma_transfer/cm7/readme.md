# flexcan_efifo_edma_transfer

## Overview
The flexcan_efifo_edma_transfer example shows how to use the EDMA version transactional driver to receive
CAN FD Message from Enhanced Rx FIFO:

In this example, when setting ENABLE_LOOPBACK macro, only one board is needed. The example will config one FlexCAN Message
Buffer to Tx Message Buffer and setup Enhanced Rx FIFO. After that, the example will send 4 CAN FD Messages
from Tx Message Buffer to Enhanced Rx FIFO through internal loopback interconnect and read them out using
EDMA version FlexCAN transactional driver. The sent and received message will be print out to terminal.

2 boards are required to be connected through CAN bus if ENABLE_LOOPBACK is not defined. Endpoint A(board A) send a CAN Message to
Endpoint B(board B) when user press space key in terminal. Endpoint B receives the message by Enhanced Rx FIFO and reads 
them out using EDMA version FlexCAN transactional driver. The sent and received message will be print out to terminal.

## Supported Boards
- [EVK9-MIMX8ULP](../../../_boards/evk9mimx8ulp/driver_examples/flexcan/efifo_edma_transfer/example_board_readme.md)
- [EVK-MIMX8ULP](../../../_boards/evkmimx8ulp/driver_examples/flexcan/efifo_edma_transfer/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/flexcan/efifo_edma_transfer/example_board_readme.md)
- [FRDM-MCXW71](../../../_boards/frdmmcxw71/driver_examples/flexcan/efifo_edma_transfer/example_board_readme.md)
- [IMX95LP4XEVK-15](../../../_boards/imx95lp4xevk15/driver_examples/flexcan/efifo_edma_transfer/example_board_readme.md)
- [IMX95LPD5EVK-19](../../../_boards/imx95lpd5evk19/driver_examples/flexcan/efifo_edma_transfer/example_board_readme.md)
- [imx95verdinevk](../../../_boards/imx95verdinevk/driver_examples/flexcan/efifo_edma_transfer/example_board_readme.md)
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/flexcan/efifo_edma_transfer/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/flexcan/efifo_edma_transfer/example_board_readme.md)
- [MCX-W72-EVK](../../../_boards/mcxw72evk/driver_examples/flexcan/efifo_edma_transfer/example_board_readme.md)
