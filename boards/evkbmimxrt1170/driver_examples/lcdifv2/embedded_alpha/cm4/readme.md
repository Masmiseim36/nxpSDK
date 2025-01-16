# lcdifv2_embedded_alpha

## Overview
This example demonstrates how to use the LCDIF v2 multiple layer blend using
the alpha embedded in pixel.
In this example, three layers are enabled. Layer 0 uses blue color RGB565 with
global alpha 50%. Layer 1 uses red color ARGB4444 with embedded_alpha %50.
Layer 2 uses green color ARGB8888 with embedded_alpha %25. The three layers are
all smaller than the screen size. By changing the layer offset, the color block
moves in the screen. The overlay region shows the blend color.

The initial screen is like this.

+------------------------------------+
|               |                    |
|  BLUE         |                    |
|       +-------+-------+            |
|       |PURPLE | RED   |            |
|       |       |       |            |
|-------+-------+-------+-------+    |
|       | RED   |YELLOW |       |    |
|       |       |       |       |    |
|       +-------+-------+       |    |
|               |               |    |
|               |     GREEN     |    |
|               +---------------+    |
|                                    |
|                                    |
|                                    |
+------------------------------------+

## Supported Boards
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/lcdifv2/lcdifv2_examples_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/lcdifv2/lcdifv2_examples_readme.md)
