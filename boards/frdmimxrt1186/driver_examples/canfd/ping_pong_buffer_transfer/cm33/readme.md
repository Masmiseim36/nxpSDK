# canfd_ping_pong_buffer_transfer

## Overview
The flexcan_pingpang_buffer_transfer example shows how to use the FlexCAN queue feature to create
2 simulate FIFOs that can receive CAN/CANFD frames:

In this example, 2 boards are connected through CAN bus. Endpoint A(board A) send CAN/CANFD messages to
Endpoint B(board B) when user inputs the number of CAN messages to be sent in terminal. Endpoint B uses
two receiving queues to receive messages in turn, and prints the message content and the receiving queue
number to the terminal after any queue is full.  

This example enables FlexCAN Individual RX Masking and Queue feature, which makes FlexCAN search for first 
free-to-receive matched Rx MBs and last non-free-to-receive Rx MBs.  
If not, matching winner is the first matched Rx MB.

Node A transmit frame with ID 0x321. Node B Rx message buffer ID is 0x21, so Node B match ID 0x21 frames.  
But masked by 0xFF in Receive Individual Mask register, Node B will match ID 0x321 frames.  

When queue 1 finish receive, update queue 1 individual ID mask to 0x7FF, can only receive frame with ID 0x21. So queue 1 will ignore the next messages and the queue 2 start to receive frame.  
When queue 2 finish receive, restore queue 1 individual ID mask to 0xFF, make it receive ID 0x321 frames again.  

This example also demonstrates how to create custom FlexCAN IRQ handler.  

This example limits maximum number of CAN messages to be send each time to 256.

## Supported Boards
- [EVK9-MIMX8ULP](../../../_boards/evk9mimx8ulp/driver_examples/canfd/ping_pong_buffer_transfer/example_board_readme.md)
- [IMX952LPD5EVK-19](../../../_boards/imx952evk/driver_examples/flexcan/canfd_ping_pong_buffer_transfer/example_board_readme.md)
- [IMX952LPD5EVK-15](../../../_boards/imx952evk/driver_examples/flexcan/canfd_ping_pong_buffer_transfer/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/canfd/ping_pong_buffer_transfer/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/canfd/ping_pong_buffer_transfer/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/driver_examples/canfd/ping_pong_buffer_transfer/example_board_readme.md)
- [EVK-MIMX8ULP](../../../_boards/evkmimx8ulp/driver_examples/canfd/ping_pong_buffer_transfer/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/driver_examples/canfd/ping_pong_buffer_transfer/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/driver_examples/canfd/ping_pong_buffer_transfer/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/canfd/ping_pong_buffer_transfer/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/canfd/ping_pong_buffer_transfer/example_board_readme.md)
- [IMX95LP4XEVK-15](../../../_boards/imx95lp4xevk15/driver_examples/canfd/ping_pong_buffer_transfer/example_board_readme.md)
- [IMX95LPD5EVK-19](../../../_boards/imx95lpd5evk19/driver_examples/canfd/ping_pong_buffer_transfer/example_board_readme.md)
- [imx95verdinevk](../../../_boards/imx95verdinevk/driver_examples/canfd/ping_pong_buffer_transfer/example_board_readme.md)
- [FRDMIMX95](../../../_boards/frdmimx95/driver_examples/canfd/ping_pong_buffer_transfer/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../../_boards/mcimx93autoevk/driver_examples/canfd/ping_pong_buffer_transfer/example_board_readme.md)
- [MCIMX93-EVK](../../../_boards/mcimx93evk/driver_examples/canfd/ping_pong_buffer_transfer/example_board_readme.md)
- [MCIMX93W-EVK](../../../_boards/mcimx93wevk/driver_examples/canfd/ping_pong_buffer_transfer/example_board_readme.md)
- [MCIMX93-QSB](../../../_boards/mcimx93qsb/driver_examples/canfd/ping_pong_buffer_transfer/example_board_readme.md)
- [FRDM-IMXRT1186](../../../_boards/frdmimxrt1186/driver_examples/canfd/ping_pong_buffer_transfer/example_board_readme.md)
