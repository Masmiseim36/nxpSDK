# cache

## Overview

The cache example shows how to use memory cache driver.

In this example, many memory (such as SDRAM, etc) and DMA will be used to show the example.
Those memory is both accessible for cpu and DMA. For the memory data alignment, cache driver should be 
used rightly. For cache enabled case, the cache invalidate operation should be done before cpu access the memory. The cache clean opeartion should be done after CPU verify the memory
memory and before DMA access the memory.

## Supported Boards
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/driver_examples/cache/example_board_readme.md)
- [MIMXRT1060-EVKB](../../_boards/evkbmimxrt1060/driver_examples/cache/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/driver_examples/cache/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/driver_examples/cache/example_board_readme.md)
- [EVK-MIMXRT1010](../../_boards/evkmimxrt1010/driver_examples/cache/example_board_readme.md)
- [EVK-MIMXRT1015](../../_boards/evkmimxrt1015/driver_examples/cache/example_board_readme.md)
- [EVK-MIMXRT1020](../../_boards/evkmimxrt1020/driver_examples/cache/example_board_readme.md)
- [MIMXRT1024-EVK](../../_boards/evkmimxrt1024/driver_examples/cache/example_board_readme.md)
- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/driver_examples/cache/example_board_readme.md)
- [EVK-MIMXRT1064](../../_boards/evkmimxrt1064/driver_examples/cache/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/driver_examples/cache/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/driver_examples/cache/example_board_readme.md)
