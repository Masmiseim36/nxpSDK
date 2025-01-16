# canfd_loopback_transfer

## Overview
The flexcan_loopback example shows how to use the loopback test mode to debug your CAN Bus design:

To demonstrates this example, only one board is needed. The example will config one FlexCAN Message
Buffer to Rx Message Buffer and the other FlexCAN Message Buffer to Tx Message Buffer with same ID.
After that, the example will send a CAN Message from the Tx Message Buffer to the Rx Message Buffer
through internal loopback interconnect and print out the Message payload to terminal.

## Supported Boards
- [EVK9-MIMX8ULP](../../../_boards/evk9mimx8ulp/driver_examples/canfd/loopback_transfer/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/canfd/loopback_transfer/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/canfd/loopback_transfer/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/driver_examples/canfd/loopback_transfer/example_board_readme.md)
- [EVK-MIMX8ULP](../../../_boards/evkmimx8ulp/driver_examples/canfd/loopback_transfer/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/driver_examples/canfd/loopback_transfer/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/driver_examples/canfd/loopback_transfer/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/canfd/loopback_transfer/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/canfd/loopback_transfer/example_board_readme.md)
- [IMX95LP4XEVK-15](../../../_boards/imx95lp4xevk15/driver_examples/canfd/loopback_transfer/example_board_readme.md)
- [IMX95LPD5EVK-19](../../../_boards/imx95lpd5evk19/driver_examples/canfd/loopback_transfer/example_board_readme.md)
- [imx95verdinevk](../../../_boards/imx95verdinevk/driver_examples/canfd/loopback_transfer/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../../_boards/mcimx93autoevk/driver_examples/canfd/loopback_transfer/example_board_readme.md)
- [MCIMX93-EVK](../../../_boards/mcimx93evk/driver_examples/canfd/loopback_transfer/example_board_readme.md)
- [MCIMX93-QSB](../../../_boards/mcimx93qsb/driver_examples/canfd/loopback_transfer/example_board_readme.md)
