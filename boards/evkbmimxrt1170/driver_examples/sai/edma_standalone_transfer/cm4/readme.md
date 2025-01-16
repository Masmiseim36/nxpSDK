# sai_edma_standalone_transfer

## Overview
The sai_edma_standalone_transfer example shows how to use sai driver with EDMA:

In this example, a sai instance records audio data from the input and processes its data as follows, Then restore the processed data and play it.

original data:
 --------------------------------------------------------------------------------------------------
 |LEFT CHANNEL | RIGHT CHANNEL | LEFT CHANNEL | RIGHT CHANNEL | LEFT CHANNEL | RIGHT CHANNEL | ...|
 --------------------------------------------------------------------------------------------------
received data:
 -------------------------------------------------------------------------------------------------------
 |LEFT CHANNEL | LEFT CHANNEL | LEFT CHANNEL | ...| RIGHT CHANNEL | RIGHT CHANNEL | RIGHT CHANNEL | ...|
 -------------------------------------------------------------------------------------------------------
 sent data:
 --------------------------------------------------------------------------------------------------
 |LEFT CHANNEL | RIGHT CHANNEL | LEFT CHANNEL | RIGHT CHANNEL | LEFT CHANNEL | RIGHT CHANNEL | ...|
 --------------------------------------------------------------------------------------------------

## Supported Boards
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/sai/edma_standalone_transfer/example_board_readme.md)
