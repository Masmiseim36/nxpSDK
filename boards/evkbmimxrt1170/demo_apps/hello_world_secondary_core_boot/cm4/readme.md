# hello_world_secondary_core_boot

## Overview
The hello_world_secondary_core_boot demo use cm7 core to kick off cm4 core, and cm4 core will prints the "Hello World" string 
to the terminal using the SDK UART drivers and repeat what user input. This demo demonstrates the dual XIP architecture boot 
CM7 and CM4 core, XIP configuration will be included into CM7 image, so that the image of CM4 can also be booted from FLASH, 
which helps developers to have more choices on secondary core target memory.

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/demo_apps/hello_world_secondary_core_boot/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/demo_apps/hello_world_secondary_core_boot/example_board_readme.md)
