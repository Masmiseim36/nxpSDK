# lwip_dhcp_bm

## Overview

The lwip_dhcp demo application demonstrates a DHCP demo on the lwIP TCP/IP stack.
The application acts as a DHCP client and prints the status as it is progressing.
Once the interface is being bound to an IP address obtained from DHCP server, address information is printed.
It does so continuously - it prints the client's state even as the link state changes.
Those link state changes are also printed.

## Prepare the Demo
1.  Connect a USB cable between the PC host and the OpenSDA(or USB to Serial) USB port on the target board.
2.  Open a serial terminal on PC for OpenSDA serial(or USB to Serial) device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert the Ethernet Cable into the target board's RJ45 port and connect it to a router (or other DHCP server capable device).
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

## Running the demo
When the demo runs, the log would be seen on the terminal like:

Initializing PHY...
[LINK STATE] netif=0, state=up, speed=1000M_full

************************************************
 DHCP example
************************************************
 DHCP state       : SELECTING
 DHCP state       : REQUESTING
 DHCP state       : CHECKING
 DHCP state       : BOUND

 IPv4 Address     : 192.168.0.4
 IPv4 Subnet mask : 255.255.255.0
 IPv4 Gateway     : 192.168.0.1

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [EVK-MIMXRT1020](../../../_boards/evkmimxrt1020/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [MIMXRT1024-EVK](../../../_boards/evkmimxrt1024/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [FRDM-RW612](../../../_boards/frdmrw612/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [IMX95LP4XEVK-15](../../../_boards/imx95lp4xevk15/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [IMX95LPD5EVK-19](../../../_boards/imx95lpd5evk19/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [imx95verdinevk](../../../_boards/imx95verdinevk/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [LPCXpresso54628](../../../_boards/lpcxpresso54628/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [LPCXpresso54S018](../../../_boards/lpcxpresso54s018/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [LPCXpresso54S018M](../../../_boards/lpcxpresso54s018m/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../../_boards/mcimx93autoevk/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [MCIMX93-EVK](../../../_boards/mcimx93evk/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
- [RD-RW612-BGA](../../../_boards/rdrw612bga/lwip_examples/lwip_dhcp/bm/example_board_readme.md)
