Hardware requirements
=====================
- 2x Micro USB cable
- JTAG/SWD debugger
- EVKC-IMXRT1060 board
- Personal Computer
- Headphones with 3.5 mm stereo jack

Board settings
==============
1. Set the hardware jumpers (Tower system/base module) to default settings.

Prepare the Demo
================
1. Connect the first micro USB cable between the PC host and the debug USB port on the board.
2. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Connect the second micro USB cable between the PC host and the USB port on the board.
5. Connect the earphone/headphone at Audio Line Out port of target board.
6. Either press the reset button on your board or launch the debugger in your IDE to begin
   running the demo.

**Notes**
1. If the USB device audio speaker example uses an ISO IN feedback endpoint, please attach
   the device to a host like PC which supports feedback function. Otherwise, there might be
   attachment issue or other problems.
2. This example supports UAC 5.1 and UAC 5.1 is disabled by default, this feature can be enabled
   by set macro USB_AUDIO_CHANNEL5_1 as 1U.
3. When device functionality is changed, such as UAC 5.1, please uninstall the previous PC
   driver to make sure the device with changed functionality can run normally.

