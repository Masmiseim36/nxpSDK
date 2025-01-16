# lib_jpeg_hw_accel

## Overview
The lib jpeg with hardware acceleration demo decodes the JPEG picture using the JPEG decoder hardware,
and shows them in the LCD panel. Since not all JPEG formats are supported by the hardware, when macro
LIB_JPEG_USE_HW_ACCEL is on, the lib jpeg will parse the header first, if the format is supported, it will
use the hardware to accelerate, otherwise will use the old software way.

## Supported Boards
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/display_examples/lib_jpeg_hw_accel/example_board_readme.md)
