# keyboard2mouse



## Overview

This example implements the host and the device, where the one controller works as a host and the other controller works as a device.
<br> The host supports the keyboard device and device works as a mouse when connecting to the PC.

## System Requirement

### Hardware requirements

- Mini/micro USB cable
- USB A to micro AB cable
- Hardware (Tower module/base board, and so on) for a specific device
- Personal Computer(PC)


### Software requirements

- The project path is: 
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_keyboard2mouse/<rtos>/<toolchain>.
> The <rtos> is Bare Metal or FreeRTOS OS.


## Getting Started

### Hardware Settings

> First, set the hardware jumpers (Tower system/base module) to default settings.


### Prepare the example 

1.  Download the program to the target board.
2.  Power off the target board and power on again.
3.  Connect devices to the board and connect a USB cable between the PC and the USB device port of the board.

> For detailed instructions, see the appropriate board User's Guide.
> Host hid example doesn't support HID report descriptor analysis, this example assume that the device data are sent by specific order. 
      <br> For more detail, please refer to the code. For the device list we tested,
      <br> please refer to chapter "Peripheral devices tested with the USB Host stack" in "SDK Release Notes xxxx(board name)".

## Run the example

1.  Connect the board UART to the PC and open the COM port in a terminal tool.
2.  Plug in a hub or a keyboard device to the full-speed only port. The attached information prints out in the terminal.
3.  Plug in the device high-speed port into the PC. An HID-compliant mouse is enumerated in the Device Manager.
4.  Press <w, s, a, d> in the keyboard, which causes the mouse to move.
    - Press 'w', the mouse move up.
    - Press 's', the mouse move down.
    - Press 'a', the mouse move left.
    - Press 'd', the mouse move right.



## Supported Boards
- MIMXRT1170-EVKB
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/usb_examples/usb_keyboard2mouse/example_board_readme.md)
- [EVK-MIMXRT1064](../../_boards/evkmimxrt1064/usb_examples/usb_keyboard2mouse/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/usb_examples/usb_keyboard2mouse/example_board_readme.md)
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/usb_examples/usb_keyboard2mouse/example_board_readme.md)
- [LPCXpresso55S28](../../_boards/lpcxpresso55s28/usb_examples/usb_keyboard2mouse/example_board_readme.md)
- MIMXRT1160-EVK
- MIMXRT1180-EVK
- [LPCXpresso54S018](../../_boards/lpcxpresso54s018/usb_examples/usb_keyboard2mouse/example_board_readme.md)
- [LPCXpresso55S16](../../_boards/lpcxpresso55s16/usb_examples/usb_keyboard2mouse/example_board_readme.md)
- [LPCXpresso54S018M](../../_boards/lpcxpresso54s018m/usb_examples/usb_keyboard2mouse/example_board_readme.md)
- [LPCXpresso54628](../../_boards/lpcxpresso54628/usb_examples/usb_keyboard2mouse/example_board_readme.md)
- [MIMXRT1060-EVKB](../../_boards/evkbmimxrt1060/usb_examples/usb_keyboard2mouse/example_board_readme.md)
