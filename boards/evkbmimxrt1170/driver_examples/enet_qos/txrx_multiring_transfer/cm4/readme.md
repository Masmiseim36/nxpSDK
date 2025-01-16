# enet_qos_txrx_multiring_transfer

## Overview

The example shows the way to use ENET QOS driver to  
 receive and transmit frames supported multi-ring platforms.
 this example is only supported in multi-ring platform.

1. This example shows how to initialize the ENET MAC.
2. How to use ENET MAC to transmit frames in avb supported 
multiple-ring platforms.

The example transmits 20 number frames. For simple demo, we create frames with some special definition.
1. Build transmission frames with broadcast mac address.
2. Build frames with unicast mac address
3. Send broadcast frames to ring 0 and unitcase frames to ring 1, then receive them back in associated rings.

## Supported Boards
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/enet_qos/txrx_multiring_transfer/example_board_readme.md)
- [EVK-MIMX8MP](../../../_boards/evkmimx8mp/driver_examples/enet_qos/txrx_multiring_transfer/example_board_readme.md)
- [MCIMX93-EVK](../../../_boards/mcimx93evk/driver_examples/enet_qos/txrx_multiring_transfer/example_board_readme.md)
- [MCIMX93-QSB](../../../_boards/mcimx93qsb/driver_examples/enet_qos/txrx_multiring_transfer/example_board_readme.md)
