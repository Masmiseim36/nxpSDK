# enet_1g_txrx_multiring_transfer

## Overview

The enet_txrx_multiring_transfer example shows the way to use ENET driver to
 receive and transmit avb frame in the avb feature supported multi-ring platforms.
 this example is only supported in multi-ring platform.

1. This example shows how to initialize the ENET MAC.
2. How to use ENET MAC to transmit frames in avb supported
multiple-ring platforms.

The example transmits 30 number frames. For simple demo, we create frames with some special definition.
1. Build transmission frames with broadcast mac address.
2. Build different frames for each ring: 10 normal ethernet frame, 10 similar IEC 61883-6 type frames,
10 similar IEC 61883-8 type frames and the two IEC 1722 avb frames are set with different vlan priority .

Notice, To simply show how to use three different rings in rx/tx on one board. This example set PHY local loop and you will see there will be PHY init auto-negotiation fail and this is normal. because phy can not finish
auto-negotiation when phy is in loop back mode. However, the auto-negotiation fail has no impact on this loop back example, so just forget about the failure auto-negotiation.

## Supported Boards
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/enet_1g/txrx_multiring_transfer/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/enet_1g/txrx_multiring_transfer/example_board_readme.md)
