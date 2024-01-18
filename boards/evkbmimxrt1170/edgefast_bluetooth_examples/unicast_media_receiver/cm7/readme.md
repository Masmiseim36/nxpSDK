Overview
========
Application demonstrating how to use the unicast media receiver feature.

There should be three boards: 1 UMS + 1 UMR(left) + 1 UMR(right).
UMS: unicast stereo audio stream, left channel on first CIS and right channel on another CIS.
UMR: receive one of CIS channel and render it.


SDK version
===========
- Version: 2.15.000

Toolchain supported
===================
- MCUXpresso  11.8.0
- IAR embedded Workbench  9.40.1
- Keil MDK  5.38.1
- GCC ARM Embedded  12.2

Hardware requirements
=====================
- Micro USB cable
- evkbmimxrt1170 board
- Personal Computer
- Embedded Artists 2EL M.2 Module (Rev-A1) - direct M2 connection.

Jumper settings for RT1170-EVKB (enables external 5V supply):
remove  J38 5-6
connect J38 1-2
connect J43 with external power(controlled by SW5)
connect J25-15 with J97
connect J25-13 with 2EL's GPIO_27

Murata Solution Board settings
Embedded Artists 2EL module datasheet: https://www.embeddedartists.com/doc/ds/2EL_M2_Datasheet.pdf

The hardware should be reworked according to the Hardware Rework Guide for MIMXRT1170-EVKB and Murata 1XK M.2 Adapter in document Hardware Rework Guide for EdgeFast BT PAL.
The hardware rework for MIMXRT1170-EVKB and Murata 2EL M.2 Adapter is same as MIMXRT1170-EVKB and Murata 1XK M.2 Adapter.

Note:
To ensure that the LITTLEFS flash region has been cleaned,
all flash sectors need to be erased before downloading example code.
After downloaded binary into qspiflash and boot from qspiflash directly,
please reset the board by pressing SW4 or power off and on the board to run the application.
Prepare the Demo
================

1.  Open example's project and build it.

2.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

3.  Provide 5V voltage for the target board.

4.  Connect speaker to Audio Jack.

5.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

6.  Download the program to the target board.

7.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
The log below shows the output of the example in the terminal window.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Copyright  2022  NXP

UMR>> 
Unicast Media Receiver.

Please select sink role "left"|"right" use "init" command.

UMR>> init left

UMR@left>> UMR@left>> Bluetooth initialized
Location successfully set
Supported contexts successfully set
Available contexts successfully set
Advertising successfully started
Connected: D0:17:69:EE:71:13 (public)
Security changed: D0:17:69:EE:71:13 (public) level 2 (error 0)
MCS server discover:
MCS server discovered.
ASE Codec Config: conn 202DD4EC ep 202D08C4 dir 1
codec 0x06 cid 0x0000 vid 0x0000 count 5
data #0: type 0x01 len 1

data #1: type 0x02 len 1

data #2: type 0x03 len 4
010000
data #3: type 0x04 len 2
28
data #4: type 0x05 len 1

  Frequency: 16000 Hz
  Frame Duration: 10000 us
  Channel allocation: 0x1
  Octets per frame: 40 (negative means value not pressent)
  Frames per SDU: 1
ASE Codec Config stream 2030A0B8
QoS: stream 2030A0B8 qos 202D0934
QoS: interval 10000 framing 0x00 phy 0x02 sdu 40 rtn 2 latency 10 pd 40000
Enable: stream 2030A0B8 meta_count 1
	Codec: freq 16000, channel count 1, duration 10000, channel alloc 0x00000001, frame len 40, frame blocks per sdu 1
Unicast stream started
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Procedures to run
1 input "help" to show command list
2 input "init left" or "init right" to start sink role and start to advertise, audio will start playing after connect and configure finished.
3 input "pause" to stop playing.
4 input "play" to start playing.
5 input "vol_up", "vol_down", "vol_set" to set volume.
6 input "vol_mute", "vol_unmute" to set mute.

Note:
1 "exit" command is a shell internal command, only used to exit shell module and could not used to exit demo.
