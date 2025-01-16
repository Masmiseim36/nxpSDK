# enet_qos_txrx_ptp1588_transfer

## Overview

The example shows the way to use ENET driver to
 receive and transmit frame in the 1588 feature required cases.

1. This example shows how to initialize the ENET.
2. How to get the time stamp of the PTP 1588 timer.
3. How to use Get the ENET transmit and receive frame time stamp.

The example transmits 20 number PTP event frame, shows the timestamp of the transmitted frame.
The length, source MAC address and destination MAC address of the received frame will be print.
The time stamp of the received timestamp will be print when the PTP message frame is received(the outside loopback cable can be used to see the right rx time-stamping log since we send the ptp message).

## Supported Boards
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/enet_qos/txrx_ptp1588_transfer/example_board_readme.md)
- [EVK-MIMX8MP](../../../_boards/evkmimx8mp/driver_examples/enet_qos/txrx_ptp1588_transfer/example_board_readme.md)
- [MCIMX93-EVK](../../../_boards/mcimx93evk/driver_examples/enet_qos/txrx_ptp1588_transfer/example_board_readme.md)
- [MCIMX93-QSB](../../../_boards/mcimx93qsb/driver_examples/enet_qos/txrx_ptp1588_transfer/example_board_readme.md)
