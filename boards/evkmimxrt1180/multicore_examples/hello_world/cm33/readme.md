# hello_world

## Overview

The Multicore Hello World demo application demonstrates how to set up projects for individual
cores on a multicore system.
In this demo, the primary core prints the "Hello World from the Primary Core!" string to the terminal and
then releases the secondary core from the reset.
The secondary core toggles an on-board LED indicating that the secondary core is running.

## Building the application

This shows example how to build application for `evkbmimxrt1170` board with `cm7` core_id.
Change the `-b <board>` parameter based on board you want to build.
Change the `-Dcore_id=<core_id>` parameter based on board core you want to build.
For these parameters please see attribute `boards:` in `primary/example.yml`.

```
west build --sysbuild examples/multicore_examples/hello_world/primary --toolchain armgcc --config debug -b evkbmimxrt1170 -Dcore_id=cm7
```

## Supported Boards

- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/multicore_examples/hello_world/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/multicore_examples/hello_world/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/multicore_examples/hello_world/example_board_readme.md)
- [FRDM-K32L3A6](../../_boards/frdmk32l3a6/multicore_examples/hello_world/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/multicore_examples/hello_world/example_board_readme.md)
- [FRDM-MCXW72](../../_boards/frdmmcxw72/multicore_examples/hello_world/example_board_readme.md)
- [KW47-EVK](../../_boards/kw47evk/multicore_examples/hello_world/example_board_readme.md)
- [KW47-LOC](../../_boards/kw47loc/multicore_examples/hello_world/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/multicore_examples/hello_world/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/multicore_examples/hello_world/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/multicore_examples/hello_world/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/multicore_examples/hello_world/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/multicore_examples/hello_world/example_board_readme.md)
