# dev_lpm_hid_mouse




## Overview

The LPM(Link Power Management)USB HID mouse application is a simple demonstration program based on the MCUXpresso SDK.
It is enumerated as a mouse. Users can see the mouse arrow moving on the PC screen according in a rectangular fashion.
When host send LPM token, the device will enter into low power mode. The device can be waked up when resume signal is detected on the bus.
Or the device can remote wake-up the host by delivering the resume signal when the remote wake-up is enabled by the host.

## System Requirements

### Hardware requirements

- Mini/micro USB cable
- USB A to micro AB cable
- Hardware (Tower module/base board, and so on) for a specific device
- Personal Computer (PC)


### Software requirements

- The project files are in:
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_lpm_device_hid_mouse/<rtos>/<toolchain>.
<br> For lite version, the project files are in:
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_lpm_device_hid_mouse_lite/<rtos>/<toolchain>.
> The <rtos> is Bare Metal or FreeRTOS OS.


## Getting Started

### Hardware Settings

> Set the hardware jumpers (Tower system/base module) to default settings.


### Prepare the example

> For detailed instructions, see the appropriate board User's Guide.
<br>For this example, we only test with host lpm example, have not verify this exampel with pc and so on.
<br>For this example could be tested with the board that support "usb_lpm_host_hid_mouse" example,such as LPCXpresso54608.
## Run the example

1.  Plug-in the device, which is running LPM HID mouse example, into the PC supporting LPM. A HID-compliant mouse is enumerated in the Device Manager.
2.  The mouse arrow is moving on PC screen in the rectangular rotation.
3.  Make sure your PC enable the USB remote wake support in BIOS.Press the sleep button to make your PC go to sleep and the usb bus is idle. Then the device enters into low power mode.
4.  If the remote wake-up feature is enabled by the host, the device is waked up and delivers the resume signal when the wakeup switch is pressed.
    The name of wakeup switch is showed in debug console when the remote wake-up feature is enabled.
    For example, if the name of wakeup switch is "SW3", the debug console output is as following. 
    ```
        USB device remote wakeup state: 1
        USB device start sleep
        USB device sleeped.
        Please Press wakeup switch(SW3) to remote wakeup the host.
        Remote wakeup the host.
        USB device start resume
        USB device resumed.
    ```
    <br> Or the device is waked up when the resume signal is detected on the bus.
    ```
        USB device remote wakeup state: 0
        USB device start sleepd
        USB device sleeped.
        USB device start resume
        USB device resumed.
    ```



*/


## Supported Boards
- EVK-MIMXRT685
- MIMXRT700-EVK
- MIMXRT685-AUD-EVK
- [LPCXpresso54S018](../../_boards/lpcxpresso54s018/usb_examples/usb_lpm_device_hid_mouse/example_board_readme.md)
- EVK-MIMXRT595
- [LPCXpresso54S018M](../../_boards/lpcxpresso54s018m/usb_examples/usb_lpm_device_hid_mouse/example_board_readme.md)
- [LPCXpresso54628](../../_boards/lpcxpresso54628/usb_examples/usb_lpm_device_hid_mouse/example_board_readme.md)
