# freertos_message_buffers

## Overview

The FreeRTOS Message Buffers multicore project is a simple demonstration program that uses
the MCUXpresso SDK software and the Message Buffers component of FreeRTOS.

It shows how to implement lightweight core to core communication using FreeRTOS Message Buffers, which
are lockless circular buffers that can pass data packets of varying sizes from a single sender to a single receiver.

The primary core releases the secondary core from the reset and then the inter-core communication is established then.
Once the Message Buffers are initialized on both cores the message exchange starts, incrementing
a virtual counter that is part of the message payload.
The message pingpong finishes when the counter reaches the value of 100.
The Message Buffer is deinitialized at the end.

NXP Multicore Manager (MCMGR) component is used for several purposes in this example:
- it releases the secondary core from the reset (starts the secondary code)
- it registers and use the application event for init handshaking (the secondary core application
  signals to the primary core it is ready to communicate)
- it registers and use the FreeRtosMessageBuffersEvent for inter-core interrupts triggering and
  interrupts handling

## Shared memory usage

This multicore example uses the shared memory for data exchange.
The shared memory region is defined and the size can be adjustable in the linker file.
The shared memory region start address and the size have to be defined in linker file for each core equally.
The shared memory start address is then exported from the linker to the application to allow placing
FreeRTOS Message Buffers at a fixed location that is then known for both cores.

## Supported Boards

- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/multicore_examples/freertos_message_buffers/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/multicore_examples/freertos_message_buffers/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/multicore_examples/freertos_message_buffers/example_board_readme.md)
- [FRDM-K32L3A6](../../_boards/frdmk32l3a6/multicore_examples/freertos_message_buffers/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/multicore_examples/freertos_message_buffers/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/multicore_examples/freertos_message_buffers/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/multicore_examples/freertos_message_buffers/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/multicore_examples/freertos_message_buffers/example_board_readme.md)
