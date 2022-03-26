Overview
========
The maestro_demo application demonstrates audio processing on the ARM cortex core
utilizing the Maestro Audio Solutions library.

The application is controlled by commands from a shell interface using serial console.

Type "help" to see the command list.

After running the "usb_mic" command, the USB device will be enumerated on your host.
User will see the volume levels obtained from the USB host as in the example below.
This is just an example application. To leverage the values, the demo has to be modified.


Toolchain supported
===================
- IAR embedded Workbench  9.20.2
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.5.0

Hardware requirements
=====================
- 2x Micro USB cable
- JTAG/SWD debugger
- EVK-MIMXRT1170 board
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
5. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Note
1. When connected to MacBook, change the PCM format from (0x02,0x00,) to (0x01,0x00, ) in
   g_config_descriptor[CONFIG_DESC_SIZE] in the usb_descriptor.c. Otherwise, it can't be enumerated and
   noise is present when recording with the QuickTime player because the sampling frequency and bit resolution
   do not match.
2. When device functionality is changed, please uninstall the previous PC driver to make sure the device with changed functionality can run normally.
3. If you're having audio problems on Windows 10 for recorder, please disable signal enhancement as the following if it is enabled and have a try again.
Running the demo
================
When the example runs successfully, you should see similar output on the serial terminal as below:

*************************************************
Maestro audio USB microphone solutions demo start
*************************************************

[APP_SDCARD_Task] start
[APP_Shell_Task] start

SHELL build: Nov  5 2020
Copyright  2020  NXP

>>
>> usb_mic -1

Starting maestro usb microphone application
The application will run until the board restarts
[STREAMER] Message Task started
Starting recording
[STREAMER] start usb microphone
Set Cur Volume : 8001
Set Cur Volume : 22f8
Set Cur Volume : 265a
Set Cur Volume : 2ab6
Set Cur Volume : 30d9
Set Cur Volume : 3b4c
Set Cur Volume : 7fff
Set Cur Volume : 3b4c
