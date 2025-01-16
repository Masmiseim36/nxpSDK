# usart_dma_double_buffer_transfer

## Overview
This example shows how to use the DMA driver to implement a double buffer receive scheme from the USART

The example shows the double buffer constructed using two descriptors (g_pingpong_desc). These descriptors are cycled from one to the other.

Things to note :

- The descriptors of the ping pong transfer need to be aligned to size 16
- The inital transfer will perform the same job as first descriptor of ping pong, so the first linkeage is to go to g_pingpong_desc[1]
- g_pingpong_desc[1] then chains the g_pingpong_desc[0] as the next descriptor
- The properties are set up such that g_pingpong_desc[0] (and the initial configuration uses INTA to signal back to the callback)
- g_pingpong_desc[1] uses INTB to signal to the callback
- The scheduled callback uses this information to know which data was last written

A note on Performance :

The intent of this example is to illustrate how a double-buffer scheme can be implemented using the dma. The performance of this example will 
be limited to how quickly the echo printer can read-out the data from the ping pong buffer and display it. This means that the example will 
work well if characters are entered at a rate where the DMA callback to echo the string can keep up with the input stream. Connecting the USART
RX to a continuous fast speed will cause the DMA to fall behind.

## Supported Boards
- [EVK-MIMXRT595](../../../../_boards/evkmimxrt595/driver_examples/usart/dma_double_buffer_transfer/example_board_readme.md)
- [EVK-MIMXRT685](../../../../_boards/evkmimxrt685/driver_examples/usart/dma_double_buffer_transfer/example_board_readme.md)
- [FRDM-RW612](../../../../_boards/frdmrw612/driver_examples/usart/dma_double_buffer_transfer/example_board_readme.md)
- [LPCXpresso51U68](../../../../_boards/lpcxpresso51u68/driver_examples/usart/dma_double_buffer_transfer/example_board_readme.md)
- [LPCXpresso54628](../../../../_boards/lpcxpresso54628/driver_examples/usart/dma_double_buffer_transfer/example_board_readme.md)
- [LPCXpresso54S018](../../../../_boards/lpcxpresso54s018/driver_examples/usart/dma_double_buffer_transfer/example_board_readme.md)
- [LPCXpresso54S018M](../../../../_boards/lpcxpresso54s018m/driver_examples/usart/dma_double_buffer_transfer/example_board_readme.md)
- [LPCXpresso55S06](../../../../_boards/lpcxpresso55s06/driver_examples/usart/dma_double_buffer_transfer/example_board_readme.md)
- [LPCXpresso55S16](../../../../_boards/lpcxpresso55s16/driver_examples/usart/dma_double_buffer_transfer/example_board_readme.md)
- [LPCXpresso55S28](../../../../_boards/lpcxpresso55s28/driver_examples/usart/dma_double_buffer_transfer/example_board_readme.md)
- [LPCXpresso55S36](../../../../_boards/lpcxpresso55s36/driver_examples/usart/dma_double_buffer_transfer/example_board_readme.md)
- [LPCXpresso55S69](../../../../_boards/lpcxpresso55s69/driver_examples/usart/dma_double_buffer_transfer/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../../_boards/mimxrt685audevk/driver_examples/usart/dma_double_buffer_transfer/example_board_readme.md)
- [RD-RW612-BGA](../../../../_boards/rdrw612bga/driver_examples/usart/dma_double_buffer_transfer/example_board_readme.md)
