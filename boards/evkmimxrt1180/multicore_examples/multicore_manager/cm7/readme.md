# multicore_manager

## Overview

The Multicore Manager example application demonstrates advanced features of the MCMgr component.

In this demo, the primary core prints the "Hello World from the Primary Core!" string to the terminal and
then releases the secondary core from the reset.
The secondary core toggles an on-board LED indicating that the secondary core is running.
It is also possible to stop/start the secondary core during the runtime by pressing on-board buttons.
The on-board LED then indicates the change, it stops/starts toggling accordingly.

This application also shows how to use the Multicore Manager for remote core monitoring and
handling of events such as application, CoreUp, CoreDown and exception events.

Application-specific callback functions for events are registered by the MCMGR_RegisterEvent() API
on the primary core.
Triggering these events is done using the MCMGR_TriggerEvent() API on the
secondary core.
While CoreUp, CoreDown events are triggered from startup and MCMGR code, the
application event is triggered from the application.

The exception event is triggered in the secondary application after 100 LED toggles by trying
to write to flash. This raises the exception on the secondary core and triggers the
RemoteExceptionEvent on the primary core.

## Building the application

This shows example how to build application for `evkmimxrt1180` board with `cm33` core_id.
Change the `-b <board>` parameter based on board you want to build.
Change the `-Dcore_id=<core_id>` parameter based on board core you want to build.
For these parameters please see attribute `boards:` in `primary/example.yml`.

```
west build --sysbuild examples/multicore_manager/hello_world/primary --toolchain armgcc --config debug -b evkmimxrt1180 -Dcore_id=cm33
```

## Supported Boards

- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/multicore_examples/multicore_manager/example_board_readme.md)
- [FRDM-K32L3A6](../../_boards/frdmk32l3a6/multicore_examples/multicore_manager/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/multicore_examples/multicore_manager/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/multicore_examples/multicore_manager/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/multicore_examples/multicore_manager/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/multicore_examples/multicore_manager/example_board_readme.md)
