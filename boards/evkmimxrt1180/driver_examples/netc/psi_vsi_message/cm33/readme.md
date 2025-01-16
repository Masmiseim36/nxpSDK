# netc_psi_vsi_message

## Overview
The Multicore NETC PSI-VSI demo application demonstrates how to set up projects for individual
cores on a multicore system. In this demo, the primary core use NETC PSI to send/receive frames.
It also releases the secondary core from the reset. The secondary core toggles an on-board LED
indicating that the secondary core is running and uses NETC VSI to send/receive frames.

## Supported Boards
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/netc/psi_vsi_message/example_board_readme.md)
