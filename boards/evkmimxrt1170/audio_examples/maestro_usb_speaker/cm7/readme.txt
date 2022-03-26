Overview
========
The maestro_demo application demonstrates audio processing on the ARM cortex core
utilizing the Maestro Audio Solutions library.

The application is controlled by commands from a shell interface using serial console.

Type "help" to see the command list.

After running the "usb_speaker" command, the USB device will be enumerated on your host.
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
6. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Note
1. If the USB device audio speaker example uses an ISO IN feedback endpoint, please attach the device to a host like
   PC which supports feedback function. Otherwise, there might be attachment issue or other problems.
2. This example supports UAC 5.1 and UAC 5.1 is disabled by default, this feature can be enabled by set macro
   USB_AUDIO_CHANNEL5_1 as 1U.
3. When device functionality is changed, such as UAC 5.1, please uninstall the previous PC driver to make sure
   the device with changed functionality can run normally.
Running the demo
================
When the example runs successfully, you should see similar output on the serial terminal as below:

*************************************************
Maestro audio USB speaker solutions demo start
*************************************************

[APP_SDCARD_Task] start
[APP_Shell_Task] start

SHELL build: Nov  5 2020
Copyright  2020  NXP

>>
>> usb_speaker -1

Starting maestro usb speaker application
The application will run until the board restarts
[STREAMER] Message Task started
Starting playing
[STREAMER] start usb speaker
Set Cur Volume : 0
Set Cur Mute : 0
Set Cur Volume : 9e
Set Cur Volume : 144
Set Cur Volume : 199
Set Cur Volume : 1f1
Set Cur Volume : 24a
Set Cur Volume : 2a6
