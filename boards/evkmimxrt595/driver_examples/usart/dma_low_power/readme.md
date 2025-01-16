# usart_dma_low_power

## Overview
The usart_dma_low_power example shows how to use usart to wake up the system
in low power modes, and how to wake up for DMA only.
In this example, one usart instance connects to PC through usart, the board will
start receiving characters from PC and then go into low power mode. Each character
from PC will only wake up the DMA but the CPU keeps in low power mode. Once 8
characters were received, system will be woken up and echo the characters to PC.

## Supported Boards
- [EVK-MIMXRT595](../../../../_boards/evkmimxrt595/driver_examples/usart/dma_low_power/example_board_readme.md)
- [EVK-MIMXRT685](../../../../_boards/evkmimxrt685/driver_examples/usart/dma_low_power/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../../_boards/mimxrt685audevk/driver_examples/usart/dma_low_power/example_board_readme.md)
