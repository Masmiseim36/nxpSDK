# flexcan_loopback_edma_transfer

## Overview
The flexcan_loopback_edma example shows how to use the EDMA version transactional driver to receive
CAN Message from Rx FIFO:

To demonstrates this example, only one board is needed. The example will config one FlexCAN Message
Buffer to Tx Message Buffer and also setup Rx FIFO. After that, the example will send 4 CAN Messages
from Tx Message Buffer to Rx FIFO through internal loopback interconnect and read them out using
EDMA version FlexCAN transactional driver. The Sent and received message will be print out to terminal
at last.

## Supported Boards
- [EVK9-MIMX8ULP](../../../_boards/evk9mimx8ulp/driver_examples/flexcan/loopback_edma_transfer/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/flexcan/loopback_edma_transfer/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/driver_examples/flexcan/loopback_edma_transfer/example_board_readme.md)
- [EVK-MIMX8ULP](../../../_boards/evkmimx8ulp/driver_examples/flexcan/loopback_edma_transfer/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/driver_examples/flexcan/loopback_edma_transfer/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/flexcan/loopback_edma_transfer/example_board_readme.md)
- [FRDM-MCXW71](../../../_boards/frdmmcxw71/driver_examples/flexcan/loopback_edma_transfer/example_board_readme.md)
- [IMX95LP4XEVK-15](../../../_boards/imx95lp4xevk15/driver_examples/flexcan/loopback_edma_transfer/example_board_readme.md)
- [IMX95LPD5EVK-19](../../../_boards/imx95lpd5evk19/driver_examples/flexcan/loopback_edma_transfer/example_board_readme.md)
- [imx95verdinevk](../../../_boards/imx95verdinevk/driver_examples/flexcan/loopback_edma_transfer/example_board_readme.md)
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/flexcan/loopback_edma_transfer/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/flexcan/loopback_edma_transfer/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../../_boards/mcimx93autoevk/driver_examples/flexcan/loopback_edma_transfer/example_board_readme.md)
- [MCIMX93-EVK](../../../_boards/mcimx93evk/driver_examples/flexcan/loopback_edma_transfer/example_board_readme.md)
- [MCIMX93-QSB](../../../_boards/mcimx93qsb/driver_examples/flexcan/loopback_edma_transfer/example_board_readme.md)
- [MCX-W72-EVK](../../../_boards/mcxw72evk/driver_examples/flexcan/loopback_edma_transfer/example_board_readme.md)
