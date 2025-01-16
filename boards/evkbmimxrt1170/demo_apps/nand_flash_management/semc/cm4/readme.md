# nand_flash_management

## Overview

Nand flash management demo demonstrates the use of a open source nand flash middleware and nand flash componet in KSDK.
The demo combine a open source nand flash middleware which include:
---wear leveling,
---bad block management,
---power fail safe,
---garbage collection - user should be carefully with this function, there is a GC_RATIO in the middleware, the bigger value of the ratio, ---the more nand flash memory will be available to user, but it will cause the write amplification.
with fatfs and freertos.
The demo use shell to support multiple file operation command, such as
write -  write a new file
read -  read a file
list -  list the file in the directory
free - check the free cluster of current file system
..., user can get detail information by command "help".
There are two tasks in the demo, shell task is used to receive and process user command, another task is garbage collection task. The gc task priority is bigger than shell task to reduce the write amplification.

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/demo_apps/nand_flash_management/semc/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/demo_apps/nand_flash_management/semc/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/demo_apps/nand_flash_management/semc/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/demo_apps/nand_flash_management/semc/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/demo_apps/nand_flash_management/semc/example_board_readme.md)
