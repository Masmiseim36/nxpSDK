# flexcan_efifo_interrupt_transfer

## Overview
The flexcan_efifo_interrupt_transfer example shows how to use FlexCAN Enhanced Rx FIFO in none-blocking interrupt way:

In this example, when setting ENABLE_LOOPBACK macro, only one board is needed. The example will config one FlexCAN Message
Buffer to Tx Message Buffer and setup Enhanced Rx FIFO. After that, the example will send 4 CAN FD Messages
from Tx Message Buffer to Enhanced Rx FIFO through internal loopback interconnect. The sent and received message will be print 
out to terminal

2 boards are required to be connected through CAN bus if ENABLE_LOOPBACK is not defined. Endpoint A(board A) send a CAN Message to
Endpoint B(board B) when user press space key in terminal. Endpoint B receives the message by Enhanced Rx FIFO, and print
the message content to terminal after receive 4 CAN FD messages.

## Supported Boards
- [EVK9-MIMX8ULP](../../../_boards/evk9mimx8ulp/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [EVK-MIMX8ULP](../../../_boards/evkmimx8ulp/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [FRDM-MCXW71](../../../_boards/frdmmcxw71/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [MCX-W71-EVK](../../../_boards/mcxw71evk/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [IMX95LP4XEVK-15](../../../_boards/imx95lp4xevk15/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [IMX95LPD5EVK-19](../../../_boards/imx95lpd5evk19/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [imx95verdinevk](../../../_boards/imx95verdinevk/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [FRDMIMX95](../../../_boards/frdmimx95/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../../_boards/mcimx93autoevk/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [MCIMX93-EVK](../../../_boards/mcimx93evk/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [MCIMX93W-EVK](../../../_boards/mcimx93wevk/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [MCIMX93-QSB](../../../_boards/mcimx93qsb/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [MCX-W72-EVK](../../../_boards/mcxw72evk/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [FRDM-MCXE31B](../../../_boards/frdmmcxe31b/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [KW47-LOC](../../../_boards/kw47loc/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [MCXW72-LOC](../../../_boards/mcxw72loc/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [FRDM-IMXRT1186](../../../_boards/frdmimxrt1186/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
- [FRDM-MCXW72](../../../_boards/frdmmcxw72/driver_examples/flexcan/efifo_interrupt_transfer/example_board_readme.md)
