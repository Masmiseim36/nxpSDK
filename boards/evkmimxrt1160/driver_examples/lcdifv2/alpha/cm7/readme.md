# lcdifv2_alpha

## Overview
This example demonstrates how to use the LCDIF v2 layer blend feature. In this
example, two layers are enabled. For each layer, a global alpha value 50% (128)
is used. When the example runs, two rectangles are shown, one blue and one red,
in the overlay region, the color is purple.

+-------------------------------------------+
|                    |                      |
|      RED           |                      |
|          +---------+---------+            |
|          | PURPLE  |         |            |
|          |         |         |            |
|----------+---------+         |            |
|          |                   |            |
|          |          BLUE     |            |
|          +-------------------+            |
|                                           |
|                                           |
+-------------------------------------------+

Note: The global alpha value shall only apply to the area that 2 layers overlaps, however
due to IP limitation, the blend happens on non-ovelapped area too. So if the global alpha
value of the upper layer is set to 0xFF, the whole red area disappears. The IP cannot support
such use case.

## Supported Boards
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/lcdifv2/lcdifv2_examples_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/lcdifv2/lcdifv2_examples_readme.md)
