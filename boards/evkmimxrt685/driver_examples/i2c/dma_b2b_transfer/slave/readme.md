# lpc_i2c_dma_b2b_transfer_slave

## Overview
The i2c_dma_b2b_transfer_slave example shows how to use i2c driver as slave to do board to board transfer 
with a DMA master:

In this example, one i2c instance as slave and another i2c instance on the other board as master. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct. I2C slave works as supporting board to help demonstrate master DMA trasnfer, actually uses interrupt
way.

## Supported Boards
- [EVK-MIMXRT595](../../../../_boards/evkmimxrt595/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
- [EVK-MIMXRT685](../../../../_boards/evkmimxrt685/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
- [FRDM-RW612](../../../../_boards/frdmrw612/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
- [LPCXpresso51U68](../../../../_boards/lpcxpresso51u68/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
- [LPCXpresso54628](../../../../_boards/lpcxpresso54628/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
- [LPCXpresso54S018](../../../../_boards/lpcxpresso54s018/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
- [LPCXpresso54S018M](../../../../_boards/lpcxpresso54s018m/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
- [LPCXpresso55S06](../../../../_boards/lpcxpresso55s06/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
- [LPCXpresso55S16](../../../../_boards/lpcxpresso55s16/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
- [LPCXpresso55S28](../../../../_boards/lpcxpresso55s28/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
- [LPCXpresso55S36](../../../../_boards/lpcxpresso55s36/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
- [LPCXpresso55S69](../../../../_boards/lpcxpresso55s69/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../../_boards/mimxrt685audevk/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
- [RD-RW612-BGA](../../../../_boards/rdrw612bga/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
