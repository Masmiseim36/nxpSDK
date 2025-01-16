# dev_composite_hid_audio_unified_lite_bm




## Overview

The USB Composite HID and Audio Unified application is a simple demonstration program based on the MCUXpresso SDK. 
It is enumerated as a playback and recording device. Users can record the sound from this device via the "Sound Recorder" in the Windows Accessories and play music with the device.


## System Requirement

### Hardware requirements

- Mini/micro USB cable
- USB A to micro AB cable
- Specific hardware which has codec presented
- Personal Computer


### Software requirements

- The project files for the lite version example are in: 
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_device_composite_hid_audio_unified_lite/<rtos>/
<toolchain>.
<br>  The project files for a non-lite version example are in: 
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_device_composite_hid_audio_unified/<rtos>/<toolchain>.
> The <rtos> is Bare Metal or FreeRTOS OS.


## Getting Started

### Hardware Settings

> Set the hardware jumpers (Tower system/base module) to default settings.


### Prepare the example

1.  Connect a USB cable between the PC host and the debugger USB port on the board to provide power supply (the example is self-powered).
2.  Download the program to the target board via IDE.
3.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
4.  Connect a USB cable between the PC host and the USB device port on the board.
5.  Demo start log is printed, note that the button name can be different for multiple boards.
<br>![Demo log](usb_device_composite_hid_audio_unified_start_log.jpg "Demo log")

For detailed instructions, see the appropriate board User's Guide.


## Run the example in Windows

> The mentioned steps can be different for Playback and Recording settings as per different Windows OS version.

1.  For the Audio recorder, a USB AUDIO+HID DEMO device is enumerated in the Device Manager.
<br>![The device enumerated in the device manager](usb_device_composite_hid_audio_device_manager.jpg "The device enumerated in the device manager")
2.  Open the COM port in a terminal tool, such as the Putty.
3.  Right click on the sound control icon in the Start bar (near the clock) and select "Recording devices".
<br>![Sound control icon](usb_device_composite_hid_audio_right_click_icon.jpg "Sound control icon")
4.  In the opened window, select the "Microphone" device with the description "USB Audio + HID Demo" and click on the "Properties" button.
<br>![Select properties](usb_device_composite_hid_audio_unified_recorder_property.jpg "Select properties")
5.  In the new window, go to "Levels" tab, move the slide until 100%, and click on "OK".
<br>![Change level](usb_device_composite_hid_audio_unified_recorder_change_level.jpg "Change level")
6.  Ensure that the selected "Microphone-USB AUDIO+HID DEMO" is display as Default device if no other microphone device supported by the Windows system. If it's not mention as default device then click on the "Set default" option to make "Microphone-AUDIO+HID DEMO" as a default device.
<br>![Set default](usb_device_composite_hid_audio_unified_recorder_default.jpg "Set default")
7.  Use the board specific hardware to record. For the boards which have SGTL board, plug in the line cable into slot J9 on SGTL board and connect the other end into PC or mobile phone.
8.  Open the "Sound Recorder" application and record audio about 5-10 seconds.
9.  After recording, open the recorder file with any media player and verify it by listening on the speaker of Laptop/PC.
10.  In the opened window, select the "Speakers" device with the description "USB Audio + HID Demo" and click on the "Properties" button.
<br>![Select properties](usb_device_composite_hid_audio_unified_speaker_property.jpg "Select properties")
11.  In the new window, go to "Levels" tab, move the slide until 100%, and click on "OK".
<br>![Change level](usb_device_composite_hid_audio_unified_speaker_change_level.jpg "Change level")
<br>![Auido control log](usb_device_composite_hid_audio_unified_speaker_control_log.jpg "Auido control log")
12.  Ensure that the selected "Speakers-USB AUDIO+HID DEMO" is display as Default device if no other speaker device supported by the Windows system. 
If it's not mention as default device then click on the "Set default" option to make "Speakers-USB AUDIO+HID DEMO" as a default device. 
<br>![Set default](usb_device_composite_hid_audio_unified_speaker_default.jpg "Set default")
13.  Connect the earphone/headphone at Audio Line Out port of target board. Open the Window Media Player application, select, and play your favorite song. Check that recorded audio is audible in the connected earphone/headphone at Audio Line Out port of target board.
14.  Use the mute/unmute button shown in the debug console's print information to mute/unmute speaker, it only control the speaker.

note<br>
1.  On some platforms, the recorder may have noise since the audio clock and USB SOF are not synchronized, this problem only happens on Windows but not on 
Mac OSX since the latter uses unified engine.
2.  USB audio class 2.0 is enabled by default.
3.  Based on the USB spec, the feedback endpoint data length should be 3 bytes if the device is full speed. In this case, device can work on Mac OS. However, device can not work on Win 10 and feedback data length must be set 4. There is a workaround to fix this issue, please open the macro
USB_DEVICE_WORKAROUND_AUDIO_20_WINDOWS when meets the following conditions:
<br> - USB device is full speed and USB audio class 2.0 is enabled.
<br> - USB device uses feedback endpoint.
<br> - USB host is Windows 10.
4.  When device functionality is changed, such as USB auido class 2.0 or UAC 5.1, please uninstall the previous PC driver to make sure the device with changed functionality can run normally.
5.  For the dev_composite_hid_audio_unified_lite_bm.bin, if it uses feedback endpoint, it can not work on Windows 10 if the speed mode is full speed, please run it on Linux or Mac OS.
6.  For IP3511HS in high speed mode, microframe can not be obtained and only for frame, the used feedback solution requires us to have to use larger latency and buffer size to avoid buffer overflow or underflow. Sync mode can use
    low latency (<1ms) even if on IP3511HS.
7.  If you're having audio problems on Windows 10, please disable signal enhancement if it is enabled and have a try again.
<br>![Disable signal enhancement](usb_device_audio_recorder_signal_enhancement.jpg "Disable signal enhancement")



## Supported Boards
- MIMXRT1170-EVKB
- LPCXpresso55S69
- EVK-MIMXRT1064
- MIMXRT685-AUD-EVK
- LPCXpresso55S16
- MIMXRT1060-EVKB
- EVK-MIMXRT1010
- MIMXRT1040-EVK
- MIMXRT1024-EVK
- LPCXpresso55S28
- LPCXpresso55S36
- MCX-N5XX-EVK
- MIMXRT1060-EVKC
- MIMXRT1160-EVK
- MIMXRT1180-EVK
- EVK-MIMXRT1020
- MIMXRT700-EVK
- EVK-MIMXRT595
- EVK-MIMXRT685
- EVKB-IMXRT1050
- RD-RW612-BGA
- EVK-MIMXRT1015
- FRDM-MCXN236
