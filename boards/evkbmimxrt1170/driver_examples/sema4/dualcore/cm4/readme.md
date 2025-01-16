# sema4_dualcore

## Overview
This example shows how to use the SEMA4 gate lock and unlock, and the gate notify
feature. In this example, core 0 sends command to core 1 through MU. According to
the commands, core 1 locks and unlocks the specific SEMA4 gate at the proper time.
If core 0 trys to lock a SEMA4 gate which has been locked by core 1, the opeartion
fails, and core 0 is notified when the SEMA4 gate is unlocked by core 1. This
example demonstrates this feature.

## Supported Boards
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/sema4/dualcore/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/sema4/dualcore/example_board_readme.md)
