# lwip_dhcp_usb_freertos

## Overview

The lwip_dhcp demo application demonstrates a DHCP and ping demo on the lwIP TCP/IP and USB stack.
The application acts as a DHCP client and prints the status as it is progressing.
Once the interface is being bound to an IP address obtained from DHCP server, address information is printed.
The application will get ip adress www.nxp.com and ping the ip address.

## Prepare the Demo
1.  Connect a USB cable between the PC host and the OpenSDA(or USB to Serial) USB port on the target board.
2.  Open a serial terminal on PC for OpenSDA serial(or USB to Serial) device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

## Running the demo
refer to MCUXpresso SDK USB RNDIS & LWIP User Guide

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/lwip_examples/lwip_dhcp_usb/freertos/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/lwip_examples/lwip_dhcp_usb/freertos/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/lwip_examples/lwip_dhcp_usb/freertos/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/lwip_examples/lwip_dhcp_usb/freertos/example_board_readme.md)
- [EVK-MIMXRT1020](../../../_boards/evkmimxrt1020/lwip_examples/lwip_dhcp_usb/freertos/example_board_readme.md)
- [MIMXRT1024-EVK](../../../_boards/evkmimxrt1024/lwip_examples/lwip_dhcp_usb/freertos/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/lwip_examples/lwip_dhcp_usb/freertos/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/lwip_examples/lwip_dhcp_usb/freertos/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/lwip_examples/lwip_dhcp_usb/freertos/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/lwip_examples/lwip_dhcp_usb/freertos/example_board_readme.md)
- [FRDM-RW612](../../../_boards/frdmrw612/lwip_examples/lwip_dhcp_usb/freertos/example_board_readme.md)
- [LPCXpresso54628](../../../_boards/lpcxpresso54628/lwip_examples/lwip_dhcp_usb/freertos/example_board_readme.md)
- [LPCXpresso54S018](../../../_boards/lpcxpresso54s018/lwip_examples/lwip_dhcp_usb/freertos/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/lwip_examples/lwip_dhcp_usb/freertos/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/lwip_examples/lwip_dhcp_usb/freertos/example_board_readme.md)
- [RD-RW612-BGA](../../../_boards/rdrw612bga/lwip_examples/lwip_dhcp_usb/freertos/example_board_readme.md)
