# rpmsg_lite_pingpong_rtos

## Overview

The Multicore RPMsg-Lite pingpong FreeRTOS project is a simple demonstration program that uses the
MCUXpresso SDK software and the RPMsg-Lite library and shows how to implement the inter-core
communication between cores of the multicore system.

The primary core releases the secondary core from the reset and then the inter-core communication is established.

Once the RPMsg is initialized and endpoints are created the message exchange starts, incrementing a
virtual counter that is part of the message payload.

The message pingpong finishes when the counter reaches the value of 100.
Then the RPMsg-Lite is deinitialized and the procedure of the data exchange is repeated again.

## Shared memory usage

This multicore example uses the shared memory for data exchange. The shared memory region is
defined and the size can be adjustable in the linker file.
The shared memory region start address and the size have to be defined in linker file for each core equally.
The shared memory start address is then exported from the linker to the application.

## Building the application

This shows example how to build application for `evkbmimxrt1170` board with `cm7` core_id.
Change the `-b <board>` parameter based on board you want to build.
Change the `-Dcore_id=<core_id>` parameter based on board core you want to build.
For these parameters please see attribute `boards:` in `primary/example.yml`.

```
west build --sysbuild examples/multicore_examples/rpmsg_lite_pingpong_rtos/primary --toolchain armgcc --config debug -b evkbmimxrt1170 -Dcore_id=cm7
```

## Supported Boards

- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/multicore_examples/rpmsg_lite_pingpong_rtos/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/multicore_examples/rpmsg_lite_pingpong_rtos/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/multicore_examples/rpmsg_lite_pingpong_rtos/example_board_readme.md)
- [FRDM-K32L3A6](../../_boards/frdmk32l3a6/multicore_examples/rpmsg_lite_pingpong_rtos/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/multicore_examples/rpmsg_lite_pingpong_rtos/example_board_readme.md)
- [FRDM-MCXW72](../../_boards/frdmmcxw72/multicore_examples/rpmsg_lite_pingpong_rtos/example_board_readme.md)
- [KW47-EVK](../../_boards/kw47evk/multicore_examples/rpmsg_lite_pingpong_rtos/example_board_readme.md)
- [KW47-LOC](../../_boards/kw47loc/multicore_examples/rpmsg_lite_pingpong_rtos/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/multicore_examples/rpmsg_lite_pingpong_rtos/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/multicore_examples/rpmsg_lite_pingpong_rtos/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/multicore_examples/rpmsg_lite_pingpong_rtos/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/multicore_examples/rpmsg_lite_pingpong_rtos/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/multicore_examples/rpmsg_lite_pingpong_rtos/example_board_readme.md)
