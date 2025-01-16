# src_reset_source

## Overview

The src_reset_source example shows how to check the reset source and boot option with SRC driver.

When run this example firstly on the board after power up, the POR reset or IPP reset flag would be asserted. But when running it again, the flag would be cleared, since it is no longer a reset.

Usually, the reset flags are used and cleared by ROM code or bootloader to check which reset occurred. With the boot mode words, the initial boot code would go to the right way to execute the start code. For example, when the SoC is waked up from some low power mode, all necessary data are already staying in the RAM, the CPU would not have to reload the program to RAM and execute the application directly.

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/driver_examples/src/src_reset_source/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/src/src_reset_source/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/driver_examples/src/src_reset_source/example_board_readme.md)
- [EVK-MIMXRT1010](../../../_boards/evkmimxrt1010/driver_examples/src/src_reset_source/example_board_readme.md)
- [EVK-MIMXRT1015](../../../_boards/evkmimxrt1015/driver_examples/src/src_reset_source/example_board_readme.md)
- [EVK-MIMXRT1020](../../../_boards/evkmimxrt1020/driver_examples/src/src_reset_source/example_board_readme.md)
- [MIMXRT1024-EVK](../../../_boards/evkmimxrt1024/driver_examples/src/src_reset_source/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/driver_examples/src/src_reset_source/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/driver_examples/src/src_reset_source/example_board_readme.md)
