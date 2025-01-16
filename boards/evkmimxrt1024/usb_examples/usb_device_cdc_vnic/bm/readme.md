# dev_cdc_vnic




## Overview

The Virtual NIC project is a simple demonstration program based on the MCUXpresso SDK.  It is enumerated as a network adapter. Users can access the network by properly configuring this network adapter. The purpose of this demo is to show how to build a device of USB CDC class to implement RNDIS protocol and to provide a simple project for further development.


## System Requirement

### Hardware requirements

- Mini/micro USB cable
- USB A to micro AB cable
- Hardware (Tower module/base board, and so on) for a specific device
- Personal Computer


### Software requirements

- The project files are in:
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_device_cdc_vnic/<rtos>/<toolchain>.
<br> For lite version, the project files are in:
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_device_cdc_vnic_lite/<rtos>/<toolchain>.
> The <rtos> is Bare Metal or FreeRTOS OS.


## Getting Started

### Hardware Settings

> Set the hardware jumpers (Tower system/base module) to default settings.


### Prepare the example 

1.  Download the program to the target board.
2.  Either press the reset button on your board or launch the debugger in your IDE to begin running
    the demo.
3.  Plug in the network cable before running this example. (Or you may have to disable and then enable the RNDIS network adapter after you plug in the network cable.)
4.  Connect the USB device port on the board to the host PC and the using a USB cable.

> For detailed instructions, see the appropriate board User's Guide.
<br>    Duo to the speed missmatch between usb and enet, some enet frame will be discard when lots of data are received in short time, this issue is obvious, especially on Full speed usb. 
<br>    The upper layer protocol will handle this issue, such as TCP/IP protocol. 
<br>    Based on the test on TWR-K65,add the buffer number to receive enet frame data could reduce the discard frame, user may add the buffer number if ram is enough.

## Run the example in Windows&reg;

1.  A network adapter is enumerated in Device Manager.
<br>![Virtual NIC in device manager](vnic_device_manager.jpg "Virtual NIC in device manager")
> On Windows 10, the device would be identified as an virtual com for the first time.
<br>![Virtual com in device manager on Windows 10](vnic_device_manager_win10.jpg "Virtual com in device manager on Windows 10")
2.  In *Control Panel\\Network and Internet\\Network Connections*, the RNDIS network adapter is listed as below.
<br>![Virtual NIC in Network Connections](vnic_network_connection.jpg "Virtual NIC in Network Connections")
3.  Connect the ethernet port to another test PC using a network cable. The IP address of this test PC can be configured as 192.168.1.102. 
<br>![Test PC NIC configuration](test_pc_ip_config.jpg "Test PC NIC configuration")
4.  Then configure the RNDIS adapter in Windows for your host PC. For this testing example, please make sure your PC host and the test PC are in the same network segment. E.g
<br>![Host PC Virtual NIC configuration](vnic_config.jpg "Host PC Virtual NIC configuration")

5.  Then you are supposed to access the test PC through this USB RNDIS network adapter from your host PC
<br>![Ping to other PC](vnic_ping_windows.jpg "Ping to other PC")

## Note
1.  The VNIC CIC CLASS code can be changed to be miscellaneous device to implement that there is no need to install inf file by changing the codes for descriptor. Provide the following codes macros for descriptor
to change its related codes:
<br>    #define USB_CDC_IFACE_DESCRIPTOR_VNIC_CIC_CLASS (0xEF)
<br>    #define USB_CDC_IFACE_DESCRIPTOR_VNIC_CIC_SUBCLASS (0x04)
<br>    #define USB_CDC_IFACE_DESCRIPTOR_VNIC_CIC_PROTOCOL (0x01)
<br>	Note that the above change is only applied for descriptor, please do not change the codes of class driver. This is not verified under all conditions, so please be cautious to change.


## Installing the RNDIS driver for virtual_nic example
Below are the steps to install the RNDIS driver on Windows 7. Similar steps apply for Windows XP.

Step 1. Click "Update Driver Software..."

<br>![](rndis_driver_install_00.jpg "")

Step 2. Choose "Browse..."

<br>![](rndis_driver_install_01.jpg "")

Step 3. Select "Let me pick..."

<br>![](rndis_driver_install_02.jpg "")

Step 4. Navigate to your RNDIS driver location.

<install_dir>\\boards\\<board>\\usb_examples\\usb_device_cdc_vnic\\inf

or

<install_dir>\\boards\\<board>\\usb_examples\\usb_device_cdc_vnic_lite\\inf


<br>![](rndis_driver_install_03.jpg "")

Step 5. Press "Next".

<br>![](rndis_driver_install_04.jpg "")

Step 6. Ignore the warning and press "Yes".

<br>![](rndis_driver_install_05.jpg "")

Step 7. Now the RNDIS driver should be installed successfully.

<br>![](rndis_driver_install_06.jpg "")

- If a driver signature issue occurs on Windows 8 OS,  see the link,\n
[Disabling Driver Signature on Windows 8](https://learn.sparkfun.com/tutorials/disabling-driver-signature-on-windows-8/disabling-signed-driver-enforcement-on-windows-8)
- To enable driver signing on Windows OS, see the link,
  + <a href="msdn.microsoft.com/en-us/library/windows/hardware/ff544865(v=vs.85.aspx)">Driver Signing</a>
  + [Practical Windows Code and Driver Signing](http://www.davidegrayson.com/signing/#howto)

*/



## Supported Boards
- MIMXRT1170-EVKB
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/usb_examples/usb_device_cdc_vnic/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/usb_examples/usb_device_cdc_vnic/example_board_readme.md)
- EVK-MIMXRT1064
- MIMXRT1060-EVKC
- MIMXRT1040-EVK
- EVKB-IMXRT1050
- MIMXRT1024-EVK
- FRDM-MCXN947
- FRDM-RW612
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/usb_examples/usb_device_cdc_vnic/example_board_readme.md)
- MIMXRT1180-EVK
- RD-RW612-BGA
- EVK-MIMXRT1020
- LPCXpresso54S018
- LPCXpresso54S018M
- LPCXpresso54628
- MIMXRT1060-EVKB
