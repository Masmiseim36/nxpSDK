# pxp_color_key

## Overview
The PXP color key project shows how to use the AS color key together with the
alpha blend. In this example, the AS pixel format is RGB565, the global alpha
is used for alpha blend.

The PS frame buffer is like this:

+---------------------------------+-----------------------------------+
|                                 |                                   |
|                                 |                                   |
|                                 |                                   |
|                                 |                                   |
|                                 |                                   |
|                                 |                                   |
|                                 |                                   |
|           BLUE                  |             RED                   |
|                                 |                                   |
|                                 |                                   |
|                                 |                                   |
|                                 |                                   |
|                                 |                                   |
|                                 |                                   |
|                                 |                                   |
|                                 |                                   |
|                                 |                                   |
+---------------------------------+-----------------------------------+

The AS frame buffer is square like this:

               +---------------------------------+
               |                                 |
               |                                 |
               |             GREEN               |
               |                                 |
               |        +---------------+        |
               |        |               |        |
               |        |               |        |
               |        |               |        |
               |        |    YELLOW     |        |
               |        |               |        |
               |        |               |        |
               |        |               |        |
               |        +---------------+        |
               |                                 |
               |                                 |
               |                                 |
               +---------------------------------+

The AS is placed in the center of the screen. The AS color key is set to yellow
color, the blend alpha value is set to 128. So in the output frame buffer, the
originally yellow region shows PS color, the green region is blend with PS color.
The screen shows the output frame like this:

+-------------+-------------------+-------------------+---------------+
|             |                   |                   |               |
|             |   HALF CYAN       |    HALF YELLOW    |               |
|             |                   |                   |               |
|             |                   |                   |               |
|             |          +--------+---------+         |               |
|             |          |        |         |         |               |
|             |          |        |         |         |               |
|   BLUE      |          | BLUE   |  RED    |         |    RED        |
|             |          |        |         |         |               |
|             |          |        |         |         |               |
|             |          |        |         |         |               |
|             |          +--------+---------+         |               |
|             |                   |                   |               |
|             |                   |                   |               |
|             |                   |                   |               |
|             |                   |                   |               |
|             |                   |                   |               |
+-------------+-------------------+-------------------+---------------+

## Supported Boards
- [EVK9-MIMX8ULP](../../../_boards/evk9mimx8ulp/driver_examples/pxp/color_key/example_board_readme.md)
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/driver_examples/pxp/color_key/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/pxp/color_key/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/pxp/color_key/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/driver_examples/pxp/color_key/example_board_readme.md)
- [EVK-MIMX8ULP](../../../_boards/evkmimx8ulp/driver_examples/pxp/color_key/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/driver_examples/pxp/color_key/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/driver_examples/pxp/color_key/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/pxp/color_key/example_board_readme.md)
