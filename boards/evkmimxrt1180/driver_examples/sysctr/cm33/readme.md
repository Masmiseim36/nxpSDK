# sysctr

## Overview

The sysctr example shows the usage of System Counter driver in application. The System Counter provides
a shared time base to multiple processors. It is intended for applications where the counter is always
powered on, and supports multiple unrelated clocks.

In this example, System Counter is clocked by 24MHz base clock. System Counter count value will be printed
at 3 second and 5 second.

## Supported Boards
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/driver_examples/sysctr/example_board_readme.md)
