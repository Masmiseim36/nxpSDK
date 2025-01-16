# dac12_basic

## Overview

The dac12_basic example shows how to use DAC12 module simply as the general DAC12 converter.

When the DAC12's fifo feature is not enabled, Any write to the DATA register will replace the
data in the buffer and push data to analog conversion without trigger support.
In this example, it gets the value from terminal,
outputs the DAC12 output voltage through DAC12 output pin.

## Supported Boards
- [EVK9-MIMX8ULP](../../../_boards/evk9mimx8ulp/driver_examples/dac12/basic/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/dac12/basic/example_board_readme.md)
- [EVK-MCIMX7ULP](../../../_boards/evkmcimx7ulp/driver_examples/dac12/basic/example_board_readme.md)
- [EVK-MIMX8ULP](../../../_boards/evkmimx8ulp/driver_examples/dac12/basic/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/dac12/basic/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/dac12/basic/example_board_readme.md)
