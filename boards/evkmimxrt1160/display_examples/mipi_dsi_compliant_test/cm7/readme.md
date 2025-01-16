# mipi_dsi_compliant_test

## Overview
This project is used for MIPI DSI compliant test.

For compliant test, the MIPI DSI works in video mode, there should be 3 types
of data pattern on data lane:

1. ... 11110000 11110000 ...
2. ... 11111000 11111000 ...
3. ... 00000111 00000111 ...

To meet this requirement, the MIPI DSI output pixel format is set to 24-bit format,
and the framebuffer format is XRGB8888. The whole framebuffer is divided into
three parts, one part for each data pattern.

When this project runs, if the panel is connected, the panel shows three regions
with different gray scale. The MIPI DSI signals could be connected to
compliant test instrument.

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/display_examples/mipi_dsi_compliant_test/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/display_examples/mipi_dsi_compliant_test/example_board_readme.md)
