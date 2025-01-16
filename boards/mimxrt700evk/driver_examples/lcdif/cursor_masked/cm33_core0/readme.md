# lcdif_cursor_masked

## Overview
This example demonstrates how to use the LCDIF cursor in masked mode.
In this exapmle, the screen is devided into two parts: red and blue. A cursor
is moving in the screen, the cursor contains 4 parts like this:

          +-----------------------+
          |        Part 1         |
          +-----------------------+
          |        Part 2         |
          +-----------------------+
          |        Part 3         |
          +-----------------------+
          |        Part 4         |
          +-----------------------+

Part 1 is background color set by LCDIF_SetCursorColor, it is black.
Part 2 is the backgroud frame buffer color.
Part 3 is foreground color set by LCDIF_SetCursorColor, it is white.
Part 4 is the invertion of backgroud frame buffer color.

## Supported Boards
- [EVK-MIMXRT595](../../../_boards/evkmimxrt595/driver_examples/lcdif/lcdif_examples_readme.md)
- [MIMXRT700-EVK](../../../_boards/mimxrt700evk/driver_examples/lcdif/lcdif_examples_readme.md)
