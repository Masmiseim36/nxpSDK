# dac12_fifo_interrupt

## Overview

The dac12_fifo_interrupt example shows how to use DAC12 FIFO interrupt.

When the DAC12 FIFO watermark interrupt is enabled firstly, the application would enter the DAC12 ISR immediately, since remaining FIFO data is less than the watermark. Then the FIFO would be feed inside the ISR. Then the DAC12 interrupt could be restrained. Once the DAC12 FIFO is triggered in while loop, the data in FIFO is read out, then it becomes less than the watermark, so the FIFO would be feed again in DAC12 ISR. 

With this example, user can define the DAC12 output array to generate the different wave output. Also the software trigger can be called in some timer ISR so that the DAC12 would output the analog signal in indicated period. Or even use the hardware trigger to release the CPU.

## Supported Boards
- [EVK9-MIMX8ULP](../../../_boards/evk9mimx8ulp/driver_examples/dac12/fifo_interrupt/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/dac12/fifo_interrupt/example_board_readme.md)
- [EVK-MCIMX7ULP](../../../_boards/evkmcimx7ulp/driver_examples/dac12/fifo_interrupt/example_board_readme.md)
- [EVK-MIMX8ULP](../../../_boards/evkmimx8ulp/driver_examples/dac12/fifo_interrupt/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/dac12/fifo_interrupt/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/dac12/fifo_interrupt/example_board_readme.md)
