# csi_fragment_gray

## Overview
The csi_fragment_gray project shows how to use CSI driver fragment mode functions.
In this example, the camera input format is YUV422, the CSI driver extracts the
Y element of a specific window (gray image). To show gray image, ELCDIF LUT is used.
If this example runs successfully, you will see a gray image window in the middle
of the LCD panel. The image is updated each time input any value from debug console.

Please note that only one frame buffer is used in this example, so there might be
screen tearing.

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/driver_examples/csi/fragment_gray/example_board_readme.md)
