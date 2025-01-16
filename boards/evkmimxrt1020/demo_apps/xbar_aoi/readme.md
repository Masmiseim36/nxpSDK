# xbar_aoi

## Overview
The XBAR AOI demo application demonstrates the usage of the XBAR, AOI peripheral to combine interrupt
from CMP and PIT. If the button is pressed(CMP interrupt) and the PIT periodic interrupt occurs a message is printed.

When there is no CMP/ACMP ip in SoC, AOI combines a high level signal(force the input to be 1) and PIT trigger signal, then
the message "PIT periodic interrupt occurs" will be printed when the PIT timer period has timed out.

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/demo_apps/xbar_aoi/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/demo_apps/xbar_aoi/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/demo_apps/xbar_aoi/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/demo_apps/xbar_aoi/example_board_readme.md)
- [EVK-MIMXRT1020](../../../_boards/evkmimxrt1020/demo_apps/xbar_aoi/example_board_readme.md)
- [MIMXRT1024-EVK](../../../_boards/evkmimxrt1024/demo_apps/xbar_aoi/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/demo_apps/xbar_aoi/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/demo_apps/xbar_aoi/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/demo_apps/xbar_aoi/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/demo_apps/xbar_aoi/example_board_readme.md)
