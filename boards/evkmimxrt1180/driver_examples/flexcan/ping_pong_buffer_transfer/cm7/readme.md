# flexcan_ping_pong_buffer_transfer

## Overview
The flexcan_pingpang_buffer_transfer example shows how to use the FlexCAN queue feature to create
2 simulate FIFOs that can receive CAN/CANFD frames:

In this example, 2 boards are connected through CAN bus. Endpoint A(board A) send CAN/CANFD messages to
Endpoint B(board B) when user inputs the number of CAN messages to be sent in terminal. Endpoint B uses
two receiving queues to receive messages in turn, and prints the message content and the receiving queue
number to the terminal after any queue is full.

## Supported Boards
- [EVK9-MIMX8ULP](../../../_boards/evk9mimx8ulp/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [EVK-MIMX8MP](../../../_boards/evkmimx8mp/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [EVK-MIMX8ULP](../../../_boards/evkmimx8ulp/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [FRDM-MCXA166](../../../_boards/frdmmcxa166/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [FRDM-MCXA276](../../../_boards/frdmmcxa276/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [FRDM-MCXN236](../../../_boards/frdmmcxn236/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [IMX95LP4XEVK-15](../../../_boards/imx95lp4xevk15/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [IMX95LPD5EVK-19](../../../_boards/imx95lpd5evk19/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [imx95verdinevk](../../../_boards/imx95verdinevk/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../../_boards/mcimx93autoevk/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [MCIMX93-EVK](../../../_boards/mcimx93evk/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [MCIMX93-QSB](../../../_boards/mcimx93qsb/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/flexcan/ping_pong_buffer_transfer/example_board_readme.md)
