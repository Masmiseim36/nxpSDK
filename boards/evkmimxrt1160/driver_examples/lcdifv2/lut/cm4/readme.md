# lcdifv2_lut

## Overview
This example demonstrates how to use the LCDIF v2 color palette (LUT). In this
example the pixel format is 8-bit LUT index. Although the LUT supports 256 items
in this example only items 0-7 are used, because the frame buffer content are
restricted to 0-7.

When the example runs, a rectangle is moving in the screen, and
its color changes when touch border.

## Supported Boards
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/lcdifv2/lcdifv2_examples_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/lcdifv2/lcdifv2_examples_readme.md)
