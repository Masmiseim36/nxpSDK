# pxp_lcdif_handshake

## Overview
The PXP LCDIF hand shake project shows how to enable the hand shake between
PXP and LCDIF. If this example runs correctly, you will see two rectangles moving in the
screen. One rectangle is process surface output, and the other is alpha surface
output, the overlay region color is OR'ed value of process surface and alpha surface.

NOTE:
This example must run continously to get the right result, breaking during debug
will results to handshake fail. Because the PXP must be started before every LCD
frame, if system is halt, the PXP does not start correctly.

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/driver_examples/pxp/lcdif_handshake/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/pxp/lcdif_handshake/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/pxp/lcdif_handshake/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/driver_examples/pxp/lcdif_handshake/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/driver_examples/pxp/lcdif_handshake/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/driver_examples/pxp/lcdif_handshake/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/pxp/lcdif_handshake/example_board_readme.md)
