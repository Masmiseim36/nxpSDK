# flexcan_legacy_fifo_edma_transfer

## Overview
The flexcan_legacy_fifo_edma_transfer example shows how to use the EDMA version transactional
driver to receive CAN Message from Legacy Rx FIFO.

In this example, when set ENABLE_LOOPBACK macro, only one board is needed. The example will
config one FlexCAN Message Buffer as Tx Message Buffer and setup Legacy Rx FIFO. After that,
example will send 6 CAN Messages from Tx Message Buffer to Legacy Rx FIFO through internal
loopback interconnect and read them out using EDMA version FlexCAN transactional driver. The
send and received message will be print out to terminal.

When not define ENABLE_LOOPBACK, need 2 boards are connected through CAN bus. Endpoint A
(board A) send a CAN Message to Endpoint B(board B) when user press space key in terminal.
Endpoint B receive the message by Legacy Rx FIFO and read them out using EDMA version FlexCAN
transactional driver. The send and received message will be print out to terminal.

## Supported Boards
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/flexcan/legacy_fifo_edma_transfer/example_board_readme.md)
