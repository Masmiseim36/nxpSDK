# usart_9bit_interrupt_transfer

## Overview
The usart_9bit_interrupt_transfer example shows how to use usart driver in 9-bit mode in multi-slave system.
Master can send data to slave with certain address specifically, and slave can only receive data when it is addressed.

In this example, one usart instance is used with address configured. Its TX and RX pins are connected together.
First it sends a piece of data out, then addresses itself, after that sends the other piece of data. Only data
sent after the address can be received by itself.

## Supported Boards
- [EVK-MIMXRT685](../../../../_boards/evkmimxrt685/driver_examples/usart/9bit_interrupt_transfer/example_board_readme.md)
- [LPCXpresso51U68](../../../../_boards/lpcxpresso51u68/driver_examples/usart/9bit_interrupt_transfer/example_board_readme.md)
- [LPCXpresso54628](../../../../_boards/lpcxpresso54628/driver_examples/usart/9bit_interrupt_transfer/example_board_readme.md)
- [LPCXpresso54S018](../../../../_boards/lpcxpresso54s018/driver_examples/usart/9bit_interrupt_transfer/example_board_readme.md)
- [LPCXpresso54S018M](../../../../_boards/lpcxpresso54s018m/driver_examples/usart/9bit_interrupt_transfer/example_board_readme.md)
- [LPCXpresso55S06](../../../../_boards/lpcxpresso55s06/driver_examples/usart/9bit_interrupt_transfer/example_board_readme.md)
- [LPCXpresso55S16](../../../../_boards/lpcxpresso55s16/driver_examples/usart/9bit_interrupt_transfer/example_board_readme.md)
- [LPCXpresso55S28](../../../../_boards/lpcxpresso55s28/driver_examples/usart/9bit_interrupt_transfer/example_board_readme.md)
- [LPCXpresso55S69](../../../../_boards/lpcxpresso55s69/driver_examples/usart/9bit_interrupt_transfer/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../../_boards/mimxrt685audevk/driver_examples/usart/9bit_interrupt_transfer/example_board_readme.md)
