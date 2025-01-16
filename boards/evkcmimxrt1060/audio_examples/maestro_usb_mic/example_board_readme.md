Hardware requirements
=====================
- 2x Micro USB cable
- JTAG/SWD debugger
- EVKC-IMXRT1060 board
- Personal Computer

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
5. Either press the reset button on your board or launch the debugger in your IDE to begin
   running the demo.

**Notes**
1. When connected to MacBook, change the PCM format from (0x02,0x00,) to (0x01,0x00, ) in
   g_config_descriptor[CONFIG_DESC_SIZE] in the usb_descriptor.c. Otherwise, it can't be
   enumerated and noise is present when recording with the QuickTime player because the
   sampling frequency and bit resolution do not match.
2. When device functionality is changed, please uninstall the previous PC driver to make
   sure the device with changed functionality can run normally.
3. If you're having audio problems on Windows 10 for recorder, please disable signal enhancement
   as the following if it is enabled and have a try again.

