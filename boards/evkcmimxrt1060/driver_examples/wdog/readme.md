# wdog01

## Overview
The WDOG Example project is to demonstrate usage of the KSDK wdog driver.
In this example,implemented to test the wdog.
Please notice that because WDOG control registers are write-once only. And
for the field WDT, once software performs a write "1" operation to this bit,
it can not be reset/cleared until the next POR, this bit does not get reset/
cleared due to any system reset. So the WDOG_Init function can be called 
only once after power reset when WDT set, and the WDOG_Disable function can 
be called only once after reset.

## Supported Boards
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/driver_examples/wdog/example_board_readme.md)
- [MIMXRT1060-EVKB](../../_boards/evkbmimxrt1060/driver_examples/wdog/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/driver_examples/wdog/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/driver_examples/wdog/example_board_readme.md)
- [EVK-MIMX8MM](../../_boards/evkmimx8mm/driver_examples/wdog/example_board_readme.md)
- [EVK-MIMX8MN](../../_boards/evkmimx8mn/driver_examples/wdog/example_board_readme.md)
- [EVK-MIMX8MNDDR3L](../../_boards/evkmimx8mnddr3l/driver_examples/wdog/example_board_readme.md)
- [EVK-MIMX8MP](../../_boards/evkmimx8mp/driver_examples/wdog/example_board_readme.md)
- [EVK-MIMX8MQ](../../_boards/evkmimx8mq/driver_examples/wdog/example_board_readme.md)
- [EVK-MIMXRT1010](../../_boards/evkmimxrt1010/driver_examples/wdog/example_board_readme.md)
- [EVK-MIMXRT1015](../../_boards/evkmimxrt1015/driver_examples/wdog/example_board_readme.md)
- [EVK-MIMXRT1020](../../_boards/evkmimxrt1020/driver_examples/wdog/example_board_readme.md)
- [MIMXRT1024-EVK](../../_boards/evkmimxrt1024/driver_examples/wdog/example_board_readme.md)
- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/driver_examples/wdog/example_board_readme.md)
- [EVK-MIMXRT1064](../../_boards/evkmimxrt1064/driver_examples/wdog/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/driver_examples/wdog/example_board_readme.md)
