# cmsis_usart_dma_transfer

## Overview
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage method please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_uart_edma_transfer example shows how to use uart cmsis  driver with EDMA:

In this example, one uart instance connect to PC through uart, the board will
send back all characters that PC send to the board.

Note: The example echo every 8 characters, so input 8 characters every time.

## Supported Boards
- [EVK-MIMXRT595](../../../_boards/evkmimxrt595/cmsis_driver_examples/usart/dma_transfer/example_board_readme.md)
- [EVK-MIMXRT685](../../../_boards/evkmimxrt685/cmsis_driver_examples/usart/dma_transfer/example_board_readme.md)
- [FRDM-RW612](../../../_boards/frdmrw612/cmsis_driver_examples/usart/dma_transfer/example_board_readme.md)
- [LPCXpresso51U68](../../../_boards/lpcxpresso51u68/cmsis_driver_examples/usart/dma_transfer/example_board_readme.md)
- [LPCXpresso54628](../../../_boards/lpcxpresso54628/cmsis_driver_examples/usart/dma_transfer/example_board_readme.md)
- [LPCXpresso54S018](../../../_boards/lpcxpresso54s018/cmsis_driver_examples/usart/dma_transfer/example_board_readme.md)
- [LPCXpresso54S018M](../../../_boards/lpcxpresso54s018m/cmsis_driver_examples/usart/dma_transfer/example_board_readme.md)
- [LPCXpresso55S06](../../../_boards/lpcxpresso55s06/cmsis_driver_examples/usart/dma_transfer/example_board_readme.md)
- [LPCXpresso55S16](../../../_boards/lpcxpresso55s16/cmsis_driver_examples/usart/dma_transfer/example_board_readme.md)
- [LPCXpresso55S28](../../../_boards/lpcxpresso55s28/cmsis_driver_examples/usart/dma_transfer/example_board_readme.md)
- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/cmsis_driver_examples/usart/dma_transfer/example_board_readme.md)
- [LPCXpresso55S69](../../../_boards/lpcxpresso55s69/cmsis_driver_examples/usart/dma_transfer/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../_boards/mimxrt685audevk/cmsis_driver_examples/usart/dma_transfer/example_board_readme.md)
- [RD-RW612-BGA](../../../_boards/rdrw612bga/cmsis_driver_examples/usart/dma_transfer/example_board_readme.md)
