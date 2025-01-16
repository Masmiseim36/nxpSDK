# enet_txrx_ptp1588_transfer

## Overview

The enet_rxtx_ptp1588 example shows the way to use ENET driver to  
 receive and transmit frame in the 1588 feature required cases.

1. This example shows how to initialize the ENET MAC.
2. How to use ENET MAC to receive and transmit frame.
3. How to add to the multicast group to receive PTP 1588 message.
4. How to get the time stamp of the PTP 1588 timer.
4. How to use Get the ENET transmit and receive frame time stamp.

The example transmits 20 number PTP event frame, shows the timestamp of the transmitted frame.
The length, source MAC address and destination MAC address of the received frame will be print. 
The time stamp of the received timestamp will be print when the PTP message frame is received. 

Note, The RMII mode is used for default setting to initialize the ENET interface between MAC and the external PHY. you 
can change it to MII mode as you wish. Please make sure the MII Mode setting in the MAC is synchronize to the setting
in TWR-SERIAL board for the external PHY.

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/driver_examples/enet/txrx_ptp1588_transfer/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/enet/txrx_ptp1588_transfer/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/enet/txrx_ptp1588_transfer/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/driver_examples/enet/txrx_ptp1588_transfer/example_board_readme.md)
- [EVK-MIMX8MM](../../../_boards/evkmimx8mm/driver_examples/enet/txrx_ptp1588_transfer/example_board_readme.md)
- [EVK-MIMX8MP](../../../_boards/evkmimx8mp/driver_examples/enet/txrx_ptp1588_transfer/example_board_readme.md)
- [EVK-MIMXRT1020](../../../_boards/evkmimxrt1020/driver_examples/enet/txrx_ptp1588_transfer/example_board_readme.md)
- [MIMXRT1024-EVK](../../../_boards/evkmimxrt1024/driver_examples/enet/txrx_ptp1588_transfer/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/driver_examples/enet/txrx_ptp1588_transfer/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/driver_examples/enet/txrx_ptp1588_transfer/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/enet/txrx_ptp1588_transfer/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../../_boards/mcimx93autoevk/driver_examples/enet/txrx_ptp1588_transfer/example_board_readme.md)
- [MCIMX93-EVK](../../../_boards/mcimx93evk/driver_examples/enet/txrx_ptp1588_transfer/example_board_readme.md)
