# sema42_dualcore

## Overview
The sema42 example shows how to use SEMA42 driver to lock and unlock a sema gate:

In this example:
1. Firstly, Core 0 turn on LED  and lock a sema gate then boot up Core 1 wake up.
2. Core 1 must be wait until Core 0 unlocks this sema gate to lock it.
3. After user press any key in terminal window, the sema gate will be unlocked by core 0,
then core 1 will lock it and turn off the LED

If the board does not have LED to show the status, then core 1 will send a flag
to core 0 when core 1 has locked the sema gate. Core 0 outputs the success log
after it received the flag.

## Supported Boards
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/sema42/dual_core/example_board_readme.md)
- [FRDM-K32L3A6](../../../_boards/frdmk32l3a6/driver_examples/sema42/dual_core/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/sema42/dual_core/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/sema42/dual_core/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/sema42/dual_core/example_board_readme.md)
- [MIMXRT700-EVK](../../../_boards/mimxrt700evk/driver_examples/sema42/dual_core/example_board_readme.md)
