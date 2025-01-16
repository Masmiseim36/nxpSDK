# csi_mipi_yuv

## Overview
This project shows how to use the CSI with MIPI_CSI. In this example, the
camera device output pixel format is YUYV, the MIPI_CSI converts it to
YUV internally and sends to CSI. In other words, the CSI input data bus
width is 24-bit. The CSI saves the frame as 32-bit format XYUV8888. PXP
is used to convert the XYUV to RGB565 and show in the LCD panel.

## Supported Boards
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/csi/mipi_yuv/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/csi/mipi_yuv/example_board_readme.md)
