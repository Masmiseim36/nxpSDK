# erpc_two_way_rpc_rpmsg_rtos

## Overview

The Multicore eRPC Two Way RPC RTOS project is a simple demonstration
program that uses the MCUXpresso SDK software and the Multicore SDK to show
how to implement the Remote Procedure Call between cores of the multicore system.

This multicore example shows how both the eRPC client and the eRPC server can be
setup on one side/core (bidirectional communication) and how to handle callback
functions in eRPC.

The primary core (Core0) creates client and server tasks first.
The client task releases the secondary core from the reset, initializes the RPMsg-Lite erpc transport
and once the server task is running it configures the arbitrated client.
Then the application logic is running.

The secondary core (Core1) creates client and server tasks two.
The client task initializes the RPMsg-Lite erpc transport and once the server task is running it
configures the arbitrated client.
Then the application logic is running.
The client task logic of the Core1 is very simple, it repeatedly calls the
increaseNumber() erpc function that is implemented on the Core0 and that increments
the counter.

The client task logic of the Core0 alternately issues either getNumberFromCore0()
function implementation on the Core0 or getNumberFromCore1() function implementation
on the Core1 (erpc call).
Then, the nestedCallGetNumber() erpc function call is issued that
alternately triggers either getNumberFromCore1() function implementation
on the Core1 (normal erpc call) or it triggers the getNumberFromCore0() function
implementation on the Core0 (nested erpc call, routed through the Core1 erpc server).

## Shared memory usage

This multicore example uses the shared memory for data exchange.
The shared memory region is defined and the size can be adjustable in the linker file.
The shared memory region start address and the size have to be defined in linker file for each core equally.
The shared memory start address is then exported from the linker to the application.

## eRPC documentation

eRPC specific files are stored in: middleware\multicore\erpc
eRPC documentation is stored in: middleware\multicore\erpc\doc
eRPC is open-source project stored on github: https://github.com/EmbeddedRPC/erpc
eRPC documentation can be also found in: http://embeddedrpc.github.io

## Building the application

This shows example how to build application for `evkbmimxrt1170` board with `cm7` core_id.
Change the `-b <board>` parameter based on board you want to build.
Change the `-Dcore_id=<core_id>` parameter based on board core you want to build.
For these parameters please see attribute `boards:` in `primary/example.yml`.

```
west build --sysbuild examples/multicore_examples/erpc_two_way_rpc_rpmsg_rtos/primary --toolchain armgcc --config debug -b evkbmimxrt1170 -Dcore_id=cm7
```

## Supported Boards

- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/multicore_examples/erpc_two_way_rpc_rpmsg_rtos/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/multicore_examples/erpc_two_way_rpc_rpmsg_rtos/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/multicore_examples/erpc_two_way_rpc_rpmsg_rtos/example_board_readme.md)
- [FRDM-K32L3A6](../../_boards/frdmk32l3a6/multicore_examples/erpc_two_way_rpc_rpmsg_rtos/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/multicore_examples/erpc_two_way_rpc_rpmsg_rtos/example_board_readme.md)
- [FRDM-MCXW72](../../_boards/frdmmcxw72/multicore_examples/erpc_two_way_rpc_rpmsg_rtos/example_board_readme.md)
- [KW47-EVK](../../_boards/kw47evk/multicore_examples/erpc_two_way_rpc_rpmsg_rtos/example_board_readme.md)
- [KW47-LOC](../../_boards/kw47loc/multicore_examples/erpc_two_way_rpc_rpmsg_rtos/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/multicore_examples/erpc_two_way_rpc_rpmsg_rtos/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/multicore_examples/erpc_two_way_rpc_rpmsg_rtos/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/multicore_examples/erpc_two_way_rpc_rpmsg_rtos/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/multicore_examples/erpc_two_way_rpc_rpmsg_rtos/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/multicore_examples/erpc_two_way_rpc_rpmsg_rtos/example_board_readme.md)
