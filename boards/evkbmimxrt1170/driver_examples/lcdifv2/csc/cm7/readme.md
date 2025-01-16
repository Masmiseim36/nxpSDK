# lcdifv2_csc

## Overview
This example demonstrates how to use the LCDIF v2 driver to do the YUV to RGB
conversion. In this example, the frame buffer pixel format is YUV, the byte
order in frame buffer is:

Y U Y V Y U Y V ...

The LCDIV v2 converts the color internally and sends out RGB color to the panel.

When the example runs, the panel shows four color blocks:

+-------------------------------+
|            RED                |
+-------------------------------+
|            GREEN              |
+-------------------------------+
|            BLUE               |
+-------------------------------+
|            WHITE              |
+-------------------------------+

## Supported Boards
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/lcdifv2/lcdifv2_examples_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/lcdifv2/lcdifv2_examples_readme.md)
