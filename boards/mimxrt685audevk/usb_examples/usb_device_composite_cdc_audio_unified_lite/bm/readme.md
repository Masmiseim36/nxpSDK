# dev_composite_cdc_audio_unified_lite_bm




## Overview

The Composite CDC and Audio Unified project is a simple demonstration program based on the MCUXpresso SDK.  
It is enumerated as a COM port and playback/recording device, which the COM port can be opened using terminal tools, such as TeraTerm. 
The purpose of this demo is to show how to build a composite USB device and to provide a simple example for further development.

## System Requirement

### Hardware requirements

- Mini/micro USB cable
- USB A to micro AB cable
- Hardware (Tower module/base board, and so on) for a specific device
- Personal Computer


### Software requirements

- The project files are in: 
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_device_composite_cdc_audio_unified/<rtos>/<toolchain>.
<br> For a lite version, the project files are in: 
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_device_composite_cdc_audio_unified/<rtos>/<toolchain>.
> The <rtos> is Bare Metal or FreeRTOS OS.


## Getting Started

### Hardware Settings

> Set the hardware jumpers (Tower system/base module) to default settings.


### Prepare the example 

1.  Connect a USB cable between the PC host and the debugger USB port on the board to provide power supply (the example is self-powered).
2.  Download the program to the target board via IDE.
3.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
4.  Connect a USB cable between the PC host and the USB device port on the board.
5.  Demo start log is printed, note that the log can be different for multiple boards.
<br>![Demo log](usb_device_composite_cdc_audio_unified_start_log.jpg "Demo log")

> For detailed instructions, see the appropriate board User's Guide.


## Run the example in Windows OS

> The mentioned steps can be different for Playback and Recording settings as per different Windows OS version.

1.  A COM port and a Audio Device is enumerated in the Device Manager.
<br>![The device enumerated in the device manager](usb_device_composite_cdc_audio_enumeration.jpg "The device enumerated in the device manager")
2.  Switch the terminal window connection to target USB port.
3.  Type some characters, which are echoed back from the COM port.
<br>![Run virtual com example](usb_device_composite_cdc_audio_vcom_type.jpg "Run virtual com example")
4.  Right click on the sound control icon in the Start bar (near the clock) and select "Recording devices".
5.  In the opened window, select the "Microphone" device with the description "USB Audio + CDC Demo" and click on the "Properties" button.
<br>![Select properties](usb_device_composite_cdc_audio_microphone.jpg "Select properties")
6.  In the new window, go to "Levels" tab, move the slide until 100%, and click on "OK".
<br>![Change level](usb_device_composite_cdc_audio_change_microphone_volume.jpg "Change level")
7.  Ensure that the selected "Microphone-USB AUDIO+CDC DEMO" is display as Default device if no other microphone device supported by the Windows system. If it's not mention as default device then click on the "Set default" option to make "Microphone-USB AUDIO+CDC DEMO" as a default device.
<br>![Set default](usb_device_composite_cdc_audio_microphone_set_default.jpg "Set default")
8.  Open the "Sound Recorder" application and record audio about 5-10 seconds.
9.  After recording, open the recorder file with any media player and verify it by listening on the speaker of Laptop/PC.
10.  Switch to "playback" tab and select the "Speakers" device with the description "USB Audio + CDC Demo" and click on the "Properties" button.
<br>![Select properties](usb_device_composite_cdc_audio_speaker.jpg "Select properties")
11.  In the new window, go to "Levels" tab, move the slide until 100%, and click on "OK".
<br>![Change level](usb_device_composite_cdc_audio_change_speaker_volume.jpg "Change level")
12.  Ensure that the selected "Speakers-USB AUDIO+CDC DEMO" is display as Default device if no other speaker device supported by the Windows system. 
If it's not mention as default device then click on the "Set default" option to make "Speakers-USB AUDIO+CDC DEMO" as a default device.  
<br>![Set default](usb_device_composite_cdc_audio_speaker_set_default.jpg "Set default")
13.  Connect the earphone/headphone at Audio Line Out port of target board. Open the Window Media Player application, select, and play your favorite song. Check that recorded audio is audible in the connected earphone/headphone at Audio Line Out port of target board.

note<br>
1.  USB audio class 2.0 is enabled by default.
2.  Based on the USB spec, the feedback endpoint data length should be 3 bytes if the device is full speed. In this case, device can work on Mac OS. However, device can not work on Win 10 and feedback data length must be set 4. There is a workaround to fix this issue, please open the macro
USB_DEVICE_WORKAROUND_AUDIO_20_WINDOWS when meets the following conditions:
<br> - USB device is full speed and USB audio class 2.0 is enabled.
<br> - USB device uses feedback endpoint.
<br> - USB host is Windows 10.
3.  When device functionality is changed, such as USB auido class 2.0 or UAC 5.1, please uninstall the previous PC driver to make sure the device with changed functionality can run normally.
4.  For the dev_composite_cdc_audio_unified_lite_bm.bin, if it uses feedback endpoint, it can not work on Windows 10 if the speed mode is full speed, please run it on Linux or Mac OS.
5.  For IP3511HS in high speed mode, microframe can not be obtained and only for frame, the used feedback solution requires us to have to use larger latency and buffer size to avoid buffer overflow or underflow. Sync mode can use
    low latency (<1ms) even if on IP3511HS.
6.  If you're having audio problems on Windows 10, please disable signal enhancement if it is enabled and have a try again.
<br>![Disable signal enhancement](usb_device_audio_recorder_signal_enhancement.jpg "Disable signal enhancement")
*/





## Supported Boards
- EVK-MIMXRT685
- EVK-MIMXRT595
- MIMXRT685-AUD-EVK
