# pdm_standalone_transfer_edma

## Overview
The pdm multi channel standalone transfer example shows how to use pdm driver with edma to receive multi DMIC data into standalone buffer:

In this example, pdm will trigger edma to transfer data when one PDM channel watermark value is reached, the DMIC data format,
 ----------------------------------------------------------------------------------------------------------------------
 |CHANNEL0 | CHANNEL0 | CHANNEL0 | .... | CHANNEL1 | CHANNEL 1 | CHANNEL1 |....| CHANNEL2 | CHANNEL 2 | CHANNEL2 |....|
 ----------------------------------------------------------------------------------------------------------------------

## Supported Boards
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/pdm/pdm_standalone_transfer_edma/example_board_readme.md)
