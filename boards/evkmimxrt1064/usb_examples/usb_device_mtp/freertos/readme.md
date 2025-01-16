# dev_mtp




## Overview

The USB MTP application is a simple demonstration program based on the MCUXpresso SDK.
It is enumerated as a portable device. Users can read and write the storage in the portable device.

## System Requirement

### Hardware requirements

- Mini/micro USB cable
- USB A to micro AB cable
- Hardware (Tower module/base board, and so on) for a specific device
- Personal Computer (PC)


### Software requirements

- The project files for lite version example are in: 
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_device_mtp_lite/<rtos>/<toolchain>.
<br>  For non-lite version example, the path is: 
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_device_mtp/<rtos>/<toolchain>.
> The <rtos> is Bare Metal or FreeRTOS OS.


## Getting Started

### Hardware Settings

> Set the hardware jumpers (Tower system/base module) to default settings.


### Prepare the example

1.  Download the program to the target board.
2.  Connect the target board to the external power source (the example is self-powered).
3.  Power off the target board. And then power on again.
4.  Connect a USB cable between the PC and the USB device port of the board.

> For detailed instructions, see the appropriate board User's Guide.

## Run the example

1.  Plug in the device, which is running the usb_device_mtp example, into the PC. A Portable Device is enumerated in the Device Manager.
2.  The computer will display the portable device.
<br>![Portable Device](portable_device.jpg "Portable Device")
3.  Double click the portable device icon, and the computer will display the storage capacity in the portable device.
<br>![Storage](portable_device_storage.jpg "Storage")

note<br>
- The USB_DEVICE_MTP_TRANSFER_BUFF_SIZE macro limits the device identification and data transfer speed.
   The larger the buffer size is, the faster the data transfer speed is. The buffer size should be a multiple of 512 with the smallest value being 1024.<br/>
- The USB_DEVICE_MTP_DIR_INSTANCE macro limits the maximum directory depth that can be created. 
   The larger the value, the deeper the directory can be created.<br/>

*/


## Supported Boards
- MIMXRT1170-EVKB
- LPCXpresso55S69
- EVK-MIMXRT1064
- MIMXRT685-AUD-EVK
- LPCXpresso54S018
- LPCXpresso54S018M
- MIMXRT1060-EVKB
- MIMXRT1040-EVK
- FRDM-MCXN947
- MIMXRT1024-EVK
- LPCXpresso55S28
- MCX-N5XX-EVK
- MIMXRT1060-EVKC
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/usb_examples/usb_device_mtp/example_board_readme.md)
- MIMXRT1180-EVK
- EVK-MIMXRT1020
- MIMXRT700-EVK
- EVK-MIMXRT595
- EVK-MIMXRT685
- MCX-N9XX-EVK
- EVKB-IMXRT1050
