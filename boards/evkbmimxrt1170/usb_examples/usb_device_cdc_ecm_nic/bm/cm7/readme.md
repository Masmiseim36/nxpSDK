# dev_cdc_ecm_nic

## Overview

The device CDC-ECM project is a simple demonstration program based on the MCUXpresso SDK.
It is enumerated as an Ethernet network adapter and users can access the network using the adapter.

## System Requirement

### Hardware requirements

- Mini/micro USB cable
- USB A to micro AB cable
- Hardware (Tower module/base board, and so on) for a specific device
- Personal Computer (PC)

### Software requirements

- The project path is:
  `MCUXpresso_SDK_Install`/boards/`board`/usb_examples/usb_device_cdc_ecm_nic/`rtos`/`toolchain`.

> The `rtos` means "Bare Metal" or "FreeRTOS OS", in path, it is "bm" or "freertos".

## Getting Started

### Hardware Settings

Set the hardware jumpers (Tower system/base module) to default settings.

### Prepare the example

1.  Download the program to the target board.
2.  Connect the target board to the external power source (the example is self-powered).
3.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
4.  Connect a USB cable between the PC host and the USB device port on the board.
    - For PC host, this demo only support for Linux-like OS & Mac OS.

> For detailed instructions, see the appropriate board User's Guide.

## Run the example

1. The demo will send string “USB CDC-ECM NIC Device” on the serial port to indicate that the USB CDC-ECM Device function is ready.
2. The PC enumerates the USB device normally and accesses the network via DHCP or manual configuration.

## Supported Boards

- MIMXRT1170-EVKB
- FRDM-MCXN947
- FRDM-RW612
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/usb_examples/usb_device_cdc_ecm_nic/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/usb_examples/usb_device_cdc_ecm_nic/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/usb_examples/usb_device_cdc_ecm_nic/example_board_readme.md)

