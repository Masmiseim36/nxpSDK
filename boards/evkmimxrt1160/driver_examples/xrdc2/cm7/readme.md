# xrdc2

## Overview
The xrdc2 example shows how to control the memory slot, memory region, and
peripheral access policy using XRDC2.

In this example, the memory slot, memory region, and peripheral are set to
unaccessible. Then access to them triggers hardfault or bus fault. In the
exception handler, theses resources are set to accessible, then the previous
access continues and success.

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/driver_examples/xrdc2/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/driver_examples/xrdc2/example_board_readme.md)
