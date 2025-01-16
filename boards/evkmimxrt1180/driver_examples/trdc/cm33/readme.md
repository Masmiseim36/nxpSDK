# trdc

## Overview
The Multicore TRDC demo application demonstrates how to use TRDC on a multicore system
to control the access policy of the memory block checker(MBC) and memory region
checker(MRC), and how to configure the domain access control(DAC) for bus masters
to access both secure and non-secure domains.

First, the primary core example uses 1 domain for secure access demo. The primary
core which is secure first sets a MRC memory region and a MBC memory block to non-secure
access only, then trys to access the memory which triggers the hardfault. In hardfault
the access control is changed to secure access to resolve the error.
Then, the primary core releases the secondary core from the reset. The secondary core
example uses 2 domains, one is for secure access and the other is for non-secure access.
By enabling different DAC configuration, different domain is used for the access control.
The secure access process is the same as the primary core.
In the non-secure access demo, the secondary core sets a MRC memory region and a MBC memory
block to secure access only, and switch to the non-secure DAC configuration which force
the core to be non-secure. Then trys to access the memory which triggers the hardfault.
In hardfault certain flags stored in flash is set to let primary core know and update
the configuration to non-secure access to resolve the error, because the TRDC is secure
access only.

## Supported Boards
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/trdc/example_board_readme.md)
