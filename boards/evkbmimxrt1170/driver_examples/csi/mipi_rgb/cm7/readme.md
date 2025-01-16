# csi_mipi_rgb

## Overview
This project shows how to use the CSI with MIPI_CSI. In this example, the
camera device output pixel format is RGB565, the MIPI_CSI converts it to
RGB888 internally and sends to CSI. In other words, the CSI input data bus
width is 24-bit. The CSI saves the frame as 32-bit format XRGB8888. PXP
is used to convert the XRGB8888 to RGB565 and shown in the LCD panel.

## Supported Boards
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/csi/mipi_rgb/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/csi/mipi_rgb/example_board_readme.md)
