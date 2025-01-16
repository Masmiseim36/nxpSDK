# lpadc_dma

## Overview

The lpadc_dma example shows how to use ADC to trigger a DMA transfer.

In this example, user should indicate a channel to provide a voltage signal (can be controlled by user) as the LPADC's
sample input. When running the project, typing any key into debug console would trigger the conversion, the software trigger
API is called to start the conversion. When the ADC conversion is completed, it would trigger the DMA in ping-pong transfer
mode to move the ADC conversion result from ADC conversion data register to user indicated memory. Then the main loop waits
for the transfer to be done and print the latest ADC conversion word and ADC conversion value average to terminal.

## Supported Boards
- [EVK-MIMXRT595](../../../_boards/evkmimxrt595/driver_examples/lpadc/dma/example_board_readme.md)
- [EVK-MIMXRT685](../../../_boards/evkmimxrt685/driver_examples/lpadc/dma/example_board_readme.md)
- [LPCXpresso55S06](../../../_boards/lpcxpresso55s06/driver_examples/lpadc/dma/example_board_readme.md)
- [LPCXpresso55S16](../../../_boards/lpcxpresso55s16/driver_examples/lpadc/dma/example_board_readme.md)
- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/driver_examples/lpadc/dma/example_board_readme.md)
- [LPCXpresso55S69](../../../_boards/lpcxpresso55s69/driver_examples/lpadc/dma/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../_boards/mimxrt685audevk/driver_examples/lpadc/dma/example_board_readme.md)
