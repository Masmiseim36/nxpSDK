# dcic

## Overview

This example shows how to use DCIC to monitor the display content integrity.
In this example, DCIC monitors part of the screen, user could press input
through the debug terminal, to let the screen show two types of frames,
one matches the reference CRC (right frame), the other does not match (wrong frame).
User could measure the output signal pin, when the wrong frame is shown, the
output signal frequency is 1/4 of the frequency when right frame is shown.

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/driver_examples/dcic/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/driver_examples/dcic/example_board_readme.md)
