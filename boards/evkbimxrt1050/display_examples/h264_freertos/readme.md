# h264_freertos

## Overview
This project read the h264 files from SD card, decode them and show the frames
in LCD panel.

## Prepare the H264 files
Name the H264 files like 000.264, 001.264, 002.264, ..., and save them
to the root path of the SD card. Please starts from 000.264, if there is only one
file, name it as 000.264. This project reads the files one by one and decode.
The recommend file is: https://github.com/cisco/openh264/tree/v2.1.1/res/test_cif_P_CABAC_slice.264

## Running the demo
This demo could run without debug console. The debug console shows the
frame size, decode frame rate, and some error information.

NOTE: For better performance, this project places the decode buffer to DTCM.
So the decode buffer size is limited by the DTCM size. See the file h264_dec.cpp
for more details. When the H264 file frame size is larger than decode buffer
size, then the decode fails and error log is output in debug console.
If frame size is more important than performance, please increase the decode
buffer size (macro DEMO_DECODE_BUF_SIZE in h264_dec.cpp) and place decode buffer
in other larger RAM region.

## Supported Boards
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/display_examples/h264_freertos/example_board_readme.md)
- [MIMXRT1060-EVKB](../../_boards/evkbmimxrt1060/display_examples/h264_freertos/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/display_examples/h264_freertos/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/display_examples/h264_freertos/example_board_readme.md)
- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/display_examples/h264_freertos/example_board_readme.md)
- [EVK-MIMXRT1064](../../_boards/evkmimxrt1064/display_examples/h264_freertos/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/display_examples/h264_freertos/example_board_readme.md)
