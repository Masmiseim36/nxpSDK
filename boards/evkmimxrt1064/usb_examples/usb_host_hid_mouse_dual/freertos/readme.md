# host_hid_mouse_dual



## Overview

This Host HID example is a simple demonstration program based on the MCUXpresso SDK. 
<br> The application supports the mouse device. It prints the mouse operation when the mouse device is attached.

## System Requirement

### Hardware requirements

- Mini/micro USB cable
- USB A to micro AB cable
- Hardware (Tower module/base board, and so on) for a specific device
- Personal Computer (PC)


### Software requirements

- The project path is: 
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_host_hid_mouse/<rtos>/<toolchain>.
> The <rtos> is Bare Metal or FreeRTOS OS.


## Getting Started

### Hardware Settings

> Set the hardware jumpers (Tower system/base module) to default settings.


### Prepare the example 

1.  Download the program to the target board.
2.  Power off the target board and power on again.
3.  Connect devices to the board.

> For detailed instructions, see the appropriate board User's Guide.
> Host hid example doesn't support HID report descriptor analysis, this example assume that the device data are sent by specific order. 
      <br> For more detail, please refer to the code. For the device list we tested,
      <br> please refer to chapter "Peripheral devices tested with the USB Host stack" in "SDK Release Notes xxxx(board name)".

## Run the example

1.  Connect the board UART to the PC and open the COM port in a terminal tool.
2.  Make sure to use a USB HUB or an adapter with OTG functionality firstly. Plug in the HUB or the mouse device to the board. The attached information prints out in the terminal.
3.  The mouse operation information prints in the terminal when you operate the mouse. 
    <br> The application prints the mouse operation information in one line. Each line contains the following sequential string: 
    "Left Click", "Middle Click", "Right Click", "Right"/"Left" movement, "UP"/"Down" movement and "Wheel Down"/"Wheel Up" movement.
    White space replaces the above string if the mouse doesn't have the corresponding operation.
    <br> For example, when the mouse moves right and up, 
    <br> ``` "                                  Right UP             "```
    <br> prints in the terminal.

The following figure is an example to attach one mouse device.

<br>![Attach mouse](host_hid_mouse_output.jpg "Attach mouse")




## Supported Boards
- EVK-MIMXRT1064
- MIMXRT1060-EVKC
- EVKB-IMXRT1050
- MIMXRT1060-EVKB
